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

#include <cstdint>
#include <cstdlib>
#include <memory>
#include <ranges>
#include <stdexcept>

#include <unistr.h>

#include "guilander/mono_sort_set.hpp"

namespace guilander {

[[nodiscard]] mono_sort_set::mono_sort_set(const std::filesystem::path font_path)
    : font_{ ft2::font{ font_path } } {}

[[nodiscard]] mono_sort_set::mono_sort_set(const fc::font_properties properties)
    : font_{ ft2::font{ properties } } {}

[[nodiscard]] auto
mono_sort_set::as_sorts(const std::u8string_view str) -> std::generator<const sort_type&> {
    std::size_t str32_size;
    const auto  str32_ptr = std::unique_ptr<char32_t, decltype(&std::free)>(
        reinterpret_cast<char32_t*>(::u8_to_u32(reinterpret_cast<const std::uint8_t*>(str.data()),
                                                str.size(),
                                                nullptr,
                                                &str32_size)),
        &std::free);

    if (str32_ptr == NULL) {
        throw std::runtime_error(
            "GNU libunistring: conversion from UTF-8 to UTF-32 failed (::u8_to_u32).");
    }

    auto glyph_buffer                   = std::vector<std::uint8_t>{};
    auto accumulated_horizontal_advance = std::ptrdiff_t{ 0 } * units::pixel65536;

    // Use classic iteration to work aroud gcc ICE:
    // https://gcc.gnu.org/bugzilla/show_bug.cgi?id=118491
    //
    // for (const auto& x : std::u32string_view(str32_ptr.get(), str32_size)) {
    const auto u32_sv = std::u32string_view(str32_ptr.get(), str32_size);
    for (auto x_it = u32_sv.begin(); x_it != u32_sv.end(); ++x_it) {
        auto& x = *x_it;

        font_.set_size(sort_width_, sort_height_);
        const auto& glyph = font_.get_rendered_glyph(x);
        if (glyph->format != FT_GLYPH_FORMAT_BITMAP) {
            throw std::logic_error{
                "See note on FT_BitmapGlyphRec:"
                "https://freetype.org/freetype2/docs/reference/ft2-glyph_management.html#ft_glyphrec"
            };
        }
        const auto glyph_width = glyph->bitmap.width;
        const auto glyph_rows  = glyph->bitmap.rows;
        const auto pitch       = glyph->bitmap.pitch;

        glyph_buffer.resize(glyph_width * glyph_rows);
        const auto glyph_buffer_mdspan = canvas_type(glyph_buffer.data(), glyph_rows, glyph_width);

        for (const auto [i, j] : sstd::mdindices(glyph_buffer_mdspan)) {
            const auto bitmap_offset  = i * pitch + j;
            glyph_buffer_mdspan[i, j] = *std::ranges::next(glyph->bitmap.buffer, bitmap_offset);
        }

        const auto hori_bearing = std::ptrdiff_t{ glyph->bitmap_left } * units::pixel;
        const auto hori_anchor  = hori_bearing + accumulated_horizontal_advance;

        const auto s =
            sort_type{ glyph_buffer_mdspan,
                       canvas_anchor{ glyph->bitmap_top,
                                      -hori_anchor.force_numerical_value_in(units::pixel) } };
        co_yield s;

        const auto current_horizontal_advance = std::ptrdiff_t{ glyph->advance.x } * units::pixel64;
        accumulated_horizontal_advance += current_horizontal_advance;
    }
}

void
mono_sort_set::set_size(const ft2::pixel_uint width, const ft2::pixel_uint height) {
    this->sort_width_  = width;
    this->sort_height_ = height;
}

} // namespace guilander
