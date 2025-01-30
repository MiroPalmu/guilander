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

#include <chrono>
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
#include "guilander/interactive_window.hpp"
#include "guilander/mdspan.hpp"
#include "guilander/mono_sort_set.hpp"
#include "guilander/static_picture.hpp"

int
main(const int argc, char** argv) {
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
    sort_set.set_size(30 * guilander::units::pixel, 30 * guilander::units::pixel);

    // Construct a static picture and fill it with white.
    auto picture = guilander::static_picture(800, 200);

    auto paint_white = [](const auto mds) {
        for (const auto [i, j] : guilander::sstd::mdindices(mds)) {
            mds[i, j] = guilander::static_picture::pixel{ { 255 }, { 255 }, { 255 }, { 255 } };
        }
    };

    paint_white(picture.view_mdpixels());
    auto window = guilander::interactive_window{ std::move(picture) };

    const auto blend = [](const std::uint8_t n) {
        return guilander::static_picture::pixel_blend_over{ 50, 120, 200, n };
    };

    const auto text = std::u8string{ u8"5 4 3 2 1... launch!" };

    auto time = std::chrono::steady_clock::now();

    for (auto frame = 0uz; true; ++frame) {
        const auto frame_mds = window.damage_all();
        paint_white(frame_mds);

        const auto text_cna     = guilander::canvas_n_anchor{ frame_mds, { 50, 5 } };
        const auto partial_text = std::u8string_view{ text.data(), (frame / 30uz) % text.size() };

        for (const auto cna : sort_set.as_sorts(partial_text)) { text_cna.stamp(cna, blend); }

        const auto new_time = std::chrono::steady_clock::now();
        const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(new_time - time);
        time               = new_time;

        const auto time_cna = guilander::canvas_n_anchor{ frame_mds, { 150, 5 } };
        using namespace std::literals;
        const auto time_text = std::format("Frame time: {} or {:4.1f}fps", elapsed, 1.0s / elapsed);
        const auto time_u8text =
            std::u8string_view{ reinterpret_cast<char8_t const*>(time_text.data()) };

        for (const auto cna : sort_set.as_sorts(time_u8text)) { time_cna.stamp(cna, blend); }

        window.commit();
        window.frame_throttle();
    }
}
