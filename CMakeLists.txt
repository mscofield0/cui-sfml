cmake_minimum_required(VERSION 3.14)
project(CUI-SFML)

# -----------------------------

set(CUI_DIR "${CMAKE_SOURCE_DIR}/cui/include/")
set(SFML_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/SFML-2.5.1/include")
set(SFML_DIR "${CMAKE_SOURCE_DIR}/SFML-2.5.1/lib/cmake/SFML")

include_directories(${CUI_DIR})
find_package(SFML COMPONENTS system window graphics REQUIRED)
include_directories(${SFML_INCLUDE_DIR})

# -----------------------------


add_executable(CUI-SFML ./src/main.cpp)

target_compile_definitions(CUI-SFML PUBLIC CUI_ENABLE_CONSTEXPR_PARSING)
target_link_libraries(CUI-SFML sfml-system sfml-window sfml-graphics)
set_target_properties(CUI-SFML   
	PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
target_compile_features(CUI-SFML PUBLIC cxx_std_17)
target_compile_options(CUI-SFML PRIVATE -Wall -Wextra -Wpedantic)