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

#include "guilander/static_picture.hpp"

#include "static_picture_overlay.hpp"

int
main() {
    auto picture = guilander::static_picture(50, 50);

    for (const auto [coords, pixel] : picture.view_pixels()) {
        pixel =
            guilander::static_picture::pixel{ { static_cast<std::uint8_t>(10 * coords.x % 255) },
                                              { static_cast<std::uint8_t>(20 * coords.y % 255) },
                                              {},
                                              { 255 } };
    }

    auto overlay = guilander::static_picture_overlay(std::move(picture));
    overlay.wait_for_close_event();
}
