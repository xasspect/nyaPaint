cmake_minimum_required(VERSION 3.16)
project(nyaPaint)

set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
#set(CMAKE_AUTORCC ON)
#set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "C:/Qt/6.11.1/mingw_64")

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

set(SOURCES
        src/main.cpp
        src/canvas/Canvas.cpp
        src/canvas/CanvasModel.cpp
        src/tools/Tool.cpp
        src/tools/ToolManager.cpp
        src/tools/PenTool.cpp
        src/ui/ToolBarWidget.cpp
        src/app/MainWindow.cpp
        src/tools/EraserTool.cpp
        src/ui/ColorPickerWidget.cpp
        src/ui/BrushSizeWidget.cpp
        src/undo/UndoManager.cpp
        src/format/NyaFormat.cpp
        src/tools/LineTool.cpp
        src/tools/FillTool.cpp
)

set(HEADERS
        src/canvas/Canvas.h
        src/canvas/CanvasModel.h
        src/tools/Tool.h
        src/tools/ToolManager.h
        src/tools/PenTool.h
        src/ui/ToolBarWidget.h
        src/app/MainWindow.h
        src/tools/EraserTool.h
        src/ui/ColorPickerWidget.h
        src/ui/BrushSizeWidget.h
        src/undo/UndoManager.h
        src/format/NyaFormat.h
        src/tools/LineTool.h
        src/tools/FillTool.h
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_link_libraries(${PROJECT_NAME} PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
)

# Копирование Qt DLL в папку сборки
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE:Qt6::Core>"
        "$<TARGET_FILE:Qt6::Gui>"
        "$<TARGET_FILE:Qt6::Widgets>"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
        COMMAND windeployqt --no-compiler-runtime "$<TARGET_FILE_DIR:${PROJECT_NAME}>/$<TARGET_FILE_NAME:${PROJECT_NAME}>"
)