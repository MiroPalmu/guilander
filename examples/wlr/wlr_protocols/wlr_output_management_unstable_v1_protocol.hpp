// Generated from Wayland xml protocol: wlr_output_management_unstable_v1

/// @file
/// protocol to configure output devices
///
/// This protocol exposes interfaces to obtain and modify output device
/// configuration.
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

struct zwlr_output_manager_v1 {
    struct request {
        struct create_configuration;
        struct stop;
    };

    struct event {
        struct head;
        struct done;
        struct finished;
    };
};
struct zwlr_output_head_v1 {
    enum class adaptive_sync_state : Wint::integral_type;

    struct request {
        struct release;
    };

    struct event {
        struct name;
        struct description;
        struct physical_size;
        struct mode;
        struct enabled;
        struct current_mode;
        struct position;
        struct transform;
        struct scale;
        struct finished;
        struct make;
        struct model;
        struct serial_number;
        struct adaptive_sync;
    };
};
struct zwlr_output_mode_v1 {
    struct request {
        struct release;
    };

    struct event {
        struct size;
        struct refresh;
        struct preferred;
        struct finished;
    };
};
struct zwlr_output_configuration_v1 {
    enum class error : Wint::integral_type;

    struct request {
        struct enable_head;
        struct disable_head;
        struct apply;
        struct test;
        struct destroy;
    };

    struct event {
        struct succeeded;
        struct failed;
        struct cancelled;
    };
};
struct zwlr_output_configuration_head_v1 {
    enum class error : Wint::integral_type;

    struct request {
        struct set_mode;
        struct set_custom_mode;
        struct set_position;
        struct set_transform;
        struct set_scale;
        struct set_adaptive_sync;
    };

    struct event {
    };
};
/// create a new output configuration object
///
/// Create a new output configuration object. This allows to update head
/// properties.
struct zwlr_output_manager_v1::request::create_configuration {
    static constexpr Wopcode<zwlr_output_manager_v1> opcode{ 0 };
    Wnew_id<zwlr_output_configuration_v1> id;
    Wuint serial;
};

/// stop sending events
///
/// Indicates the client no longer wishes to receive events for output
/// configuration changes. However the compositor may emit further events,
/// until the finished event is emitted.
///
/// The client must not send any more requests after this one.
struct zwlr_output_manager_v1::request::stop {
    static constexpr Wopcode<zwlr_output_manager_v1> opcode{ 1 };
};

/// introduce a new head
///
/// This event introduces a new head. This happens whenever a new head
/// appears (e.g. a monitor is plugged in) or after the output manager is
/// bound.
struct zwlr_output_manager_v1::event::head {
    static constexpr Wopcode<zwlr_output_manager_v1> opcode{ 0 };
    Wnew_id<zwlr_output_head_v1> head;
};

/// sent all information about current configuration
///
/// This event is sent after all information has been sent after binding to
/// the output manager object and after any subsequent changes. This applies
/// to child head and mode objects as well. In other words, this event is
/// sent whenever a head or mode is created or destroyed and whenever one of
/// their properties has been changed. Not all state is re-sent each time
/// the current configuration changes: only the actual changes are sent.
///
/// This allows changes to the output configuration to be seen as atomic,
/// even if they happen via multiple events.
///
/// A serial is sent to be used in a future create_configuration request.
struct zwlr_output_manager_v1::event::done {
    static constexpr Wopcode<zwlr_output_manager_v1> opcode{ 1 };
    /// current configuration serial
    Wuint serial;
};

/// the compositor has finished with the manager
///
/// This event indicates that the compositor is done sending manager events.
/// The compositor will destroy the object immediately after sending this
/// event, so it will become invalid and the client should release any
/// resources associated with it.
struct zwlr_output_manager_v1::event::finished {
    static constexpr Wopcode<zwlr_output_manager_v1> opcode{ 2 };
};

enum class zwlr_output_head_v1::adaptive_sync_state : Wint::integral_type {
    /// adaptive sync is disabled
    Edisabled = 0,
    /// adaptive sync is enabled
    Eenabled = 1
};

