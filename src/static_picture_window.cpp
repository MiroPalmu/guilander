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

#include <cstdlib>
#include <format>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>

#include "guilander/static_picture_window.hpp"

namespace guilander {

auto
static_picture_window::fatal_error_overloads_() -> waylander::wl::message_overload_set {
    auto vis = waylander::wl::message_overload_set{};
    vis.add_overload<wl_display_error>(display_obj_id_, [](const wl_display_error& err) {
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
static_picture_window::default_overloads_() -> waylander::wl::message_overload_set {
    auto vis = fatal_error_overloads_();
    vis.add_overload<xdg_wm_base_ping>(wm_base_obj_id_, [this](const xdg_wm_base_ping ping) {
        client_.register_request(wm_base_obj_id_, xdg_wm_base_pong{ ping.serial });
    });
    return vis;
}

[[nodiscard]] static_picture_window::static_picture_window(static_picture pic)
    : picture_{ std::move(pic) } {
    // Initial stage:

    const auto get_reg = wl_display_get_registry{ registry_obj_id_.reserve_id(client_) };
    const auto sync    = wl_display_sync{ sync_obj_id_.reserve_id(client_) };
    client_.register_request(display_obj_id_, get_reg);
    client_.register_request(display_obj_id_, sync);
    client_.flush_registered_requests();

    auto globals_binder = fatal_error_overloads_();

    auto wl_shm_found        = false;
    auto wl_compositor_found = false;
    auto xdg_wm_base_found   = false;

    globals_binder.add_overload<wl_registry_global>(
        registry_obj_id_,
        [&](const wl_registry_global& msg) {
            const auto id = [&]() -> std::optional<waylander::wl::Wnew_id<>> {
                if (msg.interface == u8"wl_shm") {
                    // wl_shm is a signleton so it is expected to find only one.
                    wl_shm_found = true;
                    return shm_obj_id_.reserve_id(client_);
                } else if (msg.interface == u8"wl_compositor") {
                    // wl_compositor is a signleton so it is expected to find only one.
                    wl_compositor_found = true;
                    return compositor_obj_id_.reserve_id(client_);
                } else if (not xdg_wm_base_found and msg.interface == u8"xdg_wm_base") {
                    // xdg_wm_base is not specified as singleton so bind just to the first one.
                    xdg_wm_base_found = true;
                    return wm_base_obj_id_.reserve_id(client_);
                }
                return {};
            }();
            if (id) {
                client_.register_request(
                    registry_obj_id_,
                    wl_registry_bind{ msg.name, msg.interface, msg.version, id.value() });
            }
        });

    client_.recv_and_visit_events(globals_binder).until<wl_callback_done>(sync_obj_id_);
    if (not(wl_shm_found and wl_compositor_found and xdg_wm_base_found)) {
        throw std::runtime_error{ std::format(
            "All wanted globals not found!\n"
            "Found (wl_shm, wl_compositor, xdg_wm_base): ({}, {}, {})\n",
            wl_shm_found,
            wl_compositor_found,
            xdg_wm_base_found) };
    }
    client_.flush_registered_requests();

    // Config stage:

    const auto shm_pool_obj_id = client_.reserve_object_id<wl_shm_pool>();
    const auto create_pool     = wl_shm_create_pool{
            .id   = shm_pool_obj_id,
            .fd   = gnulander::fd_ref{ picture_.buff },
            .size = static_cast<waylander::wl::Wint::integral_type>(picture_.bytes.size())
    };
    const auto create_buffer =
        wl_shm_pool_create_buffer{ .id     = buffer_obj_id_.reserve_id(client_),
                                   .offset = 0,
                                   .width  = picture_.width,
                                   .height = picture_.height,
                                   .stride = picture_.width
                                             * static_cast<int>(sizeof(static_picture::pixel)),
                                   .format = static_picture::color_format };

    // This will trigger format events from the wl_shm object,
    // but as we are using Eargb8888 which is required to be supported by all compositors,
    // we can ignore those events happily.
    client_.register_request(shm_obj_id_, create_pool);
    client_.register_request(shm_pool_obj_id, create_buffer);
    // As the buffers store reference to reference counted pool,
    // it can be immediatly destroyed.
    client_.register_request(shm_pool_obj_id, wl_shm_pool_destroy{});

    const auto create_surface =
        wl_compositor_create_surface{ wl_surface_obj_id_.reserve_id(client_) };
    const auto get_xdg_surface =
        xdg_wm_base_get_xdg_surface{ .id      = xdg_surface_obj_id_.reserve_id(client_),
                                     .surface = wl_surface_obj_id_ };
    const auto get_toplevel = xdg_surface_get_toplevel{ toplevel_obj_id_.reserve_id(client_) };
    client_.register_request(compositor_obj_id_, create_surface);
    client_.register_request(wm_base_obj_id_, get_xdg_surface);
    client_.register_request(xdg_surface_obj_id_, get_toplevel);
    client_.register_request(wl_surface_obj_id_, wl_surface_commit{});
    client_.flush_registered_requests();

    // Now we wait for xdg_surface_configure to mark the end of a configure sequence.
    // Before it there could be events advertising states of wl_surface or xdg_toplevel,
    // but we do not care about them, so they are ignored.
    auto config_stage_handler = default_overloads_();
    client_.recv_and_visit_events(config_stage_handler)
        .until<xdg_surface_configure>(xdg_surface_obj_id_, [&](const auto msg) {
            client_.register_request(xdg_surface_obj_id_, xdg_surface_ack_configure{ msg.serial });
        });

    client_.register_request(wl_surface_obj_id_,
                             wl_surface_attach{ .buffer = buffer_obj_id_, .x = 0, .y = 0 });
    client_.register_request(wl_surface_obj_id_, wl_surface_commit{});
    client_.flush_registered_requests();
}

void
static_picture_window::wait_for_close_event() {
    auto vis = default_overloads_();
    client_.recv_and_visit_events(vis).until<xdg_toplevel_close>(toplevel_obj_id_, [](auto) {});
}

} // namespace guilander
