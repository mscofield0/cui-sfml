# -----------------------------

cmake_minimum_required(VERSION 3.14)
project(CUI)


# -----------------------------

set(SFML_DIR "${CMAKE_SOURCE_DIR}/include/ext/sfml/lib/cmake/SFML")
set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include/")

include_directories(${INCLUDE_DIR})

# find_package(SFML COMPONENTS system window graphics REQUIRED)
# include_directories(${SFML_INCLUDE_DIR})

# -----------------------------


add_executable(CUI ./src/main.cpp)

target_compile_definitions(CUI PUBLIC CUI_ENABLE_CONSTEXPR_PARSING)
# target_link_libraries(CUI sfml-system sfml-window sfml-graphics)
set_target_properties(CUI   
	PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
target_compile_features(CUI PUBLIC cxx_std_17)
target_compile_options(CUI PRIVATE -Wall -Wextra -Wpedantic)

# -Wall -Wextra -Wpedantic