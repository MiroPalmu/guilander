// Generated from Wayland xml protocol: wlr_screencopy_unstable_v1

/// @file
/// screen content capturing on client buffers
///
/// This protocol allows clients to ask the compositor to copy part of the
/// screen content to a client buffer.
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
/// Copyright © 2018 Simon Ser
/// Copyright © 2019 Andri Yngvason
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

struct zwlr_screencopy_manager_v1 {
    struct request {
        struct capture_output;
        struct capture_output_region;
        struct destroy;
    };

    struct event {
    };
};
struct zwlr_screencopy_frame_v1 {
    enum class error : Wint::integral_type;
    enum class flags : Wuint::integral_type;

    struct request {
        struct copy;
        struct destroy;
        struct copy_with_damage;
    };

    struct event {
        struct buffer;
        struct flags;
        struct ready;
        struct failed;
        struct damage;
        struct linux_dmabuf;
        struct buffer_done;
    };
};
/// capture an output
///
/// Capture the next frame of an entire output.
struct zwlr_screencopy_manager_v1::request::capture_output {
    static constexpr Wopcode<zwlr_screencopy_manager_v1> opcode{ 0 };
    Wnew_id<zwlr_screencopy_frame_v1> frame;
    /// composite cursor onto the frame
    Wint overlay_cursor;
    Wobject<wl_output> output;
};

/// capture an output's region
///
/// Capture the next frame of an output's region.
///
/// The region is given in output logical coordinates, see
/// xdg_output.logical_size. The region will be clipped to the output's
/// extents.
struct zwlr_screencopy_manager_v1::request::capture_output_region {
    static constexpr Wopcode<zwlr_screencopy_manager_v1> opcode{ 1 };
    Wnew_id<zwlr_screencopy_frame_v1> frame;
    /// composite cursor onto the frame
    Wint overlay_cursor;
    Wobject<wl_output> output;
    Wint x;
    Wint y;
    Wint width;
    Wint height;
};

/// destroy the manager
///
/// All objects created by the manager will still remain valid, until their
/// appropriate destroy request has been called.
struct zwlr_screencopy_manager_v1::request::destroy {
    static constexpr Wopcode<zwlr_screencopy_manager_v1> opcode{ 2 };
};

enum class zwlr_screencopy_frame_v1::error : Wint::integral_type {
    /// the object has already been used to copy a wl_buffer
    Ealready_used = 0,
    /// buffer attributes are invalid
    Einvalid_buffer = 1
};

enum class zwlr_screencopy_frame_v1::flags : Wuint::integral_type {
    /// contents are y-inverted
    Ey_invert = 1
};

/// copy the frame
///
/// Copy the frame to the supplied buffer. The buffer must have a the
/// correct size, see zwlr_screencopy_frame_v1.buffer and
/// zwlr_screencopy_frame_v1.linux_dmabuf. The buffer needs to have a
/// supported format.
///
/// If the frame is successfully copied, a "flags" and a "ready" events are
/// sent. Otherwise, a "failed" event is sent.
struct zwlr_screencopy_frame_v1::request::copy {
    static constexpr Wopcode<zwlr_screencopy_frame_v1> opcode{ 0 };
    Wobject<wl_buffer> buffer;
};

/// delete this object, used or not
///
/// Destroys the frame. This request can be sent at any time by the client.
struct zwlr_screencopy_frame_v1::request::destroy {
    static constexpr Wopcode<zwlr_screencopy_frame_v1> opcode{ 1 };
};

/// copy the frame when it's damaged
///
/// Same as copy, except it waits until there is damage to copy.
struct zwlr_screencopy_frame_v1::request::copy_with_damage {
    static constexpr Wopcode<zwlr_screencopy_frame_v1> opcode{ 2 };
    Wobject<wl_buffer> buffer;
};

/// wl_shm buffer information
///
/// Provides information about wl_shm buffer parameters that need to be
/// used for this frame. This event is sent once after the frame is created
/// if wl_shm buffers are supported.
struct zwlr_screencopy_frame_v1::event::buffer {
    static constexpr Wopcode<zwlr_screencopy_frame_v1> opcode{ 0 };
    /// buffer format
    wl_shm::format format;
    /// buffer width
    Wuint width;
    /// buffer height
    Wuint height;
    /// buffer stride
    Wuint stride;
};

/// frame flags
///
/// Provides flags about the frame. This event is sent once before the
/// "ready" event.
struct zwlr_screencopy_frame_v1::event::flags {
    static constexpr Wopcode<zwlr_screencopy_frame_v1> opcode{ 1 };
    /// frame flags
    zwlr_screencopy_frame_v1::flags flags;
};

/// indicates frame is available for reading
///
/// Called as soon as the frame is copied, indicating it is available
/// for reading. This event includes the time at which presentation happened
/// at.
///
/// The timestamp is expressed as tv_sec_hi, tv_sec_lo, tv_nsec triples,
/// each component being an unsigned 32-bit value. Whole seconds are in
/// tv_sec which is a 64-bit value combined from tv_sec_hi and tv_sec_lo,
/// and the additional fractional part in tv_nsec as nanoseconds. Hence,
/// for valid timestamps tv_nsec must be in [0, 999999999]. The seconds part
/// may have an arbitrary offset at start.
///
/// After receiving this event, the client should destroy the object.
struct zwlr_screencopy_frame_v1::event::ready {
    static constexpr Wopcode<zwlr_screencopy_frame_v1> opcode{ 2 };
    /// high 32 bits of the seconds part of the timestamp
    Wuint tv_sec_hi;
    /// low 32 bits of the seconds part of the timestamp
    Wuint tv_sec_lo;
    /// nanoseconds part of the timestamp
    Wuint tv_nsec;
};

/// frame copy failed
///
/// This event indicates that the attempted frame copy has failed.
///
/// After receiving this event, the client should destroy the object.
struct zwlr_screencopy_frame_v1::event::failed {
    static constexpr Wopcode<zwlr_screencopy_frame_v1> opcode{ 3 };
};

/// carries the coordinates of the damaged region
///
/// This event is sent right before the ready event when copy_with_damage is
/// requested. It may be generated multiple times for each copy_with_damage
/// request.
///
/// The arguments describe a box around an area that has changed since the
/// last copy request that was derived from the current screencopy manager
/// instance.
///
/// The union of all regions received between the call to copy_with_damage
/// and a ready event is the total damage since the prior ready event.
struct zwlr_screencopy_frame_v1::event::damage {
    static constexpr Wopcode<zwlr_screencopy_frame_v1> opcode{ 4 };
    /// damaged x coordinates
    Wuint x;
    /// damaged y coordinates
    Wuint y;
    /// current width
    Wuint width;
    /// current height
    Wuint height;
};

/// linux-dmabuf buffer information
///
/// Provides information about linux-dmabuf buffer parameters that need to
/// be used for this frame. This event is sent once after the frame is
/// created if linux-dmabuf buffers are supported.
struct zwlr_screencopy_frame_v1::event::linux_dmabuf {
    static constexpr Wopcode<zwlr_screencopy_frame_v1> opcode{ 5 };
    /// fourcc pixel format
    Wuint format;
    /// buffer width
    Wuint width;
    /// buffer height
    Wuint height;
};

/// all buffer types reported
///
/// This event is sent once after all buffer events have been sent.
///
/// The client should proceed to create a buffer of one of the supported
/// types, and send a "copy" request.
struct zwlr_screencopy_frame_v1::event::buffer_done {
    static constexpr Wopcode<zwlr_screencopy_frame_v1> opcode{ 6 };
};

} // namespace protocols
} // namespace wl
} // namespace waylander
