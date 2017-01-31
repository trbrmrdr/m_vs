#WEB_P_SRC="D:\DoodleGod\libs\webp\Android_lib"
#THIS_FOLDER=$(shell cygpath  'pwd')
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
ROOT_SRC=$DIR/../../../
#set -x
#ndk-build -j16 NDK_DEBUG_IMPORTS=1 "NDK_MODULE_PATH=${ROOT_SRC}"
ndk-build -j16 "NDK_MODULE_PATH=${ROOT_SRC}"