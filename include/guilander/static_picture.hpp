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
    struct pixel {
        std::uint8_t blue{}, green{}, red{}, alpha{ 1 };
        pixel(const std::uint8_t g) : green{ g } {}
        pixel(const std::uint8_t b,
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
        return mdpixels(reinterpret_cast<pixel*>(bytes.data()), width, height);
    }
};

} // namespace guilander
