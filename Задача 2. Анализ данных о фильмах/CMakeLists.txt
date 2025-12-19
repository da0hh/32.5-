cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 20)
set(BUILD_CPR_TESTS OFF)
set(CMAKE_USE_OPENSSL OFF)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

include(FetchContent)
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git
        GIT_TAG 3b15fa82ea74739b574d705fea44959b58142eb8)
FetchContent_MakeAvailable(cpr)

set(JSON_BuildTests OFF CACHE INTERNAL "")

add_subdirectory(nlohmann_json)

add_executable (belskiyNik "belskiyNik.cpp" "belskiyNik.h")
target_link_libraries(belskiyNik PRIVATE cpr)
target_link_libraries(belskiyNik PRIVATE nlohmann_json)
