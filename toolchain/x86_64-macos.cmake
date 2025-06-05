
set(CMAKE_SYSTEM_NAME      Darwin)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

set(CMAKE_C_COMPILER   $ENV{HOME}/osxcross/target/bin/x86_64-apple-darwin24.5-clang)
set(CMAKE_CXX_COMPILER $ENV{HOME}/osxcross/target/bin/x86_64-apple-darwin24.5-clang++)

set(CMAKE_LINKER $ENV{HOME}/osxcross/target/bin/x86_64-apple-darwin24.5-ld)
set(CMAKE_EXE_LINKER_FLAGS "-fuse-ld=$ENV{HOME}/osxcross/target/bin/x86_64-apple-darwin24.5-ld")

set(CMAKE_AR $ENV{HOME}/osxcross/target/bin/x86_64-apple-darwin24.5-ar)
set(CMAKE_RANLIB $ENV{HOME}/osxcross/target/bin/x86_64-apple-darwin24.5-ranlib)

set(CMAKE_OSX_SYSROOT $ENV{HOME}/osxcross/target/SDK/MacOSX15.5.sdk)
set(CMAKE_OSX_DEPLOYMENT_TARGET 13.3 CACHE STRING "Minimum macOS version")

set(CMAKE_FIND_ROOT_PATH $ENV{HOME}/osxcross/target/SDK/MacOSX15.5.sdk)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
