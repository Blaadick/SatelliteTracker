cmake_minimum_required(VERSION 4.3)

project(SatelliteTracker
    DESCRIPTION "Satellite tracking time calculator"
    VERSION 0.0.1
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} CONFIGURE_DEPENDS src/*.cpp)
file(GLOB_RECURSE HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} CONFIGURE_DEPENDS include/*.hpp)

find_package(nlohmann_json REQUIRED CONFIG)

add_executable(SatelliteTracker ${SOURCES} ${HEADERS})
target_include_directories(SatelliteTracker PRIVATE include)
target_link_libraries(SatelliteTracker PRIVATE nlohmann_json::nlohmann_json)
target_compile_definitions(SatelliteTracker PRIVATE
    PROJECT_NAME="${PROJECT_NAME}"
    PROJECT_DESCRIPTION="${PROJECT_DESCRIPTION}"
    PROJECT_VERSION="${PROJECT_VERSION}"
)
