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

#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <numeric>
#include <print>
#include <ranges>
#include <sstream>
#include <string>
#include <string_view>
#include <thread>
#include <utility>
#include <vector>

#include <uniconv.h>
#include <uniwidth.h>

#include "guilander/font.hpp"
#include "guilander/mono_sort_set.hpp"
#include "guilander/static_picture.hpp"
#include "guilander/static_picture_window.hpp"

#include "static_picture_overlay.hpp"

void
err() {
    std::println("Invalid arguments!");
    std::println("usage: wlr_text_overlay <font_width> <font_height> <time_visible_in_ms> <path>");
    std::exit(1);
}

int
main(const int argc, const char** argv) {
    if (argc != 5) { err(); }

    auto args = std::stringstream{};
    args << argv[1] << " " << argv[2] << " " << argv[3];

    std::size_t w, h, t;
    args >> w >> h >> t;

    // Set text size with sort_set.
    auto sort_set = guilander::mono_sort_set(guilander::fc::font_properties{ .monospace = true });
    const auto metrics =
        sort_set.set_size(w * guilander::units::pixel, h * guilander::units::pixel);

    auto text_stream = std::ifstream(argv[4], std::ios::binary);
    auto lines       = std::vector<std::string>{};
    for (std::string line; std::getline(text_stream, line);) { lines.push_back(line); }

    if (lines.empty()) return 1;

    const auto max_width =
        5 * guilander::units::pixel
        + metrics.max_advance
              * std::ranges::max(lines | std::views::transform([](const std::string& str) {
                                     return ::u8_strwidth(
                                         reinterpret_cast<const uint8_t*>(str.c_str()),
                                         ::locale_charset());
                                 }));

    const auto total_height = metrics.height_between_baselines * lines.size();

    auto picture =
        guilander::static_picture(max_width.force_numerical_value_in(guilander::units::pixel),
                                  total_height.force_numerical_value_in(guilander::units::pixel));

    for (const auto pic_mds = picture.view_mdpixels();
         const auto [i, j] : guilander::sstd::mdindices(pic_mds)) {
        pic_mds[i, j] = guilander::static_picture::pixel{ { 50 }, { 50 }, { 50 }, { 250 } };
    }

    const auto blend = [](const std::uint8_t n) {
        return guilander::static_picture::pixel_blend_over{ 255, 255, 255, n };
    };

    auto base_line = 3 * metrics.height_between_baselines / 4;
    for (const auto& line : lines) {
        const auto u8line = std::u8string{ reinterpret_cast<const char8_t*>(line.data()) };
        for (const auto cna : sort_set.as_sorts(u8line)) {
            guilander::canvas_n_anchor{
                picture.view_mdpixels(),
                { base_line.force_numerical_value_in(guilander::units::pixel), 5 }
            }
                .stamp(cna, blend);
        }
        base_line += metrics.height_between_baselines;
    }

    auto _ = guilander::static_picture_overlay(std::move(picture));
    std::this_thread::sleep_for(std::chrono::milliseconds(t));
}
