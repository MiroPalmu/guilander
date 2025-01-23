// Generated from Wayland xml protocol: wlr_input_inhibit_unstable_v1

/// @file
/// wlr_input_inhibit_unstable_v1 xml protocol
///
/// Copyright © 2018 Drew DeVault
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

struct zwlr_input_inhibit_manager_v1 {
    enum class error : Wint::integral_type;

    struct request {
        struct get_inhibitor;
    };

    struct event {
    };
};
struct zwlr_input_inhibitor_v1 {
    struct request {
        struct destroy;
    };

    struct event {
    };
};
enum class zwlr_input_inhibit_manager_v1::error : Wint::integral_type {
    /// an input inhibitor is already in use on the compositor
    Ealready_inhibited = 0
};

/// inhibit input to other clients
///
/// Activates the input inhibitor. As long as the inhibitor is active, the
/// compositor will not send input events to other clients.
struct zwlr_input_inhibit_manager_v1::request::get_inhibitor {
    static constexpr Wopcode<zwlr_input_inhibit_manager_v1> opcode{ 0 };
    Wnew_id<zwlr_input_inhibitor_v1> id;
};

/// destroy the input inhibitor object
///
/// Destroy the inhibitor and allow other clients to receive input.
struct zwlr_input_inhibitor_v1::request::destroy {
    static constexpr Wopcode<zwlr_input_inhibitor_v1> opcode{ 0 };
};

} // namespace protocols
} // namespace wl
} // namespace waylander
