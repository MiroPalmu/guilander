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

    tag("canvas") / "only 2D mdspans satisfy canvas concept"_test = [] {
        struct Foo {};
        using mdspan1D = stdex::mdspan<Foo, stdex::extents<int, 2>>;
        using mdspan2D = stdex::mdspan<Foo, stdex::extents<int, 2, 4>>;
        using mdspan3D = stdex::mdspan<Foo, stdex::extents<int, 2, 4, 2>>;
        using mdspan4D = stdex::mdspan<Foo, stdex::extents<int, 2, 4, 2, 3>>;

        using mdspan1Ddyn = stdex::mdspan<Foo, stdex::dextents<int, 1>>;
        using mdspan2Ddyn = stdex::mdspan<Foo, stdex::dextents<int, 2>>;
        using mdspan3Ddyn = stdex::mdspan<Foo, stdex::dextents<int, 3>>;
        using mdspan4Ddyn = stdex::mdspan<Foo, stdex::dextents<int, 4>>;

        expect(not guilander::canvas<mdspan1D>);
        expect(guilander::canvas<mdspan2D>);
        expect(not guilander::canvas<mdspan3D>);
        expect(not guilander::canvas<mdspan4D>);
        expect(not guilander::canvas<mdspan1Ddyn>);
        expect(guilander::canvas<mdspan2Ddyn>);
        expect(not guilander::canvas<mdspan3Ddyn>);
        expect(not guilander::canvas<mdspan4Ddyn>);
    };

    tag("canvas") / "mdspans with const qualified value types are not writable canvases"_test = [] {
        struct Foo {};
        using mds       = stdex::mdspan<Foo, stdex::extents<int, 2, 4>>;
        using const_mds = stdex::mdspan<const Foo, stdex::extents<int, 2, 4>>;
        expect(guilander::writable_canvas<mds>);
        expect(not guilander::writable_canvas<const_mds>);
    };
}
