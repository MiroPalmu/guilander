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

#include "guilander/canvas.hpp"
#include "guilander/interactive_window.hpp"
#include "guilander/mdspan.hpp"
#include "guilander/static_picture.hpp"
#include "guilander/units.hpp"

int
main() {
    auto initial_picture     = guilander::static_picture(400, 400);
    auto initial_picture_mds = initial_picture.view_mdpixels();

    for (const auto [i, j] : guilander::sstd::mdindices(initial_picture_mds)) {
        initial_picture_mds[i, j] =
            guilander::static_picture::pixel{ static_cast<std::uint8_t>(10 * j % 255),
                                              static_cast<std::uint8_t>(20 * i % 255),
                                              {},
                                              255 };
    }

    auto window = guilander::interactive_window{ std::move(initial_picture) };

    std::uint8_t frame{};
    while (true) {
        const auto dmg_mds =
            window.damage({ .x = 30u + frame, .y = 20u + frame, .width = 40u, .height = 60u });

        for (const auto [i, j] : guilander::sstd::mdindices(dmg_mds)) {
            dmg_mds[i, j] =
                guilander::static_picture::pixel{ static_cast<std::uint8_t>(10 * j % 255),
                                                  static_cast<std::uint8_t>(20 * i % 255),
                                                  static_cast<std::uint8_t>(frame),
                                                  255 };
        }
        ++frame;

        window.commit();
        window.frame_throttle();
    }
}
