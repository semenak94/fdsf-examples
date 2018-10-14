cmake_minimum_required(VERSION 3.8)
project(Examples)

# Сборка вспомогательной статической библиотеки
set(UTILS_FILES src/Main.cpp src/Common.cpp src/Common.h)
add_library(Utils STATIC ${UTILS_FILES})

# Директории подключаемых файлов
include_directories("3dparty")
include_directories("src")

link_directories(${CMAKE_CURRENT_SOURCE_DIR}/build/Release)

###############################################################################
## file globbing ##############################################################
###############################################################################

# these instructions search the directory tree when cmake is
# invoked and put all files that match the pattern in the variables 
# `sources` and `data`
file(GLOB projects "src/volume1/example-*.cpp"
                   "src/volume2/example-*.cpp")

###############################################################################
## Для каждого файла example-*.cpp отдельный проект 
###############################################################################
foreach(project ${projects})
    get_filename_component(file_basename ${project} NAME_WE)
    string(REGEX REPLACE "example-([^$]+)" "example-\\1" testcase ${file_basename})
    add_executable(${testcase} ${project})
    if (MSVC)
        # Собираем только Release
        set(CMAKE_CONFIGURATION_TYPES Release)
        # TODO
        cmake_policy(SET CMP0043 NEW)
        set_target_properties(${testcase} PROPERTIES
            CXX_STANDARD 14
            CXX_STANDARD_REQUIRED ON
        )
    else()
        set_target_properties(${testcase} PROPERTIES COMPILE_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wno-deprecated -Wno-float-equal")
    endif()

    target_compile_definitions(${testcase} PRIVATE CATCH_CONFIG_FAST_COMPILE)
    target_include_directories(${testcase} PRIVATE ${CATCH_INCLUDE_DIR})
    target_include_directories(${testcase} PRIVATE "src")
    if (MSVC)
        target_link_libraries(${testcase} Utils.lib)
    else()
        target_link_libraries(${testcase} Utils.a stdc++fs)
    endif()

endforeach()