/// destroy the head object
///
/// This request indicates that the client will no longer use this head
/// object.
struct zwlr_output_head_v1::request::release {
    static constexpr Wopcode<zwlr_output_head_v1> opcode{ 0 };
};

/// head name
///
/// This event describes the head name.
///
/// The naming convention is compositor defined, but limited to alphanumeric
/// characters and dashes (-). Each name is unique among all wlr_output_head
/// objects, but if a wlr_output_head object is destroyed the same name may
/// be reused later. The names will also remain consistent across sessions
/// with the same hardware and software configuration.
///
/// Examples of names include 'HDMI-A-1', 'WL-1', 'X11-1', etc. However, do
/// not assume that the name is a reflection of an underlying DRM
/// connector, X11 connection, etc.
///
/// If the compositor implements the xdg-output protocol and this head is
/// enabled, the xdg_output.name event must report the same name.
///
/// The name event is sent after a wlr_output_head object is created. This
/// event is only sent once per object, and the name does not change over
/// the lifetime of the wlr_output_head object.
struct zwlr_output_head_v1::event::name {
    static constexpr Wopcode<zwlr_output_head_v1> opcode{ 0 };
    Wstring name;
};

/// head description
///
/// This event describes a human-readable description of the head.
///
/// The description is a UTF-8 string with no convention defined for its
/// contents. Examples might include 'Foocorp 11" Display' or 'Virtual X11
/// output via :1'. However, do not assume that the name is a reflection of
/// the make, model, serial of the underlying DRM connector or the display
/// name of the underlying X11 connection, etc.
///
/// If the compositor implements xdg-output and this head is enabled,
/// the xdg_output.description must report the same description.
///
/// The description event is sent after a wlr_output_head object is created.
/// This event is only sent once per object, and the description does not
/// change over the lifetime of the wlr_output_head object.
struct zwlr_output_head_v1::event::description {
    static constexpr Wopcode<zwlr_output_head_v1> opcode{ 1 };
    Wstring description;
};

/// head physical size
///
/// This event describes the physical size of the head. This event is only
/// sent if the head has a physical size (e.g. is not a projector or a
/// virtual device).
///
/// The physical size event is sent after a wlr_output_head object is created. This
/// event is only sent once per object, and the physical size does not change over
/// the lifetime of the wlr_output_head object.
struct zwlr_output_head_v1::event::physical_size {
    static constexpr Wopcode<zwlr_output_head_v1> opcode{ 2 };
    /// width in millimeters of the output
    Wint width;
    /// height in millimeters of the output
    Wint height;
};

/// introduce a mode
///
/// This event introduces a mode for this head. It is sent once per
/// supported mode.
struct zwlr_output_head_v1::event::mode {
    static constexpr Wopcode<zwlr_output_head_v1> opcode{ 3 };
    Wnew_id<zwlr_output_mode_v1> mode;
};

/// head is enabled or disabled
///
/// This event describes whether the head is enabled. A disabled head is not
/// mapped to a region of the global compositor space.
///
/// When a head is disabled, some properties (current_mode, position,
/// transform and scale) are irrelevant.
struct zwlr_output_head_v1::event::enabled {
    static constexpr Wopcode<zwlr_output_head_v1> opcode{ 4 };
    /// zero if disabled, non-zero if enabled
    Wint enabled;
};

/// current mode
///
/// This event describes the mode currently in use for this head. It is only
/// sent if the output is enabled.
struct zwlr_output_head_v1::event::current_mode {
    static constexpr Wopcode<zwlr_output_head_v1> opcode{ 5 };
    Wobject<zwlr_output_mode_v1> mode;
};

/// current position
///
/// This events describes the position of the head in the global compositor
/// space. It is only sent if the output is enabled.
struct zwlr_output_head_v1::event::position {
    static constexpr Wopcode<zwlr_output_head_v1> opcode{ 6 };
    /// x position within the global compositor space
    Wint x;
    /// y position within the global compositor space
    Wint y;
};

