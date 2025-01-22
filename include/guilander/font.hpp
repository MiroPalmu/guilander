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

#include <concepts>
#include <filesystem>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <mp-units/framework.h>
#include <mp-units/systems/typographic.h>

#include "units.hpp"

namespace guilander {
namespace ft2 {

using pixel_uint = mp_units::quantity<units::pixel, FT_UInt>;

class font {
    FT_Face face_;

  public:
    [[nodiscard]] explicit font(const std::filesystem::path font_file);

    void
    set_size(const pixel_uint width, const pixel_uint height);

    const FT_GlyphSlot&
    get_rendered_glyph(const FT_ULong charcode);
};

} // namespace ft2
} // namespace guilander
