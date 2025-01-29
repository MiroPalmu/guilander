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

#pragma once

#include <optional>

#include "waylander/wayland/connected_client.hpp"
#include "waylander/wayland/message_visitor.hpp"
#include "waylander/wayland/protocols/wayland_protocol.hpp"
#include "waylander/wayland/protocols/xdg_shell_protocol.hpp"

#include "guilander/static_picture.hpp"
#include "guilander/units.hpp"

namespace guilander {

/// Double buffered interactive window.
///
/// Assumed pixel format is argb8888.
///
/// Contains user and staging buffers which are held in buffer_state_t.
/// Users can manipulate the user buffer with public member functions between commits.
/// During commit user buffer is given to the compositor and events are synced.
/// Compositor interacts with the staging buffer and during each xdg_surface configuration
/// will sync user buffer with staging buffer.
///
/// Stages:
///     1. bind to globals (in ctor)
///         - wl_display
///         - wl_registry
///         - wl_compositor
///         - wl_shm
///         - xdg_wm_base
///     2. construct objects required later (in ctor)
///         - wl_buffer (x2, user and staging)
///         - wl_surface
///         - xdg_surfcae
///         - xdg_toplevel
///     3. sync object construction and update user buffer based on events (in ctor)
///     4. take user configuration to staging buffer (public methods)
///     5. wait for frame throttle event (public method or automatically in commit)
///     6. request frame throttle (in commit method)
///     7. attach user buffer and make swap it with staging buffer (in commit method)
///     8. wl_surface::commit (in commit method)
///     9. sync commit (in commit method)
///         - handle events until sync callback for commit
///         - wait until the previously made staging buffer is released
///     10. goto stage 4
///
/// Order of steps 4 and 5 should not matter.
///
/// In steps 3, 5 and 9 when waiting for specific sync event,
/// following events should be handeled (default_overloads_):
///
/// |-------------------------+-----------------------------------------|
/// | Event                   | Reaction                                |
/// |-------------------------+-----------------------------------------|
/// | wl_dispaly::error       | throw std::runtime_error                |
/// |-------------------------+-----------------------------------------|
/// | wl_buffer::release      | mark buffer usable                      |
/// |-------------------------+-----------------------------------------|
/// | xdg_wm_base::ping       | register xdg_wm_base::pong              |
/// |-------------------------+-----------------------------------------|
/// | xdg_surface::configure  | copy staging state to user state (todo) |
/// |                         | register xdg_wm_base::ack_configuren    |
/// |-------------------------+-----------------------------------------|
/// | xdg_toplevel::configure | set staging state frame size (todo)     |
/// |-------------------------+-----------------------------------------|
/// | xdg_toplevel::close     | store                                   |
/// |-------------------------+-----------------------------------------|
class interactive_window {
    // Interfaces:
    using wl_registry   = waylander::wl::protocols::wl_registry;
    using wl_display    = waylander::wl::protocols::wl_display;
    using wl_callback   = waylander::wl::protocols::wl_callback;
    using wl_shm        = waylander::wl::protocols::wl_shm;
    using wl_shm_pool   = waylander::wl::protocols::wl_shm_pool;
    using wl_buffer     = waylander::wl::protocols::wl_buffer;
    using wl_compositor = waylander::wl::protocols::wl_compositor;
    using wl_surface    = waylander::wl::protocols::wl_surface;
    using xdg_wm_base   = waylander::wl::protocols::xdg_wm_base;
    using xdg_surface   = waylander::wl::protocols::xdg_surface;
    using xdg_toplevel  = waylander::wl::protocols::xdg_toplevel;

    waylander::wl::connected_client client_ = {};

    template<typename W>
    using Wobject = waylander::wl::Wobject<W>;

    // Globals:
    Wobject<wl_display>    display_obj_ = { waylander::wl::global_display_object };
    Wobject<wl_registry>   registry_obj_;
    Wobject<wl_compositor> compositor_obj_;
    Wobject<wl_shm>        shm_obj_;

    Wobject<wl_surface>   wl_surface_obj_;
    Wobject<xdg_wm_base>  wm_base_obj_;
    Wobject<xdg_surface>  xdg_surface_obj_;
    Wobject<xdg_toplevel> xdg_toplevel_obj_;

    Wobject<wl_callback> frame_callback_;
    bool                 frame_event_received_{ true };

    struct staging_buffer_type {
        Wobject<wl_buffer> buffer_obj_{};
        static_picture     picture_;
        // todo: see default_overloads_
        // units::pixel_size_t wanted_width_ {};
        // unist::pixel_size_t wanted_height_ {};
        bool used_by_compositor_{ false };
    } staging_buffer_;

    struct user_buffer_type {
        Wobject<wl_buffer> buffer_obj_{};
        static_picture     picture_;
        bool               used_by_compositor_{ false };
    } user_buffer_;

    bool closed_{ false };

    /// Handels wl_display::event::error
    auto
    fatal_error_overloads_() -> waylander::wl::message_overload_set;

    /// Includes fatar_error_overloads_()
    auto
    default_overloads_() -> waylander::wl::message_overload_set;

  public:
    [[nodiscard]] explicit interactive_window(static_picture initial_frame);

    [[nodiscard]] auto
    damage(wl_surface::request::damage dmg) {
        client_.register_request(wl_surface_obj_, dmg);

        auto user_buffer_mds = user_buffer_.picture_.view_mdpixels();
        return stdex::submdspan(user_buffer_mds,
                                std::tuple{ dmg.y.value, dmg.y.value + dmg.height.value },
                                std::tuple{ dmg.x.value, dmg.x.value + dmg.width.value });
    };

    void
    commit();

    void
    frame_throttle();
};

} // namespace guilander
