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

#include <format>
#include <print>
#include <stdexcept>

#include "guilander/font.hpp"

namespace {
/// Singleton freetype2 library object.
auto
lib() -> FT_Library& {
    static auto initialized_lib = [] {
        auto       lib        = FT_Library{};
        const auto init_error = FT_Init_FreeType(&lib);
        if (init_error) { throw std::runtime_error{ "Failed to initalize freetype2 library." }; }
        return lib;
    }();
    return initialized_lib;
}

auto
load_font_face(const std::filesystem::path font_file) {
    auto       face = FT_Face{};
    const auto e    = FT_New_Face(lib(), font_file.c_str(), {}, &face);
    if (e == FT_Err_Unknown_File_Format) {
        throw std::runtime_error(
            std::format("FT_New_Face failed due to unkown file format in: {}", font_file.c_str()));
    } else if (e) {
        throw std::runtime_error(
            std::format("FT_New_Face failed due to unknown error: {}", font_file.c_str()));
    }
    return face;
}

} // namespace

namespace guilander::ft2 {

[[nodiscard]] font::font(const std::filesystem::path font_file)
    : face_{ load_font_face(font_file) } {
    if (not FT_IS_SCALABLE(face_)) { throw std::runtime_error{ "Loaded font is not scalable!" }; }
}

void
font::set_size(const guilander::ft2::pixel_uint width, const guilander::ft2::pixel_uint height) {
    const auto w = width.numerical_value_in(guilander::units::pixel);
    const auto h = height.numerical_value_in(guilander::units::pixel);
    if (FT_Set_Pixel_Sizes(face_, w, h)) {
        throw std::runtime_error("FT_Set_Pixel_Sizes failed due to unknown error.");
    }
}

const FT_GlyphSlot&
font::get_rendered_glyph(const FT_ULong charcode) {
    const auto glyph_index = FT_Get_Char_Index(face_, charcode);
    if (glyph_index == 0) { std::println("[warning] undefined character code: {}", charcode); }
    if (FT_Load_Glyph(face_, glyph_index, FT_LOAD_DEFAULT)) {
        throw std::runtime_error{ "FT_Load_Glyph failed due to unknown error." };
    }

    if (FT_Render_Glyph(face_->glyph, FT_RENDER_MODE_NORMAL)) {
        throw std::runtime_error{ "FT_Render_Glyph failed due to unknown error." };
    }
    return face_->glyph;
}

} // namespace guilander::ft2
