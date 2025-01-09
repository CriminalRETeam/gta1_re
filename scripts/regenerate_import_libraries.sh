#!/bin/bash

set -e

# Get the canonical path of the folder containing this script
root=`cd -P -- "\`dirname -- "$0"\`/.." && printf '%s\n' "\`pwd -P\`"`
echo "$root"

cd "$root"

set -x

i686-w64-mingw32-dlltool --input-def 3rdparty/miles/lib/win32/mss32.def --output-lib 3rdparty/miles/lib/win32/mss32.lib        --add-stdcall-underscore
i686-w64-mingw32-dlltool --input-def 3rdparty/smack/lib/win32/smackw32.def --output-lib 3rdparty/smack/lib/win32/smackw32.lib  --add-stdcall-underscore
i686-w64-mingw32-dlltool --input-def 3rdparty/dxsdk/lib/win32/dplayx.def --output-lib 3rdparty/dxsdk/lib/win32/dplayx.lib      --kill-at
