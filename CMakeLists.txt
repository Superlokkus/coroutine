cmake_minimum_required(VERSION 3.11)
project(coroutine_demo VERSION 1.0)
include(${CMAKE_BINARY_DIR}/conan_paths.cmake) #Remove for conan agnostic build (let me know how you got it done with conan2 to acutally include the asio headers)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

find_package(Boost REQUIRED)

set(files
        ${CMAKE_CURRENT_SOURCE_DIR}/src/demo_throw.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/ub_lurking.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/completion_handlers.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/old_way.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/new_way.cpp
)
foreach (file ${files})
    get_filename_component(Name ${file} NAME_WE)
    add_executable(${Name} ${file})
    set_property(TARGET ${Name} PROPERTY LINKER_LANGUAGE CXX)
    set_property(TARGET ${Name} PROPERTY CXX_STANDARD 20)
    target_link_libraries(${Name} PUBLIC Boost::boost)

endforeach ()

