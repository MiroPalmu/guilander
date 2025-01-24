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

#include <generator>
#include <string_view>

#include "guilander/canvas.hpp"
#include "guilander/font.hpp"
#include "guilander/mdspan.hpp"

namespace guilander {

/// Sort set with fixed width rendered using FT_RENDER_MODE_NORMAL.
class mono_sort_set {
    ft2::font       font_;
    ft2::pixel_uint sort_width_{ 40u * units::pixel };
    ft2::pixel_uint sort_height_{ 60u * units::pixel };

  public:
    [[nodiscard]] explicit mono_sort_set(const std::filesystem::path font_file);
    [[nodiscard]] explicit mono_sort_set(const fc::font_properties);

    ft2::font_metrics
    set_size(const ft2::pixel_uint width, const ft2::pixel_uint height);

    using canvas_type = stdex::mdspan<std::uint8_t, stdex::dextents<std::size_t, 2>>;
    using sort_type   = canvas_n_anchor<canvas_type>;

    [[nodiscard]] auto
    as_sorts(const std::u8string_view) -> std::generator<const sort_type&>;
};

} // namespace guilander
