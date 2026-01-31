cmake_minimum_required(VERSION 3.16)

project(ScreenshotTranslation VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Multimedia)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(ScreenshotTranslation
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        ocr.h ocr.cpp
        autofontsizelabel.h autofontsizelabel.cpp autofontsizelabel.ui
        config.hpp
        screenshotwidget.h screenshotwidget.cpp
        translate.h translate.cpp
        common.hpp
        res.qrc
        pronounce.h pronounce.cpp
        word.hpp
    )
else()
    if(ANDROID)
        add_library(ScreenshotTranslation SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(ScreenshotTranslation
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(ScreenshotTranslation PRIVATE Qt${QT_VERSION_MAJOR}::Widgets  Qt${QT_VERSION_MAJOR}::Multimedia)

if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.ScreenshotTranslation)
endif()
set_target_properties(ScreenshotTranslation PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS ScreenshotTranslation
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(ScreenshotTranslation)
endif()




target_include_directories(ScreenshotTranslation PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/include)#指定头文件路径
target_link_directories(ScreenshotTranslation PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/lib/openssl)
target_link_directories(ScreenshotTranslation PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/lib/curl)
target_link_libraries(ScreenshotTranslation PRIVATE libcrypto libssl libcurl_imp)#链接库文件
















