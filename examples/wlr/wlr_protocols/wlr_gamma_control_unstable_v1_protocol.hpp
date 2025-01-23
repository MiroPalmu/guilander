// Generated from Wayland xml protocol: wlr_gamma_control_unstable_v1

/// @file
/// manage gamma tables of outputs
///
/// This protocol allows a privileged client to set the gamma tables for
/// outputs.
///
/// Warning! The protocol described in this file is experimental and
/// backward incompatible changes may be made. Backward compatible changes
/// may be added together with the corresponding interface version bump.
/// Backward incompatible changes are done by bumping the version number in
/// the protocol and interface names and resetting the interface version.
/// Once the protocol is to be declared stable, the 'z' prefix and the
/// version number in the protocol and interface names are removed and the
/// interface version number is reset.
///
/// Copyright © 2015 Giulio camuffo
/// Copyright © 2018 Simon Ser
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

struct zwlr_gamma_control_manager_v1 {
    struct request {
        struct get_gamma_control;
        struct destroy;
    };

    struct event {
    };
};
struct zwlr_gamma_control_v1 {
    enum class error : Wint::integral_type;

    struct request {
        struct set_gamma;
        struct destroy;
    };

    struct event {
        struct gamma_size;
        struct failed;
    };
};
/// get a gamma control for an output
///
/// Create a gamma control that can be used to adjust gamma tables for the
/// provided output.
struct zwlr_gamma_control_manager_v1::request::get_gamma_control {
    static constexpr Wopcode<zwlr_gamma_control_manager_v1> opcode{ 0 };
    Wnew_id<zwlr_gamma_control_v1> id;
    Wobject<wl_output> output;
};

/// destroy the manager
///
/// All objects created by the manager will still remain valid, until their
/// appropriate destroy request has been called.
struct zwlr_gamma_control_manager_v1::request::destroy {
    static constexpr Wopcode<zwlr_gamma_control_manager_v1> opcode{ 1 };
};

enum class zwlr_gamma_control_v1::error : Wint::integral_type {
    /// invalid gamma tables
    Einvalid_gamma = 1
};

/// set the gamma table
///
/// Set the gamma table. The file descriptor can be memory-mapped to provide
/// the raw gamma table, which contains successive gamma ramps for the red,
/// green and blue channels. Each gamma ramp is an array of 16-byte unsigned
/// integers which has the same length as the gamma size.
///
/// The file descriptor data must have the same length as three times the
/// gamma size.
struct zwlr_gamma_control_v1::request::set_gamma {
    static constexpr Wopcode<zwlr_gamma_control_v1> opcode{ 0 };
    /// gamma table file descriptor
    Wfd fd;
};

/// destroy this control
///
/// Destroys the gamma control object. If the object is still valid, this
/// restores the original gamma tables.
struct zwlr_gamma_control_v1::request::destroy {
    static constexpr Wopcode<zwlr_gamma_control_v1> opcode{ 1 };
};

/// size of gamma ramps
///
/// Advertise the size of each gamma ramp.
///
/// This event is sent immediately when the gamma control object is created.
struct zwlr_gamma_control_v1::event::gamma_size {
    static constexpr Wopcode<zwlr_gamma_control_v1> opcode{ 0 };
    /// number of elements in a ramp
    Wuint size;
};

/// object no longer valid
///
/// This event indicates that the gamma control is no longer valid. This
/// can happen for a number of reasons, including:
/// - The output doesn't support gamma tables
/// - Setting the gamma tables failed
/// - Another client already has exclusive gamma control for this output
/// - The compositor has transferred gamma control to another client
///
/// Upon receiving this event, the client should destroy this object.
struct zwlr_gamma_control_v1::event::failed {
    static constexpr Wopcode<zwlr_gamma_control_v1> opcode{ 1 };
};

} // namespace protocols
} // namespace wl
} // namespace waylander
