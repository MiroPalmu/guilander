// Generated from Wayland xml protocol: wlr_data_control_unstable_v1

/// @file
/// control data devices
///
/// This protocol allows a privileged client to control data devices. In
/// particular, the client will be able to manage the current selection and take
/// the role of a clipboard manager.
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
/// Copyright © 2019 Ivan Molodetskikh
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

struct zwlr_data_control_manager_v1 {
    struct request {
        struct create_data_source;
        struct get_data_device;
        struct destroy;
    };

    struct event {
    };
};
struct zwlr_data_control_device_v1 {
    enum class error : Wint::integral_type;

    struct request {
        struct set_selection;
        struct destroy;
        struct set_primary_selection;
    };

    struct event {
        struct data_offer;
        struct selection;
        struct finished;
        struct primary_selection;
    };
};
struct zwlr_data_control_source_v1 {
    enum class error : Wint::integral_type;

    struct request {
        struct offer;
        struct destroy;
    };

    struct event {
        struct send;
        struct cancelled;
    };
};
struct zwlr_data_control_offer_v1 {
    struct request {
        struct receive;
        struct destroy;
    };

    struct event {
        struct offer;
    };
};
/// create a new data source
///
/// Create a new data source.
struct zwlr_data_control_manager_v1::request::create_data_source {
    static constexpr Wopcode<zwlr_data_control_manager_v1> opcode{ 0 };
    /// data source to create
    Wnew_id<zwlr_data_control_source_v1> id;
};

/// get a data device for a seat
///
/// Create a data device that can be used to manage a seat's selection.
struct zwlr_data_control_manager_v1::request::get_data_device {
    static constexpr Wopcode<zwlr_data_control_manager_v1> opcode{ 1 };
    Wnew_id<zwlr_data_control_device_v1> id;
    Wobject<wl_seat> seat;
};

/// destroy the manager
///
/// All objects created by the manager will still remain valid, until their
/// appropriate destroy request has been called.
struct zwlr_data_control_manager_v1::request::destroy {
    static constexpr Wopcode<zwlr_data_control_manager_v1> opcode{ 2 };
};

enum class zwlr_data_control_device_v1::error : Wint::integral_type {
    /// source given to set_selection or set_primary_selection was already used before
    Eused_source = 1
};

/// copy data to the selection
///
/// This request asks the compositor to set the selection to the data from
/// the source on behalf of the client.
///
/// The given source may not be used in any further set_selection or
/// set_primary_selection requests. Attempting to use a previously used
/// source is a protocol error.
///
/// To unset the selection, set the source to NULL.
struct zwlr_data_control_device_v1::request::set_selection {
    static constexpr Wopcode<zwlr_data_control_device_v1> opcode{ 0 };
    Wobject<zwlr_data_control_source_v1> source;
};

/// destroy this data device
///
/// Destroys the data device object.
struct zwlr_data_control_device_v1::request::destroy {
    static constexpr Wopcode<zwlr_data_control_device_v1> opcode{ 1 };
};

/// copy data to the primary selection
///
/// This request asks the compositor to set the primary selection to the
/// data from the source on behalf of the client.
///
/// The given source may not be used in any further set_selection or
/// set_primary_selection requests. Attempting to use a previously used
/// source is a protocol error.
///
/// To unset the primary selection, set the source to NULL.
///
/// The compositor will ignore this request if it does not support primary
/// selection.
struct zwlr_data_control_device_v1::request::set_primary_selection {
    static constexpr Wopcode<zwlr_data_control_device_v1> opcode{ 2 };
    Wobject<zwlr_data_control_source_v1> source;
};

/// introduce a new wlr_data_control_offer
///
/// The data_offer event introduces a new wlr_data_control_offer object,
/// which will subsequently be used in either the
/// wlr_data_control_device.selection event (for the regular clipboard
/// selections) or the wlr_data_control_device.primary_selection event (for
/// the primary clipboard selections). Immediately following the
/// wlr_data_control_device.data_offer event, the new data_offer object
/// will send out wlr_data_control_offer.offer events to describe the MIME
/// types it offers.
struct zwlr_data_control_device_v1::event::data_offer {
    static constexpr Wopcode<zwlr_data_control_device_v1> opcode{ 0 };
    Wnew_id<zwlr_data_control_offer_v1> id;
};

