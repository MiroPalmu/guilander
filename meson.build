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

project(
    'guilander',
    'cpp',
    version: '0.0.1',
    meson_version: '>= 1.4.0',
    default_options : [
      'warning_level=3',
      'cpp_std=c++26',
    ],
)

if not meson.is_subproject()
    add_global_arguments('-fconcepts-diagnostics-depth=10', language : 'cpp')
endif

waylander_proj = subproject('waylander')
waylander_dep = waylander_proj.get_variable('waylander_dep')

guilander_source_files = []

# Subdirectors
subdir('docs')
subdir('src')

cpp = meson.get_compiler('cpp')

guilander_lib = static_library(
    'guilander',
    guilander_source_files,
    include_directories : include_directories('include'),
    dependencies : [
        waylander_dep,
        dependency('freetype2'),
        dependency('mdspan'),
        dependency('mp-units'),
        dependency('fontconfig'),
        cpp.find_library('unistring'),
    ],
)

guilander_dep = declare_dependency(
    include_directories : include_directories('include'),
    link_with : guilander_lib,
    dependencies : [
        waylander_dep,
        dependency('mp-units'),
        dependency('mdspan'),
        dependency('freetype2'),
    ],
)

if not meson.is_subproject()
    # Tests has to come after guiladner_lib as they use it as meson dependency.
    subdir('tests')
    subdir('examples')

    # compile_commands.json stuff:
    compdb = find_program('compdb', required : false)

    if compdb.found() and meson.backend() == 'ninja'
        message('Custom target compdb added for generating compile_commands.json entries for headers')
        custom_target(
            'compdb',
            output: 'compdb',
            command: [
                files('.generate_compile_commands_json_with_headers.sh'),
                meson.project_build_root(),
                meson.project_source_root(),
            ],
            install: false,
            build_always_stale: true,
        )
    else
        message('Skipping custom target compdb for greating compile_commands.json entries for headers due to:')
        if not compdb.found()
            message('- missing compdb (compdb can be installed with: pip install compdb)')
        endif
        if meson.backend() != 'ninja'
            message('- ninja is not meson backend (only backend that generates compile_commands.json)')
        endif
    endif
endif
