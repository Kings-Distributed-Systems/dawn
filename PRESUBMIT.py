# Copyright 2018 The Dawn Authors
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import os
import platform
import subprocess

def _DoClangFormat(input_api, output_api):
    if platform.system() == 'Linux' and platform.architecture()[0] == '64bit':
        clang_format_path = 'buildtools/linux64/clang-format'
    elif platform.system() == 'Mac':
        clang_format_path = 'buildtools/mac/clang-format'
    elif platform.system() == 'Windows':
        clang_format_path = 'buildtools/win/clang-format.exe'
    else:
        return [output_api.PresubmitNotifyResult('Skipping clang-format')]

    # We need to know which commit to diff against. It doesn't seem to be exposed anywhere
    # except in that private member of presubmit_support.Change. This is likely to break
    # but hopefully we have an updated clang-format in CPID/GS before it does.
    upstream_commit = input_api.change._upstream
    if upstream_commit == None:
        return []

    lint_cmd = [
        'scripts/lint_clang_format.sh',
        clang_format_path,
        upstream_commit
    ]

    # Call the linting script and forward the output as a notification or as an error
    try:
        output = subprocess.check_output(lint_cmd)
        return [output_api.PresubmitNotifyResult(output)]
    except subprocess.CalledProcessError as e:
        return [output_api.PresubmitError(e.output)]

def _DoCommonChecks(input_api, output_api):
    results = []
    results.extend(input_api.canned_checks.CheckChangedLUCIConfigs(input_api, output_api))
    results.extend(input_api.canned_checks.CheckGNFormatted(input_api, output_api))
    results.extend(_DoClangFormat(input_api, output_api))
    return results

def CheckChangeOnUpload(input_api, output_api):
    return _DoCommonChecks(input_api, output_api)

def CheckChangeOnCommit(input_api, output_api):
    return _DoCommonChecks(input_api, output_api)
