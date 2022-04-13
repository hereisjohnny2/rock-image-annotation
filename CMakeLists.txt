cmake_minimum_required(VERSION 3.16)
project(rock_image_cpp)

set(CMAKE_CXX_STANDARD 17)
add_definitions(-D_GLIBCXX_USE_CXX17_ABI=0)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

list(APPEND CMAKE_PREFIX_PATH "/usr/local/libtorch")
find_package(Torch REQUIRED)

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED
)

add_executable(rock_image_cpp
        src/main.cpp
        src/windows/RockImageWindow/rockimageui.cpp
        src/windows/RockImageWindow/rockimageui.h
        src/widgets/PixelDataTable.cpp
        src/widgets/PixelDataTable.h
        src/widgets/ImageDisplaySubWindow.cpp
        src/widgets/ImageDisplaySubWindow.h
        src/widgets/ImageDisplayWidget.cpp
        src/widgets/ImageDisplayWidget.h
        src/windows/Dialogs/CustomMessageDialogs.cpp
        src/windows/Dialogs/CustomMessageDialogs.h
        src/widgets/StackedImagesWidget.cpp
        src/widgets/StackedImagesWidget.h
        src/NeuralNetwork/models/RockImageModel.cpp
        src/NeuralNetwork/models/RockImageModel.h
        src/NeuralNetwork/datasets/RockImageDataset.cpp 
        src/NeuralNetwork/datasets/RockImageDataset.h 
        src/NeuralNetwork/entities/RockData.h 
        src/NeuralNetwork/services/RockImageModelTrainService.cpp 
        src/NeuralNetwork/services/RockImageModelTrainService.h 
        src/NeuralNetwork/services/RockImageModelTestService.cpp
        src/NeuralNetwork/services/RockImageModelTestService.h
)

target_link_libraries(rock_image_cpp
        "${TORCH_LIBRARIES}"
        Qt::Core
        Qt::Gui
        Qt::Widgets
)