/// advertise new selection
///
/// The selection event is sent out to notify the client of a new
/// wlr_data_control_offer for the selection for this device. The
/// wlr_data_control_device.data_offer and the wlr_data_control_offer.offer
/// events are sent out immediately before this event to introduce the data
/// offer object. The selection event is sent to a client when a new
/// selection is set. The wlr_data_control_offer is valid until a new
/// wlr_data_control_offer or NULL is received. The client must destroy the
/// previous selection wlr_data_control_offer, if any, upon receiving this
/// event.
///
/// The first selection event is sent upon binding the
/// wlr_data_control_device object.
struct zwlr_data_control_device_v1::event::selection {
    static constexpr Wopcode<zwlr_data_control_device_v1> opcode{ 1 };
    Wobject<zwlr_data_control_offer_v1> id;
};

/// this data control is no longer valid
///
/// This data control object is no longer valid and should be destroyed by
/// the client.
struct zwlr_data_control_device_v1::event::finished {
    static constexpr Wopcode<zwlr_data_control_device_v1> opcode{ 2 };
};

/// advertise new primary selection
///
/// The primary_selection event is sent out to notify the client of a new
/// wlr_data_control_offer for the primary selection for this device. The
/// wlr_data_control_device.data_offer and the wlr_data_control_offer.offer
/// events are sent out immediately before this event to introduce the data
/// offer object. The primary_selection event is sent to a client when a
/// new primary selection is set. The wlr_data_control_offer is valid until
/// a new wlr_data_control_offer or NULL is received. The client must
/// destroy the previous primary selection wlr_data_control_offer, if any,
/// upon receiving this event.
///
/// If the compositor supports primary selection, the first
/// primary_selection event is sent upon binding the
/// wlr_data_control_device object.
struct zwlr_data_control_device_v1::event::primary_selection {
    static constexpr Wopcode<zwlr_data_control_device_v1> opcode{ 3 };
    Wobject<zwlr_data_control_offer_v1> id;
};

enum class zwlr_data_control_source_v1::error : Wint::integral_type {
    /// offer sent after wlr_data_control_device.set_selection
    Einvalid_offer = 1
};

/// add an offered MIME type
///
/// This request adds a MIME type to the set of MIME types advertised to
/// targets. Can be called several times to offer multiple types.
///
/// Calling this after wlr_data_control_device.set_selection is a protocol
/// error.
struct zwlr_data_control_source_v1::request::offer {
    static constexpr Wopcode<zwlr_data_control_source_v1> opcode{ 0 };
    /// MIME type offered by the data source
    Wstring mime_type;
};

/// destroy this source
///
/// Destroys the data source object.
struct zwlr_data_control_source_v1::request::destroy {
    static constexpr Wopcode<zwlr_data_control_source_v1> opcode{ 1 };
};

/// send the data
///
/// Request for data from the client. Send the data as the specified MIME
/// type over the passed file descriptor, then close it.
struct zwlr_data_control_source_v1::event::send {
    static constexpr Wopcode<zwlr_data_control_source_v1> opcode{ 0 };
    /// MIME type for the data
    Wstring mime_type;
    /// file descriptor for the data
    Wfd fd;
};

/// selection was cancelled
///
/// This data source is no longer valid. The data source has been replaced
/// by another data source.
///
/// The client should clean up and destroy this data source.
struct zwlr_data_control_source_v1::event::cancelled {
    static constexpr Wopcode<zwlr_data_control_source_v1> opcode{ 1 };
};

/// request that the data is transferred
///
/// To transfer the offered data, the client issues this request and
/// indicates the MIME type it wants to receive. The transfer happens
/// through the passed file descriptor (typically created with the pipe
/// system call). The source client writes the data in the MIME type
/// representation requested and then closes the file descriptor.
///
/// The receiving client reads from the read end of the pipe until EOF and
/// then closes its end, at which point the transfer is complete.
///
/// This request may happen multiple times for different MIME types.
struct zwlr_data_control_offer_v1::request::receive {
    static constexpr Wopcode<zwlr_data_control_offer_v1> opcode{ 0 };
    /// MIME type desired by receiver
    Wstring mime_type;
    /// file descriptor for data transfer
    Wfd fd;
};

/// destroy this offer
///
/// Destroys the data offer object.
struct zwlr_data_control_offer_v1::request::destroy {
    static constexpr Wopcode<zwlr_data_control_offer_v1> opcode{ 1 };
};

/// advertise offered MIME type
///
/// Sent immediately after creating the wlr_data_control_offer object.
/// One event per offered MIME type.
struct zwlr_data_control_offer_v1::event::offer {
    static constexpr Wopcode<zwlr_data_control_offer_v1> opcode{ 0 };
    /// offered MIME type
    Wstring mime_type;
};

} // namespace protocols
} // namespace wl
} // namespace waylander
