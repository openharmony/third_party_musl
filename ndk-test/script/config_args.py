import shutil
import os

cmake_path = shutil.which('cmake')

cmake_file = os.path.join(os.path.dirname(
    cmake_path), '../../../build/cmake/ohos.toolchain.cmake')

find_toolchain_file = os.path.isfile(cmake_file)

if(find_toolchain_file):
    toolchain_file = cmake_file
else:
    print(cmake_file+" not found")
    # toolchain_file = "~/openharmony/out/sdk/packages/ohos-sdk/linux/native/build/cmake/ohos.toolchain.cmake"


linux_args = [
    '-DOHOS_STL=c++_shared',
    '-DOHOS_ARCH=armeabi-v7a',
    '-DOHOS_PLATFORM=OHOS',
    '-DCMAKE_TOOLCHAIN_FILE='+toolchain_file,
]