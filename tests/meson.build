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

subproject('ut')

test_dependencies = []
test_dependencies += dependency('ut')
test_dependencies += guilander_dep

test_env = [
    'GUILANDER_TEST_TTF_FONT_PATH=' + files('ComicMono.ttf')[0].full_path(),
]

# List of tests that can run in parallel
unit_tests = [
    'test_unit_test',
    'test_sstd_mdspan',
    'test_canvas_concepts',
    'test_canvas_anchor',
    'test_canvas_stamp',
    'test_font',
]

test_names_and_exes = {}

# Create test executables:
foreach test_name : unit_tests
    test_names_and_exes += {test_name :
        executable(
            test_name,
            files(test_name + '.cpp'),
            dependencies: test_dependencies,
            override_options: ['optimization=0'],
        )
    }

endforeach

foreach test_name, test_exe : test_names_and_exes
    test(
        test_name,
        test_exe,
        is_parallel: true,
        timeout: 10,
        env: test_env,
    )
endforeach
