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

#include "waylander/wayland/connected_client.hpp"
#include "waylander/wayland/message_visitor.hpp"
#include "waylander/wayland/protocols/wayland_protocol.hpp"
#include "waylander/wayland/protocols/xdg_shell_protocol.hpp"

#include "guilander/static_picture.hpp"

namespace guilander {

class static_picture_window {
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

    // Requests:
    using wl_display_sync              = wl_display::request::sync;
    using wl_registry_bind             = wl_registry::request::bind;
    using wl_display_get_registry      = wl_display::request::get_registry;
    using wl_shm_create_pool           = wl_shm::request::create_pool;
    using wl_shm_pool_create_buffer    = wl_shm_pool::request::create_buffer;
    using wl_shm_pool_destroy          = wl_shm_pool::request::destroy;
    using wl_compositor_create_surface = wl_compositor::request::create_surface;
    using wl_surface_commit            = wl_surface::request::commit;
    using wl_surface_attach            = wl_surface::request::attach;
    using xdg_wm_base_get_xdg_surface  = xdg_wm_base::request::get_xdg_surface;
    using xdg_wm_base_pong             = xdg_wm_base::request::pong;
    using xdg_surface_get_toplevel     = xdg_surface::request::get_toplevel;
    using xdg_surface_ack_configure    = xdg_surface::request::ack_configure;

    // Events:
    using wl_callback_done      = wl_callback::event::done;
    using wl_registry_global    = wl_registry::event::global;
    using wl_display_error      = wl_display::event::error;
    using xdg_wm_base_ping      = xdg_wm_base::event::ping;
    using xdg_surface_configure = xdg_surface::event::configure;
    using xdg_toplevel_close    = xdg_toplevel::event::close;

    waylander::wl::connected_client client_ = {};

    template<typename W>
    using Wobject = waylander::wl::Wobject<W>;

    Wobject<wl_display>    display_obj_id_ = { waylander::wl::global_display_object };
    Wobject<wl_registry>   registry_obj_id_;
    Wobject<wl_callback>   sync_obj_id_;
    Wobject<wl_compositor> compositor_obj_id_;
    Wobject<wl_shm>        shm_obj_id_;
    Wobject<wl_buffer>     buffer_obj_id_;
    Wobject<wl_surface>    wl_surface_obj_id_;
    Wobject<xdg_wm_base>   wm_base_obj_id_;
    Wobject<xdg_surface>   xdg_surface_obj_id_;
    Wobject<xdg_toplevel>  toplevel_obj_id_;

    static_picture picture_;

    /// Handels: wl_display:error
    auto
    fatal_error_overloads_() -> waylander::wl::message_overload_set;

    /// Handels: wl_display::error, xdg_wm_base::ping
    ///
    /// Returned overload set should not escape the scope of the member function which
    /// called this function, as this pointer is stored in it which could be invalidated.
    auto
    default_overloads_() -> waylander::wl::message_overload_set;

  public:
    /// Connects to compositor, enumerates and binds globals
    [[nodiscard]] explicit static_picture_window(static_picture pic);

    void
    wait_for_close_event();
};

} // namespace guilander
