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

#include <algorithm>
#include <format>
#include <string>

#include "guilander/canvas.hpp"
#include "guilander/mdspan.hpp"

auto
format_canvas(const guilander::canvas auto got, const guilander::canvas auto should_be) {
    auto s = std::format("Expecting {} x {} canvas:\n", should_be.extent(0), should_be.extent(1));

    for (auto i = 0uz; i < should_be.extent(0); ++i) {
        auto row = std::string{};
        for (auto j = 0uz; j < should_be.extent(1); ++j) {
            row += std::format("{:6}", should_be[i, j]);
        }
        s += row + "\n";
    }
    s += std::format("Got {} x {} canvas:\n", got.extent(0), got.extent(1));
    for (auto i = 0uz; i < got.extent(0); ++i) {
        auto row = std::string{};
        for (auto j = 0uz; j < got.extent(1); ++j) { row += std::format("{:6}", got[i, j]); }
        s += row + "\n";
    }
    return s;
}

int
main() {
    using namespace boost::ut;

    cfg<override> = { .tag = { "canvas" } };

    tag("canvas") / "canvas_n_anchor::stamp can replace whole canvas"_test = [] {
        int        buffA[]{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
        const auto canvasA = stdex::mdspan(buffA, 3, 4);
        const auto anchorA = guilander::canvas_anchor{ 0, 0 };
        const auto cnaA    = guilander::canvas_n_anchor{ canvasA, anchorA };

        constexpr int buffB[]{ 1000, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110 };
        const auto    canvasB = stdex::mdspan(buffB, 3, 4);
        const auto    anchorB = guilander::canvas_anchor{ 0, 0 };
        const auto    cnaB    = guilander::canvas_n_anchor{ canvasB, anchorB };

        expect(not std::ranges::equal(buffA, buffB)) << format_canvas(canvasA, canvasB);
        cnaA.stamp(cnaB);
        expect(std::ranges::equal(buffA, buffB)) << format_canvas(canvasA, canvasB);
    };

    tag("canvas") / "canvas_n_anchor::stamp can replace part of canvas"_test = [] {
        int        buffA[]{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
        const auto canvasA = stdex::mdspan(buffA, 3, 4);
        const auto anchorA = guilander::canvas_anchor{ 0, 0 };
        const auto cnaA    = guilander::canvas_n_anchor{ canvasA, anchorA };

        constexpr int buffB[]{ 10, 11, 12, 13 };
        const auto    canvasB = stdex::mdspan(buffB, 2, 2);
        const auto    anchorB = guilander::canvas_anchor{ 0, 0 };
        const auto    cnaB    = guilander::canvas_n_anchor{ canvasB, anchorB };

        constexpr int correctAB[]{ 10, 11, 2, 3, 12, 13, 6, 7, 8, 9, 10, 11 };

        expect(not std::ranges::equal(buffA, correctAB));
        cnaA.stamp(cnaB);
        expect(std::ranges::equal(buffA, correctAB))
            << format_canvas(canvasA, stdex::mdspan(correctAB, 3, 4));

        constexpr int buffC[]{ 100, 101, 102, 103 };
        const auto    canvasC = stdex::mdspan(buffC, 2, 2);
        const auto    anchorC = guilander::canvas_anchor{ 0, 1 };
        const auto    cnaC    = guilander::canvas_n_anchor{ canvasC, anchorC };

        const auto anchorD = guilander::canvas_anchor{ 0, 1 };
        const auto cnaD    = guilander::canvas_n_anchor{ canvasA, anchorD };

        constexpr int correctDB[]{ 10, 10, 11, 3, 12, 12, 13, 7, 8, 9, 10, 11 };
        constexpr int correctDC[]{ 100, 101, 11, 3, 102, 103, 13, 7, 8, 9, 10, 11 };

        expect(not std::ranges::equal(buffA, correctDB));
        cnaD.stamp(cnaB);
        expect(std::ranges::equal(buffA, correctDB))
            << format_canvas(canvasA, stdex::mdspan(correctDB, 3, 4));

        expect(not std::ranges::equal(buffA, correctDC));
        cnaD.stamp(cnaC);
        expect(std::ranges::equal(buffA, correctDC))
            << format_canvas(canvasA, stdex::mdspan(correctDC, 3, 4));
    };

    tag("canvas") / "canvas_n_anchor::stamp can extend out of canvas top left corner"_test = [] {
        int        buffA[]{ 0, 1, 2, 3 };
        const auto canvasA = stdex::mdspan(buffA, 2, 2);
        const auto anchorA = guilander::canvas_anchor{ 1, 1 };
        const auto cnaA    = guilander::canvas_n_anchor{ canvasA, anchorA };

        static constexpr int buffB[]{ 10, 11, 12, 13, 14, 15, 16, 17, 18 };
        const auto           canvasB = stdex::mdspan(buffB, 3, 3);
        const auto           cnaB    = guilander::canvas_n_anchor{ canvasB, { 0, 0 } };
        const auto           cnaC    = guilander::canvas_n_anchor{ canvasB, { 1, 1 } };
        const auto           cnaD    = guilander::canvas_n_anchor{ canvasB, { 2, 2 } };
        const auto           cnaE    = guilander::canvas_n_anchor{ canvasB, { 3, 3 } };
        const auto           cnaF    = guilander::canvas_n_anchor{ canvasB, { 4, 4 } };

        constexpr int correctB[]{ 0, 1, 2, 10 };
        constexpr int correctC[]{ 10, 11, 13, 14 };
        constexpr int correctD[]{ 14, 15, 17, 18 };
        constexpr int correctE[]{ 18, 15, 17, 18 };

        cnaA.stamp(cnaB);
        expect(std::ranges::equal(buffA, correctB))
            << format_canvas(canvasA, stdex::mdspan(correctB, 2, 2));

        cnaA.stamp(cnaC);
        expect(std::ranges::equal(buffA, correctC))
            << format_canvas(canvasA, stdex::mdspan(correctC, 2, 2));

        cnaA.stamp(cnaD);
        expect(std::ranges::equal(buffA, correctD))
            << format_canvas(canvasA, stdex::mdspan(correctD, 2, 2));

        cnaA.stamp(cnaE);
        expect(std::ranges::equal(buffA, correctE))
            << format_canvas(canvasA, stdex::mdspan(correctE, 2, 2));

        cnaA.stamp(cnaF);
        expect(std::ranges::equal(buffA, correctE))
            << format_canvas(canvasA, stdex::mdspan(correctE, 2, 2));
    };

    tag("canvas") / "canvas_n_anchor::stamp is noop for non-overlapping canvas"_test = [] {
        int        buffA[]{ 0, 1 };
        const auto canvasA = stdex::mdspan(buffA, 2, 1);
        const auto anchorA = guilander::canvas_anchor{ 0, 0 };
        const auto cnaA    = guilander::canvas_n_anchor{ canvasA, anchorA };

        static constexpr int buffB[]{ 11, 12, 13, 14 };
        constexpr auto       canvasB = stdex::mdspan(buffB, 2, 2);
        constexpr auto       cnaB    = guilander::canvas_n_anchor{ canvasB, { -2, 0 } };
        constexpr auto       cnaC    = guilander::canvas_n_anchor{ canvasB, { 2, 0 } };
        constexpr auto       cnaD    = guilander::canvas_n_anchor{ canvasB, { 0, -1 } };
        constexpr auto       cnaE    = guilander::canvas_n_anchor{ canvasB, { 0, 2 } };

        static constexpr int original[]{ 0, 1 };
        constexpr auto       og_mds = stdex::mdspan(original, 2, 1);

        cnaA.stamp(cnaB);
        expect(std::ranges::equal(buffA, original)) << format_canvas(canvasA, og_mds);
        cnaA.stamp(cnaC);
        expect(std::ranges::equal(buffA, original)) << format_canvas(canvasA, og_mds);
        cnaA.stamp(cnaD);
        expect(std::ranges::equal(buffA, original)) << format_canvas(canvasA, og_mds);
        cnaA.stamp(cnaE);
        expect(std::ranges::equal(buffA, original)) << format_canvas(canvasA, og_mds);
    };

    tag("canvas") / "canvas_n_anchor::stamp can be used with larger canvases"_test = [] {
        // clang-format off
        int        buffA[]{ 111, 222, 333,
                            444, 555, 666 };
        // clang-format on

        const auto canvasA = stdex::mdspan(buffA, 2, 3);
        const auto anchorA = guilander::canvas_anchor{ 1, 2 };
        const auto cnaA    = guilander::canvas_n_anchor{ canvasA, anchorA };

        // clang-format off
        static constexpr int buffB[]{  0,  1,  2,  3,
                                       4,  5,  6,  7,
                                       8,  9, 10, 11,
                                      12, 13, 14, 15  };
        // clang-format on
        constexpr auto canvasB = stdex::mdspan(buffB, 4, 4);
        constexpr auto cnaB    = guilander::canvas_n_anchor{ canvasB, { 3, 3 } };
        constexpr auto cnaC    = guilander::canvas_n_anchor{ canvasB, { 3, 2 } };
        constexpr auto cnaD    = guilander::canvas_n_anchor{ canvasB, { 2, 2 } };
        constexpr auto cnaE    = guilander::canvas_n_anchor{ canvasB, { 1, 2 } };

        // clang-format off
        static constexpr int        correctB[]{  9, 10, 11,
                                                13, 14, 15  };
        static constexpr int        correctC[]{  8,  9, 10,
                                                12, 13, 14  };
        static constexpr int        correctD[]{  4,  5,  6,
                                                 8,  9, 10  };
        static constexpr int        correctE[]{  0,  1,  2,
                                                 4,  5,  6  };
        // clang-format on

        constexpr auto correctBmds = stdex::mdspan(correctB, 2, 3);
        constexpr auto correctCmds = stdex::mdspan(correctC, 2, 3);
        constexpr auto correctDmds = stdex::mdspan(correctD, 2, 3);
        constexpr auto correctEmds = stdex::mdspan(correctE, 2, 3);

        cnaA.stamp(cnaB);
        expect(std::ranges::equal(buffA, correctB)) << format_canvas(canvasA, correctBmds);
        cnaA.stamp(cnaC);
        expect(std::ranges::equal(buffA, correctC)) << format_canvas(canvasA, correctCmds);
        cnaA.stamp(cnaD);
        expect(std::ranges::equal(buffA, correctD)) << format_canvas(canvasA, correctDmds);
        cnaA.stamp(cnaE);
        expect(std::ranges::equal(buffA, correctE)) << format_canvas(canvasA, correctEmds);
    };

    tag("canvas") / "canvas_n_anchor::stamp is noop for zero measure canvas"_test = [] {
        int        buffA[]{ 0, 1, 2, 3 };
        const auto canvasA = stdex::mdspan(buffA, 2, 2);
        const auto anchorA = guilander::canvas_anchor{ 0, 0 };
        const auto cnaA    = guilander::canvas_n_anchor{ canvasA, anchorA };

        static constexpr int buffB[]{ 11, 12, 13, 14 };
        constexpr auto       canvasB1 = stdex::mdspan(buffB, 2, 0);
        constexpr auto       canvasB2 = stdex::mdspan(buffB, 0, 2);
        constexpr auto       canvasB3 = stdex::mdspan(buffB, 0, 0);
        constexpr auto       cnaB1    = guilander::canvas_n_anchor{ canvasB1, { 0, 0 } };
        constexpr auto       cnaB2    = guilander::canvas_n_anchor{ canvasB2, { 0, 0 } };
        constexpr auto       cnaB3    = guilander::canvas_n_anchor{ canvasB3, { 0, 0 } };

        static constexpr int original[]{ 0, 1, 2, 3 };
        constexpr auto       og_mds = stdex::mdspan(original, 2, 2);

        cnaA.stamp(cnaB1);
        expect(std::ranges::equal(buffA, original)) << format_canvas(canvasA, og_mds);
        cnaA.stamp(cnaB2);
        expect(std::ranges::equal(buffA, original)) << format_canvas(canvasA, og_mds);
        cnaA.stamp(cnaB3);
        expect(std::ranges::equal(buffA, original)) << format_canvas(canvasA, og_mds);
    };
}
