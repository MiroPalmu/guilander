// Generated from Wayland xml protocol: wlr_output_power_management_unstable_v1

/// @file
/// Control power management modes of outputs
///
/// This protocol allows clients to control power management modes
/// of outputs that are currently part of the compositor space. The
/// intent is to allow special clients like desktop shells to power
/// down outputs when the system is idle.
///
/// To modify outputs not currently part of the compositor space see
/// wlr-output-management.
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
/// Copyright © 2019 Purism SPC
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

struct zwlr_output_power_manager_v1 {
    struct request {
        struct get_output_power;
        struct destroy;
    };

    struct event {
    };
};
struct zwlr_output_power_v1 {
    enum class mode : Wint::integral_type;
    enum class error : Wint::integral_type;

    struct request {
        struct set_mode;
        struct destroy;
    };

    struct event {
        struct mode;
        struct failed;
    };
};
/// get a power management for an output
///
/// Create an output power management mode control that can be used to
/// adjust the power management mode for a given output.
struct zwlr_output_power_manager_v1::request::get_output_power {
    static constexpr Wopcode<zwlr_output_power_manager_v1> opcode{ 0 };
    Wnew_id<zwlr_output_power_v1> id;
    Wobject<wl_output> output;
};

/// destroy the manager
///
/// All objects created by the manager will still remain valid, until their
/// appropriate destroy request has been called.
struct zwlr_output_power_manager_v1::request::destroy {
    static constexpr Wopcode<zwlr_output_power_manager_v1> opcode{ 1 };
};

enum class zwlr_output_power_v1::mode : Wint::integral_type {
    /// Output is turned off.
    Eoff = 0,
    /// Output is turned on, no power saving
    Eon = 1
};

enum class zwlr_output_power_v1::error : Wint::integral_type {
    /// nonexistent power save mode
    Einvalid_mode = 1
};

/// Set an outputs power save mode
///
/// Set an output's power save mode to the given mode. The mode change
/// is effective immediately. If the output does not support the given
/// mode a failed event is sent.
struct zwlr_output_power_v1::request::set_mode {
    static constexpr Wopcode<zwlr_output_power_v1> opcode{ 0 };
    /// the power save mode to set
    zwlr_output_power_v1::mode mode;
};

/// destroy this power management
///
/// Destroys the output power management mode control object.
struct zwlr_output_power_v1::request::destroy {
    static constexpr Wopcode<zwlr_output_power_v1> opcode{ 1 };
};

/// Report a power management mode change
///
/// Report the power management mode change of an output.
///
/// The mode event is sent after an output changed its power
/// management mode. The reason can be a client using set_mode or the
/// compositor deciding to change an output's mode.
/// This event is also sent immediately when the object is created
/// so the client is informed about the current power management mode.
struct zwlr_output_power_v1::event::mode {
    static constexpr Wopcode<zwlr_output_power_v1> opcode{ 0 };
    /// the output's new power management mode
    zwlr_output_power_v1::mode mode;
};

/// object no longer valid
///
/// This event indicates that the output power management mode control
/// is no longer valid. This can happen for a number of reasons,
/// including:
/// - The output doesn't support power management
/// - Another client already has exclusive power management mode control
/// for this output
/// - The output disappeared
///
/// Upon receiving this event, the client should destroy this object.
struct zwlr_output_power_v1::event::failed {
    static constexpr Wopcode<zwlr_output_power_v1> opcode{ 1 };
};

} // namespace protocols
} // namespace wl
} // namespace waylander
