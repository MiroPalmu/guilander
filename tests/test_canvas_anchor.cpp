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

    tag("canvas") / "anchor_transform from a anchor to the same anchor is identity"_test = [] {
        constexpr auto x = guilander::canvas_anchor{ -12, 10 };
        constexpr auto y = guilander::canvas_anchor{ 1, 2 };

        expect(guilander::anchor_transform(x, y, y) == x);
        expect(guilander::anchor_transform(y, x, x) == y);
    };

    tag("canvas") / "anchor_transform of anchor from itself is the resulting anchor"_test = [] {
        constexpr auto x = guilander::canvas_anchor{ -12, 10 };
        constexpr auto y = guilander::canvas_anchor{ 1, 2 };

        expect(guilander::anchor_transform(x, x, y) == y);
        expect(guilander::anchor_transform(y, y, x) == x);
        expect(guilander::anchor_transform(x, x, x) == x);
        expect(guilander::anchor_transform(y, y, y) == y);
    };

    tag("canvas") / "roundtrip anchor_transform is identity"_test = [] {
        constexpr auto x = guilander::canvas_anchor{ -12, 10 };
        constexpr auto y = guilander::canvas_anchor{ 1, 2 };
        constexpr auto z = guilander::canvas_anchor{ 13, -2 };

        auto round_trip = [](const auto a, const auto b, const auto c) {
            const auto a_from_b_to_c = guilander::anchor_transform(a, b, c);
            return guilander::anchor_transform(a_from_b_to_c, c, b);
        };

        expect(round_trip(x, x, x) == x);
        expect(round_trip(x, x, y) == x);
        expect(round_trip(x, x, z) == x);
        expect(round_trip(x, y, x) == x);
        expect(round_trip(x, y, y) == x);
        expect(round_trip(x, y, z) == x);
        expect(round_trip(x, z, x) == x);
        expect(round_trip(x, z, y) == x);
        expect(round_trip(x, z, z) == x);

        expect(round_trip(y, x, x) == y);
        expect(round_trip(y, x, y) == y);
        expect(round_trip(y, x, z) == y);
        expect(round_trip(y, y, x) == y);
        expect(round_trip(y, y, y) == y);
        expect(round_trip(y, y, z) == y);
        expect(round_trip(y, z, x) == y);
        expect(round_trip(y, z, y) == y);
        expect(round_trip(y, z, z) == y);

        expect(round_trip(z, x, x) == z);
        expect(round_trip(z, x, y) == z);
        expect(round_trip(z, x, z) == z);
        expect(round_trip(z, y, x) == z);
        expect(round_trip(z, y, y) == z);
        expect(round_trip(z, y, z) == z);
        expect(round_trip(z, z, x) == z);
        expect(round_trip(z, z, y) == z);
        expect(round_trip(z, z, z) == z);
    };
}
