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

#include <bit>
#include <cstdint>
#include <cstdlib>
#include <ranges>
#include <tuple>
#include <type_traits>

#include "mdspan.hpp"

#include "gnulander/memory_block.hpp"
#include "waylander/wayland/protocols/wayland_protocol.hpp"

namespace guilander {

struct static_picture {
    int                      width, height;
    gnulander::memory_block  buff;
    gnulander::mapped_memory bytes;

    [[nodiscard]] static_picture(const int width_arg, const int height_arg);

    static constexpr auto color_format = waylander::wl::protocols::wl_shm::format::Eargb8888;

    struct pixel_blend_over;

    struct pixel {
        std::uint8_t blue{}, green{}, red{}, alpha{};

        constexpr std::tuple<float, float, float, float>
        as_floats() const {
            return { blue / 255., green / 255., red / 255., alpha / 255. };
        };

        static constexpr pixel
        from_floats(const float b, const float g, const float r, const float a) {
            return pixel{ static_cast<std::uint8_t>(255 * b),
                          static_cast<std::uint8_t>(255 * g),
                          static_cast<std::uint8_t>(255 * r),
                          static_cast<std::uint8_t>(255 * a) };
        }

        constexpr pixel&
        operator=(const pixel_blend_over A) {
            const auto [Ab, Ag, Ar, Aa] = A.as_floats();
            const auto [Bb, Bg, Br, Ba] = this->as_floats();

            const auto new_alpha = Aa + Ba * (1 - Aa);
            const auto new_blue  = (Ab * Aa + Bb * Ba * (1 - Aa)) / new_alpha;
            const auto new_green = (Ag * Aa + Bg * Ba * (1 - Aa)) / new_alpha;
            const auto new_red   = (Ar * Aa + Br * Ba * (1 - Aa)) / new_alpha;

            return *this = pixel::from_floats(new_blue, new_green, new_red, new_alpha);
        }

        constexpr pixel(const std::uint8_t b,
                        const std::uint8_t g,
                        const std::uint8_t r,
                        const std::uint8_t a)
            : blue{ b },
              green{ g },
              red{ r },
              alpha{ a } {}
        static_assert(std::endian::native == std::endian::little,
                      "Only little edian platforms are supported atm.");
        static constexpr pixel
        debug_color() {
            return pixel{ 0, 0, 255, 255 };
        };
    };

    struct pixel_blend_over : public pixel {
        using pixel::pixel;
    };

    struct pixel_coords {
        int x, y;
    };

    using pixel_view_type = std::tuple<pixel_coords, pixel&>;

    [[nodiscard]] std::ranges::view auto
    view_pixels() {
        namespace rv = std::ranges::views;

#ifdef __cpp_lib_is_implicit_lifetime
        static_assert(std::is_implicit_lifetime_v<pixel>);
#endif
        static_assert(std::alignment_of_v<pixel> == std::alignment_of_v<std::byte>);
        static_assert(sizeof(pixel) == sizeof(std::uint32_t));

        return bytes | rv::chunk(4) | rv::enumerate | rv::transform([&](auto index_bgra_tuple) {
                   const auto index       = static_cast<int>(std::get<0>(index_bgra_tuple));
                   const auto x           = index % width;
                   const auto y           = index / width;
                   auto       pixel_begin = std::get<1>(index_bgra_tuple).begin();
                   return pixel_view_type{ { x, y }, reinterpret_cast<pixel&>(*pixel_begin) };
               });
    }

    using mdpixels = stdex::mdspan<pixel, stdex::dextents<std::size_t, 2>>;

    [[nodiscard]] auto
    view_mdpixels() -> mdpixels {
        return mdpixels(reinterpret_cast<pixel*>(bytes.data()), height, width);
    }
};

} // namespace guilander
