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

#pragma once

#include <mp-units/framework.h>
#include <mp-units/systems/typographic.h>

namespace guilander {
namespace units {

inline constexpr struct pixel final : mp_units::named_unit<"#pixels", mp_units::one> {
} pixel;

inline constexpr struct pixel64 final :
    mp_units::named_unit<"#pixel / 64", mp_units::mag_ratio<1, 64> * pixel> {
} pixel64;

inline constexpr struct pixel65536 final :
    mp_units::named_unit<"#pixel / 65536", mp_units::mag_ratio<1, 65536> * pixel> {
} pixel65536;

inline constexpr auto point = mp_units::typographic::point_dtp;

inline constexpr struct dpi final :
    mp_units::named_unit<"dpi", pixel / mp_units::international::inch> {
} dpi;

using dpi_double           = mp_units::quantity<dpi, double>;
using point_double         = mp_units::quantity<point, double>;
using pixel_double         = mp_units::quantity<pixel, double>;
using pixel_size_t         = mp_units::quantity<pixel, std::size_t>;
using pixel_ptrdiff_t      = mp_units::quantity<pixel, std::ptrdiff_t>;
using pixel64_ptrdiff_t    = mp_units::quantity<pixel64, std::ptrdiff_t>;
using pixel65536_ptrdiff_t = mp_units::quantity<pixel64, std::ptrdiff_t>;

} // namespace units
} // namespace guilander
