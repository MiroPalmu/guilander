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

#include <cstddef>
#include <ranges>
#include <type_traits>

#include <experimental/mdspan>
namespace stdex = std::experimental;

namespace guilander {
namespace sstd {

template<typename T>
struct is_mdspan : std::false_type {};

template<typename T, typename E, typename LP, typename AP>
struct is_mdspan<stdex::mdspan<T, E, LP, AP>> : std::true_type {};

template<typename T>
static constexpr bool is_mdspan_v = is_mdspan<T>::value;

template<typename MDS>
    requires is_mdspan_v<MDS>
constexpr std::ranges::view auto
cartesian_iota(const MDS& mds) {
    const auto make_iota_for_rank = [extents = mds.extents()]<MDS::rank_type Rank>() {
        return std::views::iota(typename MDS::index_type{}, extents.extent(Rank));
    };

    return [&]<MDS::rank_type... Ranks>(std::integer_sequence<typename MDS::rank_type, Ranks...>) {
        return std::views::cartesian_product(make_iota_for_rank.template operator()<Ranks>()...);
    }(std::make_integer_sequence<typename MDS::rank_type, MDS::extents_type::rank()>());
}

} // namespace sstd
} // namespace guilander
