cmake_minimum_required(VERSION 3.14)
project(CUI-SFML)

# -----------------------------

set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include/")
set(CUI_DIR "${CMAKE_SOURCE_DIR}/CUI")
set(SFML_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/SFML-2.5.1/include")
set(SFML_DIR "${CMAKE_SOURCE_DIR}/SFML-2.5.1/lib/cmake/SFML")


add_subdirectory(${CUI_DIR})
find_package(SFML COMPONENTS system window graphics REQUIRED)

# -----------------------------

add_executable(CUI-SFML ./src/main.cpp)

target_include_directories(CUI-SFML PRIVATE ${INCLUDE_DIR})
target_link_libraries(CUI-SFML sfml-system sfml-window sfml-graphics)
target_link_libraries(CUI-SFML CUI)
set_target_properties(CUI-SFML   
	PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
target_compile_features(CUI-SFML PUBLIC cxx_std_17)
target_compile_options(CUI-SFML PRIVATE -g -Wall -Wextra -Wpedantic)