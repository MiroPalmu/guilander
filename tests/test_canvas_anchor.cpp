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

#include "guilander/canvas.hpp"
#include "guilander/mdspan.hpp"

int
main() {
    using namespace boost::ut;

    cfg<override> = { .tag = { "canvas" } };

    tag("canvas") / "canvas_n_anchor::extent_* are positive for interior anchors"_test = [] {
        constexpr int buff[]{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
        const auto    c   = stdex::mdspan(buff, 3, 4);
        const auto    a   = guilander::canvas_anchor{ 1, 2 };
        const auto    cna = guilander::canvas_n_anchor{ c, a };

        expect(cna.extent_up() == 1);
        expect(cna.extent_down() == 1);
        expect(cna.extent_right() == 1);
        expect(cna.extent_left() == 2);
    };

    tag("canvas") / "canvas_n_anchor::extent_* are zero at the edge of interior"_test = [] {
        constexpr int buff[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
        const auto    c      = stdex::mdspan(buff, 3, 4);
        const auto    a1     = guilander::canvas_anchor{ 0, 0 };
        const auto    a2     = guilander::canvas_anchor{ 0, 1 };
        const auto    cna1   = guilander::canvas_n_anchor{ c, a1 };
        const auto    cna2   = guilander::canvas_n_anchor{ c, a2 };

        expect(cna1.extent_up() == 0);
        expect(cna1.extent_down() == 2);
        expect(cna1.extent_right() == 3);
        expect(cna1.extent_left() == 0);

        expect(cna2.extent_up() == 0);
        expect(cna2.extent_down() == 2);
        expect(cna2.extent_right() == 2);
        expect(cna2.extent_left() == 1);
    };

    tag("canvas") / "canvas_n_anchor::extent_* are negative in exterior"_test = [] {
        constexpr int buff[]{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
        const auto    c    = stdex::mdspan(buff, 3, 4);
        const auto    a1   = guilander::canvas_anchor{ -1, -1 };
        const auto    a2   = guilander::canvas_anchor{ -1, 0 };
        const auto    a3   = guilander::canvas_anchor{ -1, 1 };
        const auto    cna1 = guilander::canvas_n_anchor{ c, a1 };
        const auto    cna2 = guilander::canvas_n_anchor{ c, a2 };
        const auto    cna3 = guilander::canvas_n_anchor{ c, a3 };

        expect(cna1.extent_up() == -1);
        expect(cna1.extent_down() == 3);
        expect(cna1.extent_right() == 4);
        expect(cna1.extent_left() == -1);

        expect(cna2.extent_up() == -1);
        expect(cna2.extent_down() == 3);
        expect(cna2.extent_right() == 3);
        expect(cna2.extent_left() == 0);

        expect(cna3.extent_up() == -1);
        expect(cna3.extent_down() == 3);
        expect(cna3.extent_right() == 2);
        expect(cna3.extent_left() == 1);
    };
}
