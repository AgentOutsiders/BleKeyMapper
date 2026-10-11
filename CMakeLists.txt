# Minimum version of CMake required
cmake_minimum_required(VERSION 3.10)

# Project name and language
project(BleKeyMapper CXX)

# Set the C++ standard to C++17 minimum
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the executable target for the project
add_executable(BleKeyMapper 
    src/main.cpp 
    src/BleRule.cpp
    src/RuleManager.cpp
    src/ConfigLoader.cpp
    )

# Tell the compiler where to look for .h files
target_include_directories(BleKeyMapper PRIVATE include)

# Enable strict warnings for the target
if(MSVC)
    # Windows/MSVC strict flags
    target_compile_options(BleKeyMapper PRIVATE /W4 /WX)
else()
    # Linux/GCC/Clang strict flags
    target_compile_options(BleKeyMapper PRIVATE 
        -Wall 
        -Wextra 
        -Wpedantic 
        -Wconversion # Warns about implicit casts (like uint64_t to size_t)
        -Wshadow     # Warns if a local variable hides a class attribute
        -Werror      # Treats all warnings as errors (stops compilation)
    )
endif()

# Automatically copy the 'profiles' folder to the output directory after building
add_custom_command(TARGET BleKeyMapper POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/profiles"
    "$<TARGET_FILE_DIR:BleKeyMapper>/profiles"
)