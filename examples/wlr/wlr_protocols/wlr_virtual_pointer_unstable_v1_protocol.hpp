// Generated from Wayland xml protocol: wlr_virtual_pointer_unstable_v1

/// @file
/// wlr_virtual_pointer_unstable_v1 xml protocol
///
/// Copyright © 2019 Josef Gajdusek
///
/// Permission is hereby granted, free of charge, to any person obtaining a
/// copy of this software and associated documentation files (the "Software"),
/// to deal in the Software without restriction, including without limitation
/// the rights to use, copy, modify, merge, publish, distribute, sublicense,
/// and/or sell copies of the Software, and to permit persons to whom the
/// Software is furnished to do so, subject to the following conditions:
///
/// The above copyright notice and this permission notice (including the next
/// paragraph) shall be included in all copies or substantial portions of the
/// Software.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
/// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
/// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
/// DEALINGS IN THE SOFTWARE.

#pragma once

#include "waylander/wayland/protocol_primitives.hpp"

namespace waylander {
namespace wl {
namespace protocols {

/// Declare everything before they might be used.

struct zwlr_virtual_pointer_v1 {
    enum class error : Wint::integral_type;

    struct request {
        struct motion;
        struct motion_absolute;
        struct button;
        struct axis;
        struct frame;
        struct axis_source;
        struct axis_stop;
        struct axis_discrete;
        struct destroy;
    };

    struct event {
    };
};
struct zwlr_virtual_pointer_manager_v1 {
    struct request {
        struct create_virtual_pointer;
        struct destroy;
        struct create_virtual_pointer_with_output;
    };

    struct event {
    };
};
enum class zwlr_virtual_pointer_v1::error : Wint::integral_type {
    /// client sent invalid axis enumeration value
    Einvalid_axis = 0,
    /// client sent invalid axis source enumeration value
    Einvalid_axis_source = 1
};

/// pointer relative motion event
///
/// The pointer has moved by a relative amount to the previous request.
///
/// Values are in the global compositor space.
struct zwlr_virtual_pointer_v1::request::motion {
    static constexpr Wopcode<zwlr_virtual_pointer_v1> opcode{ 0 };
    /// timestamp with millisecond granularity
    Wuint time;
    /// displacement on the x-axis
    Wfixed dx;
    /// displacement on the y-axis
    Wfixed dy;
};

/// pointer absolute motion event
///
/// The pointer has moved in an absolute coordinate frame.
///
/// Value of x can range from 0 to x_extent, value of y can range from 0
/// to y_extent.
struct zwlr_virtual_pointer_v1::request::motion_absolute {
    static constexpr Wopcode<zwlr_virtual_pointer_v1> opcode{ 1 };
    /// timestamp with millisecond granularity
    Wuint time;
    /// position on the x-axis
    Wuint x;
    /// position on the y-axis
    Wuint y;
    /// extent of the x-axis
    Wuint x_extent;
    /// extent of the y-axis
    Wuint y_extent;
};

/// button event
///
/// A button was pressed or released.
struct zwlr_virtual_pointer_v1::request::button {
    static constexpr Wopcode<zwlr_virtual_pointer_v1> opcode{ 2 };
    /// timestamp with millisecond granularity
    Wuint time;
    /// button that produced the event
    Wuint button;
    /// physical state of the button
    wl_pointer::button_state state;
};

/// axis event
///
/// Scroll and other axis requests.
struct zwlr_virtual_pointer_v1::request::axis {
    static constexpr Wopcode<zwlr_virtual_pointer_v1> opcode{ 3 };
    /// timestamp with millisecond granularity
    Wuint time;
    /// axis type
    wl_pointer::axis axis;
    /// length of vector in touchpad coordinates
    Wfixed value;
};

/// end of a pointer event sequence
///
/// Indicates the set of events that logically belong together.
struct zwlr_virtual_pointer_v1::request::frame {
    static constexpr Wopcode<zwlr_virtual_pointer_v1> opcode{ 4 };
};

/// axis source event
///
/// Source information for scroll and other axis.
struct zwlr_virtual_pointer_v1::request::axis_source {
    static constexpr Wopcode<zwlr_virtual_pointer_v1> opcode{ 5 };
    /// source of the axis event
    wl_pointer::axis_source axis_source;
};

/// axis stop event
///
/// Stop notification for scroll and other axes.
struct zwlr_virtual_pointer_v1::request::axis_stop {
    static constexpr Wopcode<zwlr_virtual_pointer_v1> opcode{ 6 };
    /// timestamp with millisecond granularity
    Wuint time;
    /// the axis stopped with this event
    wl_pointer::axis axis;
};

/// axis click event
///
/// Discrete step information for scroll and other axes.
///
/// This event allows the client to extend data normally sent using the axis
/// event with discrete value.
struct zwlr_virtual_pointer_v1::request::axis_discrete {
    static constexpr Wopcode<zwlr_virtual_pointer_v1> opcode{ 7 };
    /// timestamp with millisecond granularity
    Wuint time;
    /// axis type
    wl_pointer::axis axis;
    /// length of vector in touchpad coordinates
    Wfixed value;
    /// number of steps
    Wint discrete;
};

/// destroy the virtual pointer object
struct zwlr_virtual_pointer_v1::request::destroy {
    static constexpr Wopcode<zwlr_virtual_pointer_v1> opcode{ 8 };
};

/// Create a new virtual pointer
///
/// Creates a new virtual pointer. The optional seat is a suggestion to the
/// compositor.
struct zwlr_virtual_pointer_manager_v1::request::create_virtual_pointer {
    static constexpr Wopcode<zwlr_virtual_pointer_manager_v1> opcode{ 0 };
    Wobject<wl_seat> seat;
    Wnew_id<zwlr_virtual_pointer_v1> id;
};

/// destroy the virtual pointer manager
struct zwlr_virtual_pointer_manager_v1::request::destroy {
    static constexpr Wopcode<zwlr_virtual_pointer_manager_v1> opcode{ 1 };
};

/// Create a new virtual pointer
///
/// Creates a new virtual pointer. The seat and the output arguments are
/// optional. If the seat argument is set, the compositor should assign the
/// input device to the requested seat. If the output argument is set, the
/// compositor should map the input device to the requested output.
struct zwlr_virtual_pointer_manager_v1::request::create_virtual_pointer_with_output {
    static constexpr Wopcode<zwlr_virtual_pointer_manager_v1> opcode{ 2 };
    Wobject<wl_seat> seat;
    Wobject<wl_output> output;
    Wnew_id<zwlr_virtual_pointer_v1> id;
};

} // namespace protocols
} // namespace wl
} // namespace waylander
