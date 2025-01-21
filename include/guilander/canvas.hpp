// Copyright (C) 2025 Miro Palmu.
//
// This file is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This file is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this file.  If not, see <https://www.gnu.org/licenses/>.

#pragma once

#include <algorithm>
#include <numeric>
#include <print>
#include <ranges>
#include <stdexcept>

#include "guilander/mdspan.hpp"

namespace guilander {

template<typename Canvas>
concept canvas = sstd::is_mdspan_v<Canvas> and (Canvas::extents_type::rank() == 2);

template<typename Canvas>
concept writable_canvas = canvas<Canvas>
                          and std::assignable_from<typename Canvas::accessor_type::reference,
                                                   typename Canvas::value_type>;

struct canvas_anchor {
    std::ptrdiff_t i, j;

    [[nodiscard]] friend constexpr bool
    operator==(const canvas_anchor, const canvas_anchor) = default;
};

/// Transform \x and \p from such that \p from becomes \p to and return transformed \p x.
[[nodiscard]] constexpr canvas_anchor
anchor_transform(const canvas_anchor x, const canvas_anchor from, const canvas_anchor to) {
    const auto relative_x = canvas_anchor{ x.i - from.i, x.j - from.j };
    return { to.i + relative_x.i, to.j + relative_x.j };
}

template<canvas Canvas>
class canvas_n_anchor {
    Canvas        canvas_;
    canvas_anchor anchor_;

  public:
    constexpr canvas_n_anchor(const Canvas c, const canvas_anchor a) : canvas_{ c }, anchor_{ a } {}

    [[nodiscard]] constexpr std::ptrdiff_t
    extent_right(this auto&& self) {
        return std::saturate_cast<std::ptrdiff_t>(self.canvas_.extent(1)) - self.anchor_.j - 1;
    }

    [[nodiscard]] constexpr std::ptrdiff_t
    extent_left(this auto&& self) {
        return std::saturate_cast<std::ptrdiff_t>(self.anchor_.j);
    }

    [[nodiscard]] constexpr std::ptrdiff_t
    extent_up(this auto&& self) {
        return std::saturate_cast<std::ptrdiff_t>(self.anchor_.i);
    }

    [[nodiscard]] constexpr std::ptrdiff_t
    extent_down(this auto&& self) {
        return std::saturate_cast<std::ptrdiff_t>(self.canvas_.extent(0)) - self.anchor_.i - 1;
    }

    [[nodiscard]] constexpr decltype(auto)
    get_anchor(this auto&& self) {
        return self.anchor_;
    }
    [[nodiscard]] constexpr decltype(auto)
    get_canvas(this auto&& self) {
        return self.canvas_;
    }

    template<canvas OtherCanvas>
    void
    stamp(this auto& self, const canvas_n_anchor<OtherCanvas>& other) {
        // In coordinates of other.
        const auto other_anchor = other.get_anchor();

        const auto other_hori_first = other_anchor.j - other.extent_left();
        const auto other_vert_first = other_anchor.i - other.extent_up();

        // Denotes the one past elements.
        const auto other_vert_end = other_anchor.i + other.extent_down() + 1;
        const auto other_hori_end = other_anchor.j + other.extent_right() + 1;

        const auto other_topleft             = canvas_anchor{ other_vert_first, other_hori_first };
        const auto other_onepast_bottomright = canvas_anchor{ other_vert_end, other_hori_end };

        // In our coordinates.
        const auto topleft = anchor_transform(other_topleft, other_anchor, self.anchor_);
        const auto onepast_bottomright =
            anchor_transform(other_onepast_bottomright, other_anchor, self.anchor_);

        const auto self_width  = std::saturate_cast<std::ptrdiff_t>(self.canvas_.extent(1));
        const auto self_height = std::saturate_cast<std::ptrdiff_t>(self.canvas_.extent(0));

        // Nothing to do if topleft is right or below self.canvas_.
        if (topleft.i >= self_height or topleft.j >= self_width) { return; }
        // Or bottomright is left or above self.canvas_.
        if (onepast_bottomright.i <= 0 or onepast_bottomright.j <= 0) { return; }

        namespace rn        = std::ranges;
        constexpr auto zero = std::ptrdiff_t{ 0 };
        const auto     clampped_topleft =
            canvas_anchor{ rn::max(zero, topleft.i), rn::max(zero, topleft.j) };
        const auto clampped_onepast_bottomright =
            canvas_anchor{ rn::min(self_height, onepast_bottomright.i),
                           rn::min(self_width, onepast_bottomright.j) };

        namespace rv             = std::views;
        const auto vert_indecies = rv::iota(clampped_topleft.i, clampped_onepast_bottomright.i);
        const auto hori_indecies = rv::iota(clampped_topleft.j, clampped_onepast_bottomright.j);
        for (const auto [i, j] : rv::cartesian_product(vert_indecies, hori_indecies)) {
            const auto ij          = canvas_anchor{ i, j };
            const auto ij_in_other = anchor_transform(ij, self.anchor_, other_anchor);
            self.canvas_[i, j]     = other.get_canvas()[ij_in_other.i, ij_in_other.j];
        }
    }
};

} // namespace guilander
