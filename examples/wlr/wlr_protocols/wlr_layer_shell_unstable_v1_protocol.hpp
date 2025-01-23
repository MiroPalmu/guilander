// Generated from Wayland xml protocol: wlr_layer_shell_unstable_v1

/// @file
/// wlr_layer_shell_unstable_v1 xml protocol
///
/// Copyright © 2017 Drew DeVault
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

struct zwlr_layer_shell_v1 {
    enum class error : Wint::integral_type;
    enum class layer : Wint::integral_type;

    struct request {
        struct get_layer_surface;
        struct destroy;
    };

    struct event {
    };
};
struct zwlr_layer_surface_v1 {
    enum class keyboard_interactivity : Wint::integral_type;
    enum class error : Wint::integral_type;
    enum class anchor : Wuint::integral_type;

    struct request {
        struct set_size;
        struct set_anchor;
        struct set_exclusive_zone;
        struct set_margin;
        struct set_keyboard_interactivity;
        struct get_popup;
        struct ack_configure;
        struct destroy;
        struct set_layer;
        struct set_exclusive_edge;
    };

    struct event {
        struct configure;
        struct closed;
    };
};
enum class zwlr_layer_shell_v1::error : Wint::integral_type {
    /// wl_surface has another role
    Erole = 0,
    /// layer value is invalid
    Einvalid_layer = 1,
    /// wl_surface has a buffer attached or committed
    Ealready_constructed = 2
};

/// available layers for surfaces
///
/// These values indicate which layers a surface can be rendered in. They
/// are ordered by z depth, bottom-most first. Traditional shell surfaces
/// will typically be rendered between the bottom and top layers.
/// Fullscreen shell surfaces are typically rendered at the top layer.
/// Multiple surfaces can share a single layer, and ordering within a
/// single layer is undefined.
enum class zwlr_layer_shell_v1::layer : Wint::integral_type {
    Ebackground = 0,
    Ebottom = 1,
    Etop = 2,
    Eoverlay = 3
};

/// create a layer_surface from a surface
///
/// Create a layer surface for an existing surface. This assigns the role of
/// layer_surface, or raises a protocol error if another role is already
/// assigned.
///
/// Creating a layer surface from a wl_surface which has a buffer attached
/// or committed is a client error, and any attempts by a client to attach
/// or manipulate a buffer prior to the first layer_surface.configure call
/// must also be treated as errors.
///
/// After creating a layer_surface object and setting it up, the client
/// must perform an initial commit without any buffer attached.
/// The compositor will reply with a layer_surface.configure event.
/// The client must acknowledge it and is then allowed to attach a buffer
/// to map the surface.
///
/// You may pass NULL for output to allow the compositor to decide which
/// output to use. Generally this will be the one that the user most
/// recently interacted with.
///
/// Clients can specify a namespace that defines the purpose of the layer
/// surface.
struct zwlr_layer_shell_v1::request::get_layer_surface {
    static constexpr Wopcode<zwlr_layer_shell_v1> opcode{ 0 };
    Wnew_id<zwlr_layer_surface_v1> id;
    Wobject<wl_surface> surface;
    Wobject<wl_output> output;
    /// layer to add this surface to
    zwlr_layer_shell_v1::layer layer;
    /// namespace for the layer surface
    Wstring namespace_;
};

/// destroy the layer_shell object
///
/// This request indicates that the client will not use the layer_shell
/// object any more. Objects that have been created through this instance
/// are not affected.
struct zwlr_layer_shell_v1::request::destroy {
    static constexpr Wopcode<zwlr_layer_shell_v1> opcode{ 1 };
};

/// types of keyboard interaction possible for a layer shell surface
///
/// Types of keyboard interaction possible for layer shell surfaces. The
/// rationale for this is twofold: (1) some applications are not interested
/// in keyboard events and not allowing them to be focused can improve the
/// desktop experience; (2) some applications will want to take exclusive
/// keyboard focus.
enum class zwlr_layer_surface_v1::keyboard_interactivity : Wint::integral_type {
    /// no keyboard focus is possible
    ///
    /// This value indicates that this surface is not interested in keyboard
    /// events and the compositor should never assign it the keyboard focus.
    ///
    /// This is the default value, set for newly created layer shell surfaces.
    ///
    /// This is useful for e.g. desktop widgets that display information or
    /// only have interaction with non-keyboard input devices.

    Enone = 0,
    /// request exclusive keyboard focus
    ///
    /// Request exclusive keyboard focus if this surface is above the shell surface layer.
    ///
    /// For the top and overlay layers, the seat will always give
    /// exclusive keyboard focus to the top-most layer which has keyboard
    /// interactivity set to exclusive. If this layer contains multiple
    /// surfaces with keyboard interactivity set to exclusive, the compositor
    /// determines the one receiving keyboard events in an implementation-
    /// defined manner. In this case, no guarantee is made when this surface
    /// will receive keyboard focus (if ever).
    ///
    /// For the bottom and background layers, the compositor is allowed to use
    /// normal focus semantics.
    ///
    /// This setting is mainly intended for applications that need to ensure
    /// they receive all keyboard events, such as a lock screen or a password
    /// prompt.

