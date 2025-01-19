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
};

template<canvas Canvas>
struct canvas_n_anchor {
    Canvas        canvas;
    canvas_anchor anchor;

    [[nodiscard]] constexpr std::ptrdiff_t
    extent_right(this auto&& self) {
        return std::saturate_cast<std::ptrdiff_t>(self.canvas.extent(1)) - self.anchor.j - 1;
    }

    [[nodiscard]] constexpr std::ptrdiff_t
    extent_left(this auto&& self) {
        return std::saturate_cast<std::ptrdiff_t>(self.anchor.j);
    }

    [[nodiscard]] constexpr std::ptrdiff_t
    extent_up(this auto&& self) {
        return std::saturate_cast<std::ptrdiff_t>(self.anchor.i);
    }

    [[nodiscard]] constexpr std::ptrdiff_t
    extent_down(this auto&& self) {
        return std::saturate_cast<std::ptrdiff_t>(self.canvas.extent(0)) - self.anchor.i - 1;
    }
};

} // namespace guilander
