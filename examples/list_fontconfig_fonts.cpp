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

#include <iostream>
#include <string>

#include "guilander/font.hpp"

int
main() {
    std::cout << "Only monospace fonts [yes or no (default)]: ";
    auto monospace_input = std::string{};
    std::getline(std::cin, monospace_input);

    std::cout << "Only scalable fonts [yes or no (default)]: ";
    auto scalable_input = std::string{};
    std::getline(std::cin, scalable_input);

    std::cout << "Family name [any or name (default)]: ";
    auto family_name_input = std::string{};
    std::getline(std::cin, family_name_input);

    std::cout << "Style [any or style (default)]: ";
    auto style_input = std::string{};
    std::getline(std::cin, style_input);

    guilander::fc::print_available_fonts({
        .monospace = monospace_input == "yes",
        .scalable  = scalable_input == "yes",
        .family_name =
            family_name_input == "any" ? std::optional<std::string>{} : family_name_input,
        .style = style_input == "any" ? std::optional<std::string>{} : style_input,
    });
}
