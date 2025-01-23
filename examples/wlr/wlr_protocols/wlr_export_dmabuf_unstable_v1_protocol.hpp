// Generated from Wayland xml protocol: wlr_export_dmabuf_unstable_v1

/// @file
/// a protocol for low overhead screen content capturing
///
/// An interface to capture surfaces in an efficient way by exporting DMA-BUFs.
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
/// Copyright © 2018 Rostislav Pehlivanov
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

struct zwlr_export_dmabuf_manager_v1 {
    struct request {
        struct capture_output;
        struct destroy;
    };

    struct event {
    };
};
struct zwlr_export_dmabuf_frame_v1 {
    enum class flags : Wint::integral_type;
    enum class cancel_reason : Wint::integral_type;

    struct request {
        struct destroy;
    };

    struct event {
        struct frame;
        struct object;
        struct ready;
        struct cancel;
    };
};
/// capture a frame from an output
///
/// Capture the next frame of an entire output.
struct zwlr_export_dmabuf_manager_v1::request::capture_output {
    static constexpr Wopcode<zwlr_export_dmabuf_manager_v1> opcode{ 0 };
    Wnew_id<zwlr_export_dmabuf_frame_v1> frame;
    /// include custom client hardware cursor on top of the frame
    Wint overlay_cursor;
    Wobject<wl_output> output;
};

/// destroy the manager
///
/// All objects created by the manager will still remain valid, until their
/// appropriate destroy request has been called.
struct zwlr_export_dmabuf_manager_v1::request::destroy {
    static constexpr Wopcode<zwlr_export_dmabuf_manager_v1> opcode{ 1 };
};

/// frame flags
///
/// Special flags that should be respected by the client.
enum class zwlr_export_dmabuf_frame_v1::flags : Wint::integral_type {
    /// clients should copy frame before processing
    Etransient = 1
};

/// cancel reason
///
/// Indicates reason for cancelling the frame.
enum class zwlr_export_dmabuf_frame_v1::cancel_reason : Wint::integral_type {
    /// temporary error, source will produce more frames
    Etemporary = 0,
    /// fatal error, source will not produce frames
    Epermanent = 1,
    /// temporary error, source will produce more frames
    Eresizing = 2
};

/// delete this object, used or not
///
/// Unreferences the frame. This request must be called as soon as its no
/// longer used.
///
/// It can be called at any time by the client. The client will still have
/// to close any FDs it has been given.
struct zwlr_export_dmabuf_frame_v1::request::destroy {
    static constexpr Wopcode<zwlr_export_dmabuf_frame_v1> opcode{ 0 };
};

/// a frame description
///
/// Main event supplying the client with information about the frame. If the
/// capture didn't fail, this event is always emitted first before any other
/// events.
///
/// This event is followed by a number of "object" as specified by the
/// "num_objects" argument.
struct zwlr_export_dmabuf_frame_v1::event::frame {
    static constexpr Wopcode<zwlr_export_dmabuf_frame_v1> opcode{ 0 };
    /// frame width in pixels
    Wuint width;
    /// frame height in pixels
    Wuint height;
    /// crop offset for the x axis
    Wuint offset_x;
    /// crop offset for the y axis
    Wuint offset_y;
    /// flags which indicate properties (invert, interlacing),                     has the same values as zwp_linux_buffer_params_v1:flags
    Wuint buffer_flags;
    /// indicates special frame features
    zwlr_export_dmabuf_frame_v1::flags flags;
    /// format of the frame (DRM_FORMAT_*)
    Wuint format;
    /// drm format modifier, high
    Wuint mod_high;
    /// drm format modifier, low
    Wuint mod_low;
    /// indicates how many objects (FDs) the frame has (max 4)
    Wuint num_objects;
};

/// an object description
///
/// Event which serves to supply the client with the file descriptors
/// containing the data for each object.
///
/// After receiving this event, the client must always close the file
/// descriptor as soon as they're done with it and even if the frame fails.
struct zwlr_export_dmabuf_frame_v1::event::object {
    static constexpr Wopcode<zwlr_export_dmabuf_frame_v1> opcode{ 1 };
    /// index of the current object
    Wuint index;
    /// fd of the current object
    Wfd fd;
    /// size in bytes for the current object
    Wuint size;
    /// starting point for the data in the object's fd
    Wuint offset;
    /// line size in bytes
    Wuint stride;
    /// index of the plane the data in the object applies to
    Wuint plane_index;
};

/// indicates frame is available for reading
///
/// This event is sent as soon as the frame is presented, indicating it is
/// available for reading. This event includes the time at which
/// presentation happened at.
///
/// The timestamp is expressed as tv_sec_hi, tv_sec_lo, tv_nsec triples,
/// each component being an unsigned 32-bit value. Whole seconds are in
/// tv_sec which is a 64-bit value combined from tv_sec_hi and tv_sec_lo,
/// and the additional fractional part in tv_nsec as nanoseconds. Hence,
/// for valid timestamps tv_nsec must be in [0, 999999999]. The seconds part
/// may have an arbitrary offset at start.
///
/// After receiving this event, the client should destroy this object.
struct zwlr_export_dmabuf_frame_v1::event::ready {
    static constexpr Wopcode<zwlr_export_dmabuf_frame_v1> opcode{ 2 };
    /// high 32 bits of the seconds part of the timestamp
    Wuint tv_sec_hi;
    /// low 32 bits of the seconds part of the timestamp
    Wuint tv_sec_lo;
    /// nanoseconds part of the timestamp
    Wuint tv_nsec;
};

/// indicates the frame is no longer valid
///
/// If the capture failed or if the frame is no longer valid after the
/// "frame" event has been emitted, this event will be used to inform the
/// client to scrap the frame.
///
/// If the failure is temporary, the client may capture again the same
/// source. If the failure is permanent, any further attempts to capture the
/// same source will fail again.
///
/// After receiving this event, the client should destroy this object.
struct zwlr_export_dmabuf_frame_v1::event::cancel {
    static constexpr Wopcode<zwlr_export_dmabuf_frame_v1> opcode{ 3 };
    /// indicates a reason for cancelling this frame capture
    zwlr_export_dmabuf_frame_v1::cancel_reason reason;
};

} // namespace protocols
} // namespace wl
} // namespace waylander