/// current transformation
///
/// This event describes the transformation currently applied to the head.
/// It is only sent if the output is enabled.
struct zwlr_output_head_v1::event::transform {
    static constexpr Wopcode<zwlr_output_head_v1> opcode{ 7 };
    wl_output::transform transform;
};

/// current scale
///
/// This events describes the scale of the head in the global compositor
/// space. It is only sent if the output is enabled.
struct zwlr_output_head_v1::event::scale {
    static constexpr Wopcode<zwlr_output_head_v1> opcode{ 8 };
    Wfixed scale;
};

/// the head has disappeared
///
/// This event indicates that the head is no longer available. The head
/// object becomes inert. Clients should send a destroy request and release
/// any resources associated with it.
struct zwlr_output_head_v1::event::finished {
    static constexpr Wopcode<zwlr_output_head_v1> opcode{ 9 };
};

/// head manufacturer
///
/// This event describes the manufacturer of the head.
///
/// This must report the same make as the wl_output interface does in its
/// geometry event.
///
/// Together with the model and serial_number events the purpose is to
/// allow clients to recognize heads from previous sessions and for example
/// load head-specific configurations back.
///
/// It is not guaranteed this event will be ever sent. A reason for that
/// can be that the compositor does not have information about the make of
/// the head or the definition of a make is not sensible in the current
/// setup, for example in a virtual session. Clients can still try to
/// identify the head by available information from other events but should
/// be aware that there is an increased risk of false positives.
///
/// If sent, the make event is sent after a wlr_output_head object is
/// created and only sent once per object. The make does not change over
/// the lifetime of the wlr_output_head object.
///
/// It is not recommended to display the make string in UI to users. For
/// that the string provided by the description event should be preferred.
struct zwlr_output_head_v1::event::make {
    static constexpr Wopcode<zwlr_output_head_v1> opcode{ 10 };
    Wstring make;
};

/// head model
///
/// This event describes the model of the head.
///
/// This must report the same model as the wl_output interface does in its
/// geometry event.
///
/// Together with the make and serial_number events the purpose is to
/// allow clients to recognize heads from previous sessions and for example
/// load head-specific configurations back.
///
/// It is not guaranteed this event will be ever sent. A reason for that
/// can be that the compositor does not have information about the model of
/// the head or the definition of a model is not sensible in the current
/// setup, for example in a virtual session. Clients can still try to
/// identify the head by available information from other events but should
/// be aware that there is an increased risk of false positives.
///
/// If sent, the model event is sent after a wlr_output_head object is
/// created and only sent once per object. The model does not change over
/// the lifetime of the wlr_output_head object.
///
/// It is not recommended to display the model string in UI to users. For
/// that the string provided by the description event should be preferred.
struct zwlr_output_head_v1::event::model {
    static constexpr Wopcode<zwlr_output_head_v1> opcode{ 11 };
    Wstring model;
};

/// head serial number
///
/// This event describes the serial number of the head.
///
/// Together with the make and model events the purpose is to allow clients
/// to recognize heads from previous sessions and for example load head-
/// specific configurations back.
///
/// It is not guaranteed this event will be ever sent. A reason for that
/// can be that the compositor does not have information about the serial
/// number of the head or the definition of a serial number is not sensible
/// in the current setup. Clients can still try to identify the head by
/// available information from other events but should be aware that there
/// is an increased risk of false positives.
///
/// If sent, the serial number event is sent after a wlr_output_head object
/// is created and only sent once per object. The serial number does not
/// change over the lifetime of the wlr_output_head object.
///
/// It is not recommended to display the serial_number string in UI to
/// users. For that the string provided by the description event should be
/// preferred.
struct zwlr_output_head_v1::event::serial_number {
    static constexpr Wopcode<zwlr_output_head_v1> opcode{ 12 };
    Wstring serial_number;
};

/// current adaptive sync state
///
/// This event describes whether adaptive sync is currently enabled for
/// the head or not. Adaptive sync is also known as Variable Refresh
/// Rate or VRR.
struct zwlr_output_head_v1::event::adaptive_sync {
    static constexpr Wopcode<zwlr_output_head_v1> opcode{ 13 };
    zwlr_output_head_v1::adaptive_sync_state state;
};

