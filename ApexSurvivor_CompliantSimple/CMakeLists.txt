cmake_minimum_required(VERSION 3.16)
project(ApexSurvivorSimple LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(ApexSurvivor
    src/main.cpp
    src/Game.cpp
    src/GameObject.cpp
    src/PlayerCar.cpp
    src/Enemy.cpp
    src/Collectible.cpp
)

target_include_directories(ApexSurvivor PRIVATE include)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)
target_link_libraries(ApexSurvivor PRIVATE sfml-graphics sfml-window sfml-system)

# Copy PNG files to the build folder after building.
# This keeps paths like "assets/f1_car.png" working in Qt Creator.
add_custom_command(TARGET ApexSurvivor POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/assets
            $<TARGET_FILE_DIR:ApexSurvivor>/assets
)
