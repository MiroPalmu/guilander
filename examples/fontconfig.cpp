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

#include <cstdlib>
#include <filesystem>
#include <format>
#include <iostream>
#include <print>
#include <string>
#include <string_view>
#include <tuple>
#include <utility>
#include <vector>

#include "guilander/font.hpp"
#include "guilander/mdspan.hpp"
#include "guilander/mono_sort_set.hpp"
#include "guilander/static_picture.hpp"
#include "guilander/static_picture_window.hpp"

int
main(const int argc, const char** argv) {
    auto font_prop = guilander::fc::font_properties{};

    for (auto i = 1; i < argc; ++i) {
        const auto sv = std::string_view{ argv[i] };
        if (i == argc - 1) { std::println("Missing argument for: {}", sv); }
        const auto arg = std::string_view{ argv[++i] };

        if (sv == "--monospace") {
            font_prop.monospace = arg == "true";
        } else if (sv == "--scalable") {
            font_prop.scalable = arg == "true";
        } else if (sv == "--family-name") {
            font_prop.family_name = arg;
        } else if (sv == "--style") {
            font_prop.style = arg;
        } else {
            std::println("Unkown option: {}", sv);
        }
    }

    auto sort_set = guilander::mono_sort_set(font_prop);

    // Construct a static picture and fill it with black.
    auto picture = guilander::static_picture(800, 800);
    for (const auto pic_mds = picture.view_mdpixels();
         const auto [i, j] : guilander::sstd::mdindices(pic_mds)) {
        pic_mds[i, j] = guilander::static_picture::pixel{ { 255 }, { 255 }, { 255 }, { 255 } };
    }

    const auto blend = [](const std::uint8_t n) {
        return guilander::static_picture::pixel_blend_over{ 50, 120, 200, n };
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
