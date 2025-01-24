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

#include <utility>
#include <vector>

#include "guilander/mdspan.hpp"
#include "guilander/static_picture.hpp"
#include "guilander/static_picture_window.hpp"

int
main() {
    auto windows = std::vector<guilander::static_picture_window>{};
    for (auto i = 0uz; i < 3uz; ++i) {
        auto picture = guilander::static_picture(50, 50);

        for (const auto pic_mds = picture.view_mdpixels();
             const auto [i, j] : guilander::sstd::cartesian_iota(pic_mds)) {
            pic_mds[i, j] =
                guilander::static_picture::pixel{ { static_cast<std::uint8_t>(10 * j % 255) },
                                                  { static_cast<std::uint8_t>(20 * i % 255) },
                                                  {},
                                                  { 255 } };
        }

        windows.emplace_back(std::move(picture));
    }
    for (auto& w : windows) { w.wait_for_close_event(); }
}