/// destroy the mode object
///
/// This request indicates that the client will no longer use this mode
/// object.
struct zwlr_output_mode_v1::request::release {
    static constexpr Wopcode<zwlr_output_mode_v1> opcode{ 0 };
};

/// mode size
///
/// This event describes the mode size. The size is given in physical
/// hardware units of the output device. This is not necessarily the same as
/// the output size in the global compositor space. For instance, the output
/// may be scaled or transformed.
struct zwlr_output_mode_v1::event::size {
    static constexpr Wopcode<zwlr_output_mode_v1> opcode{ 0 };
    /// width of the mode in hardware units
    Wint width;
    /// height of the mode in hardware units
    Wint height;
};

/// mode refresh rate
///
/// This event describes the mode's fixed vertical refresh rate. It is only
/// sent if the mode has a fixed refresh rate.
struct zwlr_output_mode_v1::event::refresh {
    static constexpr Wopcode<zwlr_output_mode_v1> opcode{ 1 };
    /// vertical refresh rate in mHz
    Wint refresh;
};

/// mode is preferred
///
/// This event advertises this mode as preferred.
struct zwlr_output_mode_v1::event::preferred {
    static constexpr Wopcode<zwlr_output_mode_v1> opcode{ 2 };
};

/// the mode has disappeared
///
/// This event indicates that the mode is no longer available. The mode
/// object becomes inert. Clients should send a destroy request and release
/// any resources associated with it.
struct zwlr_output_mode_v1::event::finished {
    static constexpr Wopcode<zwlr_output_mode_v1> opcode{ 3 };
};

enum class zwlr_output_configuration_v1::error : Wint::integral_type {
    /// head has been configured twice
    Ealready_configured_head = 1,
    /// head has not been configured
    Eunconfigured_head = 2,
    /// request sent after configuration has been applied or tested
    Ealready_used = 3
};

/// enable and configure a head
///
/// Enable a head. This request creates a head configuration object that can
/// be used to change the head's properties.
struct zwlr_output_configuration_v1::request::enable_head {
    static constexpr Wopcode<zwlr_output_configuration_v1> opcode{ 0 };
    /// a new object to configure the head
    Wnew_id<zwlr_output_configuration_head_v1> id;
    /// the head to be enabled
    Wobject<zwlr_output_head_v1> head;
};

/// disable a head
///
/// Disable a head.
struct zwlr_output_configuration_v1::request::disable_head {
    static constexpr Wopcode<zwlr_output_configuration_v1> opcode{ 1 };
    /// the head to be disabled
    Wobject<zwlr_output_head_v1> head;
};

/// apply the configuration
///
/// Apply the new output configuration.
///
/// In case the configuration is successfully applied, there is no guarantee
/// that the new output state matches completely the requested
/// configuration. For instance, a compositor might round the scale if it
/// doesn't support fractional scaling.
///
/// After this request has been sent, the compositor must respond with an
/// succeeded, failed or cancelled event. Sending a request that isn't the
/// destructor is a protocol error.
struct zwlr_output_configuration_v1::request::apply {
    static constexpr Wopcode<zwlr_output_configuration_v1> opcode{ 2 };
};

/// test the configuration
///
/// Test the new output configuration. The configuration won't be applied,
/// but will only be validated.
///
/// Even if the compositor succeeds to test a configuration, applying it may
/// fail.
///
/// After this request has been sent, the compositor must respond with an
/// succeeded, failed or cancelled event. Sending a request that isn't the
/// destructor is a protocol error.
struct zwlr_output_configuration_v1::request::test {
    static constexpr Wopcode<zwlr_output_configuration_v1> opcode{ 3 };
};

/// destroy the output configuration
///
/// Using this request a client can tell the compositor that it is not going
/// to use the configuration object anymore. Any changes to the outputs
/// that have not been applied will be discarded.
///
/// This request also destroys wlr_output_configuration_head objects created
/// via this object.
struct zwlr_output_configuration_v1::request::destroy {
    static constexpr Wopcode<zwlr_output_configuration_v1> opcode{ 4 };
};

