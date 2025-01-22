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

#include <boost/ut.hpp> // import boost.ut;

#include <cstdlib>
#include <filesystem>
#include <string>
#include <utility>

#include "guilander/font.hpp"

int
main() {
    using namespace boost::ut;

    cfg<override> = { .tag = { "font" } };

    tag("font") / "ft2::font can be constructed from ttf font"_test = [] {
        const auto test_font_path = std::getenv("GUILANDER_TEST_TTF_FONT_PATH");
        expect(test_font_path) << "GUILANDER_TEST_TTF_FONT_PATH not set!";
        std::ignore = guilander::ft2::font{ test_font_path };
        expect(true);
    };
}
