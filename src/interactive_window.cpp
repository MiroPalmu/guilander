// Copyright (C) 2025 Miro Palmu.
//
// This file is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This file is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this file.  If not, see <https://www.gnu.org/licenses/>.

#include <thread>

#include <concepts>
#include <cstdlib>
#include <format>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <tuple>

#include "guilander/interactive_window.hpp"

namespace guilander {

auto
interactive_window::fatal_error_overloads_() -> waylander::wl::message_overload_set {
    auto vis = waylander::wl::message_overload_set{};
    vis.add_overload<wl_display::event::error>(display_obj_, [](const auto& err) {
        const auto err_type_str = [&]() -> std::string {
            switch (static_cast<wl_display::error>(err.code.value)) {
                case wl_display::error::Einvalid_object: return "invalid object";
                case wl_display::error::Einvalid_method: return "invalid method";
                case wl_display::error::Eno_memory: return "no memory";
                case wl_display::error::Eimplementation: return "implementation";
                default: return "non-global error";
            }
        }();

        throw std::runtime_error{ std::format(
            "Received global display object error event:\n"
            "    object id: {}\n"
            "    error type: {}\n"
            "    desc: {}\n",
            err.object_id.value,
            err_type_str,
            std::string_view(reinterpret_cast<char const*>(err.message.data()),
                             err.message.size())) };
    });

    return vis;
}

auto
interactive_window::default_overloads_() -> waylander::wl::message_overload_set {
    auto vis = fatal_error_overloads_();
    vis.add_overload<xdg_wm_base::event::ping>(wm_base_obj_, [this](const auto ping) {
        client_.register_request(wm_base_obj_, xdg_wm_base::request::pong{ ping.serial });
    });
    vis.add_overload<xdg_toplevel::event::close>(xdg_toplevel_obj_, [this](const auto) {
        closed_ = true;
        throw std::runtime_error{ "closed" };
    });
    vis.add_overload<xdg_toplevel::event::configure>(xdg_toplevel_obj_, [this](const auto) {
        // todo: resizing, store xdg_toplevel::state
    });
    vis.add_overload<xdg_surface::event::configure>(xdg_surface_obj_, [this](const auto conf) {
        client_.register_request(xdg_surface_obj_,
                                 xdg_surface::request::ack_configure{ conf.serial });
        // todo: apply staging_buffer_
    });

    // There might be a way to reason about the order of releases,
    // but I do not want to do it, so we can not know if the buffers have switched.
    auto mark_released = [this](const auto buff_obj) {
        if (buff_obj == staging_buffer_.buffer_obj_) {
            staging_buffer_.used_by_compositor_ = false;
        } else {
            user_buffer_.used_by_compositor_ = false;
        }
    };

    vis.add_overload<wl_buffer::event::release>(
        staging_buffer_.buffer_obj_,
        [this, mark_released](const auto) { mark_released(staging_buffer_.buffer_obj_); });
    vis.add_overload<wl_buffer::event::release>(
        user_buffer_.buffer_obj_,
        [this, mark_released](const auto) { mark_released(user_buffer_.buffer_obj_); });
    vis.add_overload<wl_callback::event::done>(frame_callback_, [this](const auto) {
        frame_event_received_ = true;
    });
    return vis;
}

[[nodiscard]] interactive_window::interactive_window(static_picture initial_frame)
    : staging_buffer_{ .picture_{ std::invoke([&] {
                           auto pic =
                               guilander::static_picture(initial_frame.width, initial_frame.height);
                           std::ranges::copy(initial_frame.bytes, pic.bytes.begin());
                           return pic;
                       }) } },
      user_buffer_{ .picture_{ std::move(initial_frame) } } {
    client_.register_request(
        display_obj_,
        wl_display::request::get_registry{ registry_obj_.reserve_id(client_) });

    const auto globals_sync = client_.reserve_object_id<wl_callback>();
    client_.register_request(display_obj_, wl_display::request::sync{ globals_sync });
    client_.flush_registered_requests();

    auto globals_binder = fatal_error_overloads_();

    auto wl_shm_found        = false;
    auto wl_compositor_found = false;
    auto xdg_wm_base_found   = false;

    globals_binder.add_overload<wl_registry::event::global>(registry_obj_, [&](const auto& msg) {
        const auto id = [&]() -> std::optional<waylander::wl::Wnew_id<>> {
            if (msg.interface == u8"wl_shm") {
                // wl_shm is a signleton so it is expected to find only one.
                wl_shm_found = true;
                return shm_obj_.reserve_id(client_);
            } else if (msg.interface == u8"wl_compositor") {
                // wl_compositor is a signleton so it is expected to find only one.
                wl_compositor_found = true;
                return compositor_obj_.reserve_id(client_);
            } else if (not xdg_wm_base_found and msg.interface == u8"xdg_wm_base") {
                // xdg_wm_base is not specified as singleton so bind just to the first one.
                xdg_wm_base_found = true;
                return wm_base_obj_.reserve_id(client_);
            }
            return {};
        }();
        if (id) {
            client_.register_request(
                registry_obj_,
                wl_registry::request::bind{ msg.name, msg.interface, msg.version, id.value() });
        }
    });

    client_.recv_and_visit_events(globals_binder).until<wl_callback::event::done>(globals_sync);

    if (not(wl_shm_found and wl_compositor_found and xdg_wm_base_found)) {
        throw std::runtime_error{ std::format(
            "All wanted globals not found!\n"
            "Found (wl_shm, wl_compositor, xdg_wm_base): ({}, {}, {})\n",
            wl_shm_found,
            wl_compositor_found,
            xdg_wm_base_found) };
    }

    // All globals are now found and we can create all required objects.

    // We start with staging buffer:

    const auto staging_shm_pool_obj = client_.reserve_object_id<wl_shm_pool>();
    const auto create_staging_pool =
        wl_shm::request::create_pool{ .id = staging_shm_pool_obj,
                                      .fd{ gnulander::fd_ref{ staging_buffer_.picture_.buff } },
                                      .size{ static_cast<waylander::wl::Wint::integral_type>(
                                          staging_buffer_.picture_.bytes.size()) } };
    const auto create_staging_buffer = wl_shm_pool::request::create_buffer{
        .id     = staging_buffer_.buffer_obj_.reserve_id(client_),
        .offset = { 0 },
        .width  = { staging_buffer_.picture_.width },
        .height = { staging_buffer_.picture_.height },
        .stride = { staging_buffer_.picture_.width
                    * static_cast<int>(sizeof(static_picture::pixel)) },
        .format = static_picture::color_format
    };

    client_.register_request(shm_obj_, create_staging_pool);
    client_.register_request(staging_shm_pool_obj, create_staging_buffer);
    client_.register_request(staging_shm_pool_obj, wl_shm_pool::request::destroy{});

    // Then user buffer:

    const auto user_shm_pool_obj = client_.reserve_object_id<wl_shm_pool>();
    const auto create_user_pool  = wl_shm::request::create_pool{
         .id = user_shm_pool_obj,
         .fd{ gnulander::fd_ref{ user_buffer_.picture_.buff } },
         .size{ static_cast<waylander::wl::Wint::integral_type>(user_buffer_.picture_.bytes.size()) }
    };
    const auto create_user_buffer = wl_shm_pool::request::create_buffer{
        .id     = user_buffer_.buffer_obj_.reserve_id(client_),
        .offset = { 0 },
        .width  = { user_buffer_.picture_.width },
        .height = { user_buffer_.picture_.height },
        .stride = { user_buffer_.picture_.width * static_cast<int>(sizeof(static_picture::pixel)) },
        .format = static_picture::color_format
    };

    client_.register_request(shm_obj_, create_user_pool);
    client_.register_request(user_shm_pool_obj, create_user_buffer);
    client_.register_request(user_shm_pool_obj, wl_shm_pool::request::destroy{});

    // Lastly we want wl_surface, xdg_surface and xdg_toplevel:

    const auto create_surface =
        wl_compositor::request::create_surface{ wl_surface_obj_.reserve_id(client_) };
    const auto get_xdg_surface =
        xdg_wm_base::request::get_xdg_surface{ .id      = xdg_surface_obj_.reserve_id(client_),
                                               .surface = wl_surface_obj_ };
    const auto get_toplevel =
        xdg_surface::request::get_toplevel{ xdg_toplevel_obj_.reserve_id(client_) };

    client_.register_request(compositor_obj_, create_surface);
    client_.register_request(wm_base_obj_, get_xdg_surface);
    client_.register_request(xdg_surface_obj_, get_toplevel);
    client_.register_request(wl_surface_obj_, wl_surface::request::commit{});

    // xdg_surface::request::ack_configure has to be requested before attaching buffer.
    const auto initial_configuration_sync = client_.reserve_object_id<wl_callback>();
    client_.register_request(display_obj_, wl_display::request::sync{ initial_configuration_sync });
    client_.flush_registered_requests();

    // This is little jank as we would like to just say ::one_past instead of ::until.
    auto vis = default_overloads_();
    client_.recv_and_visit_events(vis).until<xdg_surface::event::configure>(
        xdg_surface_obj_,
        [this](const auto conf) {
            client_.register_request(xdg_surface_obj_,
                                     xdg_surface::request::ack_configure{ conf.serial });
            // todo: apply staging_buffer_
        });

    // Attach staging buffer to the surface.
    client_.register_request(
        wl_surface_obj_,
        wl_surface::request::attach{ .buffer = staging_buffer_.buffer_obj_, .x = 0, .y = 0 });
    staging_buffer_.used_by_compositor_ = true;
    client_.register_request(wl_surface_obj_, wl_surface::request::commit{});
    client_.flush_registered_requests();
}

void
interactive_window::commit() {
    if (not frame_event_received_) { this->frame_throttle(); }

    // Ask new frame event.
    frame_event_received_ = false;
    client_.register_request(wl_surface_obj_,
                             wl_surface::request::frame{ frame_callback_.reserve_id(client_) });

    // Attach user buffer to the surface.
    client_.register_request(
        wl_surface_obj_,
        wl_surface::request::attach{ .buffer = user_buffer_.buffer_obj_, .x = 0, .y = 0 });
    user_buffer_.used_by_compositor_ = true;

    // Commit with the user buffer attached.
    client_.register_request(wl_surface_obj_, wl_surface::request::commit{});
    const auto commit_sync = client_.reserve_object_id<wl_callback>();
    client_.register_request(display_obj_, wl_display::request::sync{ commit_sync });
    client_.flush_registered_requests();

    auto mos = default_overloads_();
    client_.recv_and_visit_events(mos).until<wl_callback::event::done>(commit_sync);

    // Make sure the staging buffer is ready to take user modifications.
    while (staging_buffer_.used_by_compositor_) {
        const auto s = client_.reserve_object_id<wl_callback>();
        client_.register_request(display_obj_, wl_display::request::sync{ s });
        client_.recv_and_visit_events(mos).until<wl_callback::event::done>(s);
    }

    std::ranges::swap(staging_buffer_.buffer_obj_, user_buffer_.buffer_obj_);
    std::ranges::swap(staging_buffer_.picture_, user_buffer_.picture_);
    std::ranges::swap(staging_buffer_.used_by_compositor_, user_buffer_.used_by_compositor_);
}

void
interactive_window::frame_throttle() {
    auto mos = default_overloads_();
    while (not frame_event_received_) {
        const auto s = client_.reserve_object_id<wl_callback>();
        client_.register_request(display_obj_, wl_display::request::sync{ s });
        client_.flush_registered_requests();
        client_.recv_and_visit_events(mos).until<wl_callback::event::done>(s);
    }
}

} // namespace guilander