    Eexclusive = 1,
    /// request regular keyboard focus semantics
    ///
    /// This requests the compositor to allow this surface to be focused and
    /// unfocused by the user in an implementation-defined manner. The user
    /// should be able to unfocus this surface even regardless of the layer
    /// it is on.
    ///
    /// Typically, the compositor will want to use its normal mechanism to
    /// manage keyboard focus between layer shell surfaces with this setting
    /// and regular toplevels on the desktop layer (e.g. click to focus).
    /// Nevertheless, it is possible for a compositor to require a special
    /// interaction to focus or unfocus layer shell surfaces (e.g. requiring
    /// a click even if focus follows the mouse normally, or providing a
    /// keybinding to switch focus between layers).
    ///
    /// This setting is mainly intended for desktop shell components (e.g.
    /// panels) that allow keyboard interaction. Using this option can allow
    /// implementing a desktop shell that can be fully usable without the
    /// mouse.

    Eon_demand = 2
};

enum class zwlr_layer_surface_v1::error : Wint::integral_type {
    /// provided surface state is invalid
    Einvalid_surface_state = 0,
    /// size is invalid
    Einvalid_size = 1,
    /// anchor bitfield is invalid
    Einvalid_anchor = 2,
    /// keyboard interactivity is invalid
    Einvalid_keyboard_interactivity = 3,
    /// exclusive edge is invalid given the surface anchors
    Einvalid_exclusive_edge = 4
};

enum class zwlr_layer_surface_v1::anchor : Wuint::integral_type {
    /// the top edge of the anchor rectangle
    Etop = 1,
    /// the bottom edge of the anchor rectangle
    Ebottom = 2,
    /// the left edge of the anchor rectangle
    Eleft = 4,
    /// the right edge of the anchor rectangle
    Eright = 8
};

/// sets the size of the surface
///
/// Sets the size of the surface in surface-local coordinates. The
/// compositor will display the surface centered with respect to its
/// anchors.
///
/// If you pass 0 for either value, the compositor will assign it and
/// inform you of the assignment in the configure event. You must set your
/// anchor to opposite edges in the dimensions you omit; not doing so is a
/// protocol error. Both values are 0 by default.
///
/// Size is double-buffered, see wl_surface.commit.
struct zwlr_layer_surface_v1::request::set_size {
    static constexpr Wopcode<zwlr_layer_surface_v1> opcode{ 0 };
    Wuint width;
    Wuint height;
};

/// configures the anchor point of the surface
///
/// Requests that the compositor anchor the surface to the specified edges
/// and corners. If two orthogonal edges are specified (e.g. 'top' and
/// 'left'), then the anchor point will be the intersection of the edges
/// (e.g. the top left corner of the output); otherwise the anchor point
/// will be centered on that edge, or in the center if none is specified.
///
/// Anchor is double-buffered, see wl_surface.commit.
struct zwlr_layer_surface_v1::request::set_anchor {
    static constexpr Wopcode<zwlr_layer_surface_v1> opcode{ 1 };
    zwlr_layer_surface_v1::anchor anchor;
};

/// configures the exclusive geometry of this surface
///
/// Requests that the compositor avoids occluding an area with other
/// surfaces. The compositor's use of this information is
/// implementation-dependent - do not assume that this region will not
/// actually be occluded.
///
/// A positive value is only meaningful if the surface is anchored to one
/// edge or an edge and both perpendicular edges. If the surface is not
/// anchored, anchored to only two perpendicular edges (a corner), anchored
/// to only two parallel edges or anchored to all edges, a positive value
/// will be treated the same as zero.
///
/// A positive zone is the distance from the edge in surface-local
/// coordinates to consider exclusive.
///
/// Surfaces that do not wish to have an exclusive zone may instead specify
/// how they should interact with surfaces that do. If set to zero, the
/// surface indicates that it would like to be moved to avoid occluding
/// surfaces with a positive exclusive zone. If set to -1, the surface
/// indicates that it would not like to be moved to accommodate for other
/// surfaces, and the compositor should extend it all the way to the edges
/// it is anchored to.
///
/// For example, a panel might set its exclusive zone to 10, so that
/// maximized shell surfaces are not shown on top of it. A notification
/// might set its exclusive zone to 0, so that it is moved to avoid
/// occluding the panel, but shell surfaces are shown underneath it. A
/// wallpaper or lock screen might set their exclusive zone to -1, so that
/// they stretch below or over the panel.
///
/// The default value is 0.
///
/// Exclusive zone is double-buffered, see wl_surface.commit.
struct zwlr_layer_surface_v1::request::set_exclusive_zone {
    static constexpr Wopcode<zwlr_layer_surface_v1> opcode{ 2 };
    Wint zone;
};

/// sets a margin from the anchor point
///
/// Requests that the surface be placed some distance away from the anchor
/// point on the output, in surface-local coordinates. Setting this value
/// for edges you are not anchored to has no effect.
///
/// The exclusive zone includes the margin.
///
/// Margin is double-buffered, see wl_surface.commit.
struct zwlr_layer_surface_v1::request::set_margin {
    static constexpr Wopcode<zwlr_layer_surface_v1> opcode{ 3 };
    Wint top;
    Wint right;
    Wint bottom;
    Wint left;
};

