cmake_minimum_required(VERSION 3.23)

project(img2psd-cli VERSION 0.2.0 LANGUAGES CXX)

include(FetchContent)

FetchContent_Declare(
  cli-proj
  GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
  GIT_TAG v2.5.0
)
FetchContent_Declare(
  psd-proj
  GIT_REPOSITORY https://github.com/weqeqq/psd.git
  GIT_TAG        9d6e44f1dafa40d48deec316407891323d7ae793
)
FetchContent_Declare(
  indicators-proj
  GIT_REPOSITORY https://github.com/p-ranav/indicators.git
  GIT_TAG v2.3
)
FetchContent_MakeAvailable(
  cli-proj
  psd-proj
  indicators-proj
)
add_executable(img2psd-cli)

target_link_libraries(img2psd-cli PRIVATE
  psd::psd
  image::image
  CLI11::CLI11
  indicators::indicators
)
target_sources(img2psd-cli PRIVATE
  source/main.cc
)
set_target_properties(img2psd-cli PROPERTIES
  CXX_STANDARD          17
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS        OFF
)
target_compile_options(img2psd-cli PRIVATE
  -O3
  -Wall
  -Wpedantic
)
set(CMAKE_EXE_LINKER_FLAGS "-static -static-libgcc -static-libstdc++")
