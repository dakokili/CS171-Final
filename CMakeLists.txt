cmake_minimum_required(VERSION 3.17)
project(CS171-Final LANGUAGES C CXX)

option(USE_FILESYSTEM "use std::filesystem" off)

option(GLFW_BUILD_TESTS off)
option(GLFW_BUILD_EXAMPLES off)
option(GLFW_BUILD_DOCS off)
find_package(glfw3 CONFIG REQUIRED)
list(APPEND LIBRARIES glfw)

find_package(glad CONFIG REQUIRED)
list(APPEND LIBRARIES glad::glad)

find_package(Eigen3 CONFIG REQUIRED)
list(APPEND LIBRARIES Eigen3::Eigen)

find_package(OpenVDB CONFIG REQUIRED)
list(APPEND LIBRARIES OpenVDB::openvdb)

# add filesystem
if (USE_FILESYSTEM)
  list(APPEND DEFINES -DUSE_FILESYSTEM)
  if (NOT MSVC)
    list(APPEND LIBRARIES stdc++fs)
  endif()
endif()

# set source files and add executable
file(GLOB SRC_FILES src/*.cpp)

add_executable(main ${SRC_FILES} ${INTERNAL_SRC_FILES})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /openmp")
target_include_directories(main PRIVATE include)
target_compile_features(main PRIVATE cxx_std_17)
target_compile_definitions(main PRIVATE ${DEFINES})
target_link_libraries(main PRIVATE ${LIBRARIES})