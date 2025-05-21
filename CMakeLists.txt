cmake_minimum_required(VERSION 3.16)

project(SegmentedDeque VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# Статическая библиотека для моделей
add_library(Models STATIC
    models/inc/arraySequence.hpp
    models/impl/arraySequence.tpp
    models/inc/dynamicArray.hpp
    models/impl/dynamicArray.tpp
    models/inc/linkedList.hpp
    models/impl/linkedList.tpp
    models/inc/listSequence.hpp
    models/impl/listSequence.tpp
    models/inc/segmentedDeque.hpp
    models/impl/segmentedDeque.tpp
    models/inc/sequence.hpp
    models/types/complex.hpp
    models/types/person.hpp
)

target_include_directories(Models PUBLIC
    ${CMAKE_SOURCE_DIR}/models/inc
    ${CMAKE_SOURCE_DIR}/models/types
)

# Исходные файлы приложения
set(PROJECT_SOURCES
    main.cpp
    views/mainwindow.cpp
    views/mainwindow.h
    views/mainwindow.ui
    views/createSequenceDialog.cpp
    views/createSequenceDialog.h
    views/operationdialog.cpp
    views/operationdialog.h
    controllers/MainController.cpp
    controllers/MainController.hpp
)

# Создание исполняемого файла
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(SegmentedDeque
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(SegmentedDeque SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(SegmentedDeque
            ${PROJECT_SOURCES}
        )
    endif()
endif()

# Линковка библиотек
target_link_libraries(SegmentedDeque PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Models
)

# Настройка директорий включения для приложения
target_include_directories(SegmentedDeque PRIVATE
    ${CMAKE_SOURCE_DIR}/views
    ${CMAKE_SOURCE_DIR}/controllers
    ${CMAKE_SOURCE_DIR}/models/inc
    ${CMAKE_SOURCE_DIR}/models/types
)

# Настройки для macOS и Windows
if(${QT_VERSION} VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.SegmentedDeque)
endif()

set_target_properties(SegmentedDeque PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# Установка
include(GNUInstallDirs)
install(TARGETS SegmentedDeque Models
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Финализация для Qt6
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(SegmentedDeque)
endif()
