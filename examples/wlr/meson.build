# Copyright (C) 2025 Miro Palmu.
#
# This file is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This file is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this file.  If not, see <https://www.gnu.org/licenses/>.

executable(
    'wlr_text_overlay',
    files(
         'text_overlay.cpp',
         'static_picture_overlay.cpp',
    ),
    include_directories : include_directories('.'),
    dependencies: [
        guilander_dep,
        cpp.find_library('unistring'),
    ],
)
