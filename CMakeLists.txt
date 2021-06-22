cmake_minimum_required(VERSION 3.9)

project(QtViewer VERSION 0.0.1 DESCRIPTION "Viewer for MFSYS files")

set (CMAKE_CXX_STANDARD 11)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

set(SOURCES 
    main.cpp
    mygraphicsview.cpp
    mainwindow.cpp
    partgraphicsitem.cpp
    mygraphicsscene.cpp
    magneticcircle.cpp
    systemproperties.cpp
    statechangedialog.cpp)

set(HEADERS 
    mygraphicsview.h
    mainwindow.h
    partgraphicsitem.h
    mygraphicsscene.h
    magneticcircle.h
    systemproperties.h
    statechangedialog.h)

set(FORMS
    mainwindow.ui
    systemproperties.ui
    statechangedialog.ui)

set(RESOURCES
    icons.qrc)

set(OTHER_FILES 
    .gitignore)

include_directories("../partsEngine")
link_directories("../partsEngine")

add_executable(${PROJECT_NAME} ${FORMS} ${HEADERS} ${SOURCES} ${RESOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE partsEngine Qt::Core Qt::Gui Qt5::Widgets)

install(TARGETS QtViewer)
install(DIRECTORY deploy/examples DESTINATION examples)