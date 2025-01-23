// Generated from Wayland xml protocol: wlr_foreign_toplevel_management_unstable_v1

/// @file
/// wlr_foreign_toplevel_management_unstable_v1 xml protocol
///
/// Copyright © 2018 Ilia Bozhinov
///
/// Permission to use, copy, modify, distribute, and sell this
/// software and its documentation for any purpose is hereby granted
/// without fee, provided that the above copyright notice appear in
/// all copies and that both that copyright notice and this permission
/// notice appear in supporting documentation, and that the name of
/// the copyright holders not be used in advertising or publicity
/// pertaining to distribution of the software without specific,
/// written prior permission.  The copyright holders make no
/// representations about the suitability of this software for any
/// purpose.  It is provided "as is" without express or implied
/// warranty.
///
/// THE COPYRIGHT HOLDERS DISCLAIM ALL WARRANTIES WITH REGARD TO THIS
/// SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND
/// FITNESS, IN NO EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE FOR ANY
/// SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
/// WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN
/// AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
/// ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF
/// THIS SOFTWARE.

#pragma once

#include "waylander/wayland/protocol_primitives.hpp"

namespace waylander {
namespace wl {
namespace protocols {

/// Declare everything before they might be used.

struct zwlr_foreign_toplevel_manager_v1 {
    struct request {
        struct stop;
    };

    struct event {
        struct toplevel;
        struct finished;
    };
};
struct zwlr_foreign_toplevel_handle_v1 {
    enum class state : Wint::integral_type;
    enum class error : Wint::integral_type;

    struct request {
        struct set_maximized;
        struct unset_maximized;
        struct set_minimized;
        struct unset_minimized;
        struct activate;
        struct close;
        struct set_rectangle;
        struct destroy;
        struct set_fullscreen;
        struct unset_fullscreen;
    };

    struct event {
        struct title;
        struct app_id;
        struct output_enter;
        struct output_leave;
        struct state;
        struct done;
        struct closed;
        struct parent;
    };
};
/// stop sending events
///
/// Indicates the client no longer wishes to receive events for new toplevels.
/// However the compositor may emit further toplevel_created events, until
/// the finished event is emitted.
///
/// The client must not send any more requests after this one.
struct zwlr_foreign_toplevel_manager_v1::request::stop {
    static constexpr Wopcode<zwlr_foreign_toplevel_manager_v1> opcode{ 0 };
};

/// a toplevel has been created
///
/// This event is emitted whenever a new toplevel window is created. It
/// is emitted for all toplevels, regardless of the app that has created
/// them.
///
/// All initial details of the toplevel(title, app_id, states, etc.) will
/// be sent immediately after this event via the corresponding events in
/// zwlr_foreign_toplevel_handle_v1.
struct zwlr_foreign_toplevel_manager_v1::event::toplevel {
    static constexpr Wopcode<zwlr_foreign_toplevel_manager_v1> opcode{ 0 };
    Wnew_id<zwlr_foreign_toplevel_handle_v1> toplevel;
};

/// the compositor has finished with the toplevel manager
///
/// This event indicates that the compositor is done sending events to the
/// zwlr_foreign_toplevel_manager_v1. The server will destroy the object
/// immediately after sending this request, so it will become invalid and
/// the client should free any resources associated with it.
struct zwlr_foreign_toplevel_manager_v1::event::finished {
    static constexpr Wopcode<zwlr_foreign_toplevel_manager_v1> opcode{ 1 };
};

/// types of states on the toplevel
///
/// The different states that a toplevel can have. These have the same meaning
/// as the states with the same names defined in xdg-toplevel
enum class zwlr_foreign_toplevel_handle_v1::state : Wint::integral_type {
    /// the toplevel is maximized
    Emaximized = 0,
    /// the toplevel is minimized
    Eminimized = 1,
    /// the toplevel is active
    Eactivated = 2,
    /// the toplevel is fullscreen
    Efullscreen = 3
};

enum class zwlr_foreign_toplevel_handle_v1::error : Wint::integral_type {
    /// the provided rectangle is invalid
    Einvalid_rectangle = 0
};

/// requests that the toplevel be maximized
///
/// Requests that the toplevel be maximized. If the maximized state actually
/// changes, this will be indicated by the state event.
struct zwlr_foreign_toplevel_handle_v1::request::set_maximized {
    static constexpr Wopcode<zwlr_foreign_toplevel_handle_v1> opcode{ 0 };
};

/// requests that the toplevel be unmaximized
///
/// Requests that the toplevel be unmaximized. If the maximized state actually
/// changes, this will be indicated by the state event.
struct zwlr_foreign_toplevel_handle_v1::request::unset_maximized {
    static constexpr Wopcode<zwlr_foreign_toplevel_handle_v1> opcode{ 1 };
};

/// requests that the toplevel be minimized
///
/// Requests that the toplevel be minimized. If the minimized state actually
/// changes, this will be indicated by the state event.
struct zwlr_foreign_toplevel_handle_v1::request::set_minimized {
    static constexpr Wopcode<zwlr_foreign_toplevel_handle_v1> opcode{ 2 };
};

/// requests that the toplevel be unminimized
///
/// Requests that the toplevel be unminimized. If the minimized state actually
/// changes, this will be indicated by the state event.
struct zwlr_foreign_toplevel_handle_v1::request::unset_minimized {
    static constexpr Wopcode<zwlr_foreign_toplevel_handle_v1> opcode{ 3 };
};

/// activate the toplevel
///
/// Request that this toplevel be activated on the given seat.
/// There is no guarantee the toplevel will be actually activated.
struct zwlr_foreign_toplevel_handle_v1::request::activate {
    static constexpr Wopcode<zwlr_foreign_toplevel_handle_v1> opcode{ 4 };
    Wobject<wl_seat> seat;
};

/// request that the toplevel be closed
///
/// Send a request to the toplevel to close itself. The compositor would
/// typically use a shell-specific method to carry out this request, for
/// example by sending the xdg_toplevel.close event. However, this gives
/// no guarantees the toplevel will actually be destroyed. If and when
/// this happens, the zwlr_foreign_toplevel_handle_v1.closed event will
/// be emitted.
struct zwlr_foreign_toplevel_handle_v1::request::close {
    static constexpr Wopcode<zwlr_foreign_toplevel_handle_v1> opcode{ 5 };
};

/// the rectangle which represents the toplevel
///
/// The rectangle of the surface specified in this request corresponds to
/// the place where the app using this protocol represents the given toplevel.
/// It can be used by the compositor as a hint for some operations, e.g
/// minimizing. The client is however not required to set this, in which
/// case the compositor is free to decide some default value.
///
/// If the client specifies more than one rectangle, only the last one is
/// considered.
///
/// The dimensions are given in surface-local coordinates.
/// Setting width=height=0 removes the already-set rectangle.
struct zwlr_foreign_toplevel_handle_v1::request::set_rectangle {
    static constexpr Wopcode<zwlr_foreign_toplevel_handle_v1> opcode{ 6 };
    Wobject<wl_surface> surface;
    Wint x;
    Wint y;
    Wint width;
    Wint height;
};

/// destroy the zwlr_foreign_toplevel_handle_v1 object
///
/// Destroys the zwlr_foreign_toplevel_handle_v1 object.
///
/// This request should be called either when the client does not want to
/// use the toplevel anymore or after the closed event to finalize the
/// destruction of the object.
struct zwlr_foreign_toplevel_handle_v1::request::destroy {
    static constexpr Wopcode<zwlr_foreign_toplevel_handle_v1> opcode{ 7 };
};

/// request that the toplevel be fullscreened
///
/// Requests that the toplevel be fullscreened on the given output. If the
/// fullscreen state and/or the outputs the toplevel is visible on actually
/// change, this will be indicated by the state and output_enter/leave
/// events.
///
/// The output parameter is only a hint to the compositor. Also, if output
/// is NULL, the compositor should decide which output the toplevel will be
/// fullscreened on, if at all.
struct zwlr_foreign_toplevel_handle_v1::request::set_fullscreen {
    static constexpr Wopcode<zwlr_foreign_toplevel_handle_v1> opcode{ 8 };
    Wobject<wl_output> output;
};

/// request that the toplevel be unfullscreened
///
/// Requests that the toplevel be unfullscreened. If the fullscreen state
/// actually changes, this will be indicated by the state event.
struct zwlr_foreign_toplevel_handle_v1::request::unset_fullscreen {
    static constexpr Wopcode<zwlr_foreign_toplevel_handle_v1> opcode{ 9 };
};

/// title change
///
/// This event is emitted whenever the title of the toplevel changes.
struct zwlr_foreign_toplevel_handle_v1::event::title {
    static constexpr Wopcode<zwlr_foreign_toplevel_handle_v1> opcode{ 0 };
    Wstring title;
};

/// app-id change
///
/// This event is emitted whenever the app-id of the toplevel changes.
struct zwlr_foreign_toplevel_handle_v1::event::app_id {
    static constexpr Wopcode<zwlr_foreign_toplevel_handle_v1> opcode{ 1 };
    Wstring app_id;
};

/// toplevel entered an output
///
/// This event is emitted whenever the toplevel becomes visible on
/// the given output. A toplevel may be visible on multiple outputs.
struct zwlr_foreign_toplevel_handle_v1::event::output_enter {
    static constexpr Wopcode<zwlr_foreign_toplevel_handle_v1> opcode{ 2 };
    Wobject<wl_output> output;
};

/// toplevel left an output
///
/// This event is emitted whenever the toplevel stops being visible on
/// the given output. It is guaranteed that an entered-output event
/// with the same output has been emitted before this event.
struct zwlr_foreign_toplevel_handle_v1::event::output_leave {
    static constexpr Wopcode<zwlr_foreign_toplevel_handle_v1> opcode{ 3 };
    Wobject<wl_output> output;
};

/// the toplevel state changed
///
/// This event is emitted immediately after the zlw_foreign_toplevel_handle_v1
/// is created and each time the toplevel state changes, either because of a
/// compositor action or because of a request in this protocol.
struct zwlr_foreign_toplevel_handle_v1::event::state {
    static constexpr Wopcode<zwlr_foreign_toplevel_handle_v1> opcode{ 4 };
    Warray state;
};

/// all information about the toplevel has been sent
///
/// This event is sent after all changes in the toplevel state have been
/// sent.
///
/// This allows changes to the zwlr_foreign_toplevel_handle_v1 properties
/// to be seen as atomic, even if they happen via multiple events.
struct zwlr_foreign_toplevel_handle_v1::event::done {
    static constexpr Wopcode<zwlr_foreign_toplevel_handle_v1> opcode{ 5 };
};

/// this toplevel has been destroyed
///
/// This event means the toplevel has been destroyed. It is guaranteed there
/// won't be any more events for this zwlr_foreign_toplevel_handle_v1. The
/// toplevel itself becomes inert so any requests will be ignored except the
/// destroy request.
struct zwlr_foreign_toplevel_handle_v1::event::closed {
    static constexpr Wopcode<zwlr_foreign_toplevel_handle_v1> opcode{ 6 };
};

/// parent change
///
/// This event is emitted whenever the parent of the toplevel changes.
///
/// No event is emitted when the parent handle is destroyed by the client.
struct zwlr_foreign_toplevel_handle_v1::event::parent {
    static constexpr Wopcode<zwlr_foreign_toplevel_handle_v1> opcode{ 7 };
    Wobject<zwlr_foreign_toplevel_handle_v1> parent;
};

} // namespace protocols
} // namespace wl
} // namespace waylander
