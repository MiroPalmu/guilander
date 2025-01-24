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

#include <array>
#include <utility>

#include "guilander/mdspan.hpp"

int
main() {
    using namespace boost::ut;

    cfg<override> = { .tag = { "sstd" } };

    tag("sstd") / "is_mdspan is correct for sample of types"_test = [] {
        expect(not guilander::sstd::is_mdspan_v<int>);
        struct Foo {};
        expect(not guilander::sstd::is_mdspan_v<Foo>);
        expect(not guilander::sstd::is_mdspan_v<decltype([] {})>);

        expect(guilander::sstd::is_mdspan_v<stdex::mdspan<int, stdex::dextents<std::size_t, 0>>>);
        expect(guilander::sstd::is_mdspan_v<stdex::mdspan<int, stdex::dextents<std::size_t, 1>>>);
        expect(guilander::sstd::is_mdspan_v<stdex::mdspan<int, stdex::dextents<std::size_t, 2>>>);
        expect(guilander::sstd::is_mdspan_v<stdex::mdspan<int, stdex::dextents<std::size_t, 4>>>);

        using extentsX = stdex::extents<int, 4, std::dynamic_extent, 48>;
        using layoutX  = stdex::layout_stride;
        struct accessorX {
            using element_type     = int;
            using data_handle_type = int*;
            using reference        = int&;
            using offset_policy    = accessorX;

            reference
            access(data_handle_type, std::size_t);
            reference
            offset(data_handle_type, std::size_t);
        };
        using mdspanX = stdex::mdspan<Foo, extentsX, layoutX, accessorX>;

        // Silence unused local typedefs warning.
        std::ignore = [](accessorX::element_type, accessorX::offset_policy) {};

        expect(guilander::sstd::is_mdspan_v<mdspanX>);
    };

    tag("sstd") / "mdindices does not skip any indecies"_test = [] {
        auto buff = std::array<int, 8>{};

        const auto mdspan1D = stdex::mdspan(buff.data(), 8);
        const auto mdspan2D = stdex::mdspan(buff.data(), 2, 4);
        const auto mdspan3D = stdex::mdspan(buff.data(), 2, 2, 2);

        for (const auto [i] : guilander::sstd::mdindices(mdspan1D)) { mdspan1D[i] = i; }
        expect(buff == std::array{ 0, 1, 2, 3, 4, 5, 6, 7 });

        for (const auto [i, j] : guilander::sstd::mdindices(mdspan2D)) { mdspan2D[i, j] = i + j; }
        // clang-format off
        expect(buff == std::array{ 0 + 0, 0 + 1, 0 + 2, 0 + 3,
                                   1 + 0, 1 + 1, 1 + 2, 1 + 3 });
        //clang-format on

        for (const auto [i, j, k] : guilander::sstd::mdindices(mdspan3D)) {
            mdspan3D[i, j, k] = i + j + k;
        }

        //clang-format off
        expect(buff == std::array{ 0 + 0 + 0, 0 + 0 + 1,
                                   0 + 1 + 0, 0 + 1 + 1,

                                   1 + 0 + 0, 1 + 0 + 1,
                                   1 + 1 + 0, 1 + 1 + 1});
        //clang-format on
    };
}