/// requests keyboard events
///
/// Set how keyboard events are delivered to this surface. By default,
/// layer shell surfaces do not receive keyboard events; this request can
/// be used to change this.
///
/// This setting is inherited by child surfaces set by the get_popup
/// request.
///
/// Layer surfaces receive pointer, touch, and tablet events normally. If
/// you do not want to receive them, set the input region on your surface
/// to an empty region.
///
/// Keyboard interactivity is double-buffered, see wl_surface.commit.
struct zwlr_layer_surface_v1::request::set_keyboard_interactivity {
    static constexpr Wopcode<zwlr_layer_surface_v1> opcode{ 4 };
    zwlr_layer_surface_v1::keyboard_interactivity keyboard_interactivity;
};

/// assign this layer_surface as an xdg_popup parent
///
/// This assigns an xdg_popup's parent to this layer_surface.  This popup
/// should have been created via xdg_surface::get_popup with the parent set
/// to NULL, and this request must be invoked before committing the popup's
/// initial state.
///
/// See the documentation of xdg_popup for more details about what an
/// xdg_popup is and how it is used.
struct zwlr_layer_surface_v1::request::get_popup {
    static constexpr Wopcode<zwlr_layer_surface_v1> opcode{ 5 };
    Wobject<xdg_popup> popup;
};

/// ack a configure event
///
/// When a configure event is received, if a client commits the
/// surface in response to the configure event, then the client
/// must make an ack_configure request sometime before the commit
/// request, passing along the serial of the configure event.
///
/// If the client receives multiple configure events before it
/// can respond to one, it only has to ack the last configure event.
///
/// A client is not required to commit immediately after sending
/// an ack_configure request - it may even ack_configure several times
/// before its next surface commit.
///
/// A client may send multiple ack_configure requests before committing, but
/// only the last request sent before a commit indicates which configure
/// event the client really is responding to.
struct zwlr_layer_surface_v1::request::ack_configure {
    static constexpr Wopcode<zwlr_layer_surface_v1> opcode{ 6 };
    /// the serial from the configure event
    Wuint serial;
};

/// destroy the layer_surface
///
/// This request destroys the layer surface.
struct zwlr_layer_surface_v1::request::destroy {
    static constexpr Wopcode<zwlr_layer_surface_v1> opcode{ 7 };
};

/// change the layer of the surface
///
/// Change the layer that the surface is rendered on.
///
/// Layer is double-buffered, see wl_surface.commit.
struct zwlr_layer_surface_v1::request::set_layer {
    static constexpr Wopcode<zwlr_layer_surface_v1> opcode{ 8 };
    /// layer to move this surface to
    zwlr_layer_shell_v1::layer layer;
};

/// set the edge the exclusive zone will be applied to
///
/// Requests an edge for the exclusive zone to apply. The exclusive
/// edge will be automatically deduced from anchor points when possible,
/// but when the surface is anchored to a corner, it will be necessary
/// to set it explicitly to disambiguate, as it is not possible to deduce
/// which one of the two corner edges should be used.
///
/// The edge must be one the surface is anchored to, otherwise the
/// invalid_exclusive_edge protocol error will be raised.
struct zwlr_layer_surface_v1::request::set_exclusive_edge {
    static constexpr Wopcode<zwlr_layer_surface_v1> opcode{ 9 };
    zwlr_layer_surface_v1::anchor edge;
};

/// suggest a surface change
///
/// The configure event asks the client to resize its surface.
///
/// Clients should arrange their surface for the new states, and then send
/// an ack_configure request with the serial sent in this configure event at
/// some point before committing the new surface.
///
/// The client is free to dismiss all but the last configure event it
/// received.
///
/// The width and height arguments specify the size of the window in
/// surface-local coordinates.
///
/// The size is a hint, in the sense that the client is free to ignore it if
/// it doesn't resize, pick a smaller size (to satisfy aspect ratio or
/// resize in steps of NxM pixels). If the client picks a smaller size and
/// is anchored to two opposite anchors (e.g. 'top' and 'bottom'), the
/// surface will be centered on this axis.
///
/// If the width or height arguments are zero, it means the client should
/// decide its own window dimension.
struct zwlr_layer_surface_v1::event::configure {
    static constexpr Wopcode<zwlr_layer_surface_v1> opcode{ 0 };
    Wuint serial;
    Wuint width;
    Wuint height;
};

/// surface should be closed
///
/// The closed event is sent by the compositor when the surface will no
/// longer be shown. The output may have been destroyed or the user may
/// have asked for it to be removed. Further changes to the surface will be
/// ignored. The client should destroy the resource after receiving this
/// event, and create a new surface if they so choose.
struct zwlr_layer_surface_v1::event::closed {
    static constexpr Wopcode<zwlr_layer_surface_v1> opcode{ 1 };
};

} // namespace protocols
} // namespace wl
} // namespace waylander