/// configuration changes succeeded
///
/// Sent after the compositor has successfully applied the changes or
/// tested them.
///
/// Upon receiving this event, the client should destroy this object.
///
/// If the current configuration has changed, events to describe the changes
/// will be sent followed by a wlr_output_manager.done event.
struct zwlr_output_configuration_v1::event::succeeded {
    static constexpr Wopcode<zwlr_output_configuration_v1> opcode{ 0 };
};

/// configuration changes failed
///
/// Sent if the compositor rejects the changes or failed to apply them. The
/// compositor should revert any changes made by the apply request that
/// triggered this event.
///
/// Upon receiving this event, the client should destroy this object.
struct zwlr_output_configuration_v1::event::failed {
    static constexpr Wopcode<zwlr_output_configuration_v1> opcode{ 1 };
};

/// configuration has been cancelled
///
/// Sent if the compositor cancels the configuration because the state of an
/// output changed and the client has outdated information (e.g. after an
/// output has been hotplugged).
///
/// The client can create a new configuration with a newer serial and try
/// again.
///
/// Upon receiving this event, the client should destroy this object.
struct zwlr_output_configuration_v1::event::cancelled {
    static constexpr Wopcode<zwlr_output_configuration_v1> opcode{ 2 };
};

enum class zwlr_output_configuration_head_v1::error : Wint::integral_type {
    /// property has already been set
    Ealready_set = 1,
    /// mode doesn't belong to head
    Einvalid_mode = 2,
    /// mode is invalid
    Einvalid_custom_mode = 3,
    /// transform value outside enum
    Einvalid_transform = 4,
    /// scale negative or zero
    Einvalid_scale = 5,
    /// invalid enum value used in the set_adaptive_sync request
    Einvalid_adaptive_sync_state = 6
};

/// set the mode
///
/// This request sets the head's mode.
struct zwlr_output_configuration_head_v1::request::set_mode {
    static constexpr Wopcode<zwlr_output_configuration_head_v1> opcode{ 0 };
    Wobject<zwlr_output_mode_v1> mode;
};

/// set a custom mode
///
/// This request assigns a custom mode to the head. The size is given in
/// physical hardware units of the output device. If set to zero, the
/// refresh rate is unspecified.
///
/// It is a protocol error to set both a mode and a custom mode.
struct zwlr_output_configuration_head_v1::request::set_custom_mode {
    static constexpr Wopcode<zwlr_output_configuration_head_v1> opcode{ 1 };
    /// width of the mode in hardware units
    Wint width;
    /// height of the mode in hardware units
    Wint height;
    /// vertical refresh rate in mHz or zero
    Wint refresh;
};

/// set the position
///
/// This request sets the head's position in the global compositor space.
struct zwlr_output_configuration_head_v1::request::set_position {
    static constexpr Wopcode<zwlr_output_configuration_head_v1> opcode{ 2 };
    /// x position in the global compositor space
    Wint x;
    /// y position in the global compositor space
    Wint y;
};

/// set the transform
///
/// This request sets the head's transform.
struct zwlr_output_configuration_head_v1::request::set_transform {
    static constexpr Wopcode<zwlr_output_configuration_head_v1> opcode{ 3 };
    wl_output::transform transform;
};

/// set the scale
///
/// This request sets the head's scale.
struct zwlr_output_configuration_head_v1::request::set_scale {
    static constexpr Wopcode<zwlr_output_configuration_head_v1> opcode{ 4 };
    Wfixed scale;
};

/// enable/disable adaptive sync
///
/// This request enables/disables adaptive sync. Adaptive sync is also
/// known as Variable Refresh Rate or VRR.
struct zwlr_output_configuration_head_v1::request::set_adaptive_sync {
    static constexpr Wopcode<zwlr_output_configuration_head_v1> opcode{ 5 };
    zwlr_output_head_v1::adaptive_sync_state state;
};

} // namespace protocols
} // namespace wl
} // namespace waylander
