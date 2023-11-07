qt_add_library(
    SilentEdgeGUI 
    STATIC
    mainwindow.cpp
    mainwindow.hpp
    mainwindow.ui
)

target_link_libraries(SilentEdgeGUI PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Widgets)
target_include_directories(SilentEdgeGUI INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
