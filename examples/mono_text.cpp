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

#include <codecvt>
#include <cstdlib>
#include <filesystem>
#include <format>
#include <iostream>
#include <print>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include "guilander/font.hpp"
#include "guilander/mono_sort_set.hpp"
#include "guilander/static_picture.hpp"
#include "guilander/static_picture_window.hpp"

int
main() {
    // Get font path from SWAYLANDER_FONT_PATH (default: Comic Mono).
    constexpr auto comic_mono_path = "/usr/share/fonts/TTF/ComicMono.ttf";
    const auto     font_path_env   = std::getenv("GUILANDER_FONT_PATH");
    const auto     font_path       = std::string{ font_path_env ? font_path_env : comic_mono_path };

    // Set text size with sort_set.
    auto sort_set = guilander::mono_sort_set(font_path);

    // Construct a static picture and fill it with black.
    auto picture = guilander::static_picture(800, 800);
    for (const auto [coords, pixel] : picture.view_pixels()) {
        pixel = guilander::static_picture::pixel{ { 255 }, { 255 }, { 255 }, { 255 } };
    }

    const auto blend = [](const std::uint8_t n) {
        return guilander::static_picture::pixel_blend_over{ 100, 120, 100, n };
    };

    auto h_tot = 0;
    for (int x = 1; x < 14; x += 1) {
        const auto w = 10;
        const auto h = x * w;
        h_tot += h + 5;
        sort_set.set_size(w * guilander::units::pixel, h * guilander::units::pixel);
        const auto text_cna = guilander::canvas_n_anchor{ picture.view_mdpixels(), { h_tot, 30 } };

        const auto text   = std::format("width, height = {}, {}", w, h);
        const auto u8text = std::u8string{ reinterpret_cast<const char8_t*>(text.data()) };

        for (const auto cna : sort_set.as_sorts(u8text)) { text_cna.stamp(cna, blend); }
    }

    h_tot = 0;
    for (int x = 1; x < 100; x += 5) {
        const auto h = 10;
        const auto w = x + h;
        h_tot += h + 5;
        sort_set.set_size(w * guilander::units::pixel, h * guilander::units::pixel);
        const auto text_cna = guilander::canvas_n_anchor{ picture.view_mdpixels(), { h_tot, 240 } };

        const auto text   = std::format("width, height = {}, {}", w, h);
        const auto u8text = std::u8string{ reinterpret_cast<const char8_t*>(text.data()) };

        for (const auto cna : sort_set.as_sorts(u8text)) { text_cna.stamp(cna, blend); }
    }


    h_tot = 250;
    for (int w = 20; w < 500; w *= 2) {
        const auto h = w;
        h_tot += h + 5;
        sort_set.set_size(w * guilander::units::pixel, h * guilander::units::pixel);
        const auto text_cna = guilander::canvas_n_anchor{ picture.view_mdpixels(), { h_tot, 240 } };

        const auto text   = std::format("width, height = {}, {}", w, h);
        const auto u8text = std::u8string{ reinterpret_cast<const char8_t*>(text.data()) };

        for (const auto cna : sort_set.as_sorts(u8text)) { text_cna.stamp(cna, blend); }
    }

    auto window = guilander::static_picture_window{ std::move(picture) };
    window.wait_for_close_event();
}
