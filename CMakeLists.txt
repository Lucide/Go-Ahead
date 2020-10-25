cmake_minimum_required(VERSION 3.17)

project(Go-Ahead
        VERSION 1
        DESCRIPTION "a tiny particle sandbox game"
        LANGUAGES C)

add_executable(core 70.c)
set_target_properties(core
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "urb")
add_executable(launcher launcher.c)
add_dependencies(launcher core)
