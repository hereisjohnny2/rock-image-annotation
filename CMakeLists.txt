cmake_minimum_required(VERSION 3.21)
project(rock_image_cpp)

set(CMAKE_CXX_STANDARD 17)
add_definitions(-D_GLIBCXX_USE_CXX17_ABI=0)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(
        rock_image_cpp
        src/main.cpp
        src/windows/RockImageWindow/rockimageui.cpp
        src/windows/RockImageWindow/rockimageui.h
        src/widgets/PixelDataTable.cpp
        src/widgets/PixelDataTable.h
        src/widgets/ImageDisplaySubWindow.cpp
        src/widgets/ImageDisplaySubWindow.h
        src/widgets/ImageDisplayWidget.cpp
        src/widgets/ImageDisplayWidget.h
)

target_link_libraries(rock_image_cpp
        Qt::Core
        Qt::Gui
        Qt::Widgets
        )

