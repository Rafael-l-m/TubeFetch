cmake_minimum_required(VERSION 4.1)
project(YoutubeMediaDownloader)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Configure Qt Path
if (WIN32)
    set(CMAKE_PREFIX_PATH "D:/Applications/Qt/6.10.2/mingw_64")
elseif (UNIX AND NOT APPLE)
    set(CMAKE_PREFIX_PATH "/home/rafael/Applications/Qt/6.9/6.9.1/gcc_64")
elseif (APPLE)
    set(CMAKE_PREFIX_PATH "/Users/aguam/Applications/Qt/6.11.1/macos")
endif ()

include_directories(${CMAKE_SOURCE_DIR}/include)  # include all .h files

find_package(Qt6 COMPONENTS
        Core
        Gui
        Network
        Sql
        Widgets
        REQUIRED)

add_executable(YoutubeMediaDownloader main.cpp
        include/DataBaseManager.h
        src/DataBaseManager.cpp
        include/DownloadInfoWidget.h
        src/DownloadInfoWidget.cpp
        include/DownloadItemWidget.h
        src/DownloadItemWidget.cpp
        include/DownloadManager.h
        src/DownloadManager.cpp
        include/MainWindow.h
        src/MainWindow.cpp
        include/NotificationWindow.h
        src/NotificationWindow.cpp
        include/PathManager.h
        src/PathManager.cpp
        include/ShowCodes.h
        src/ShowCodes.cpp
        include/ToolsWindow.h
        src/ToolsWindow.cpp
        include/URLManager.h
        src/URLManager.cpp)

target_link_libraries(YoutubeMediaDownloader
        Qt::Core
        Qt::Gui
        Qt::Network
        Qt::Sql
        Qt::Widgets
)

# Platform-specific
if (WIN32)
    set_target_properties(YoutubeMediaDownloader PROPERTIES WIN32_EXECUTABLE TRUE)

    add_custom_command(TARGET YoutubeMediaDownloader POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/translations
            $<TARGET_FILE_DIR:YoutubeMediaDownloader>/translations
    )

    add_custom_command(TARGET YoutubeMediaDownloader POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/resources/icons
            $<TARGET_FILE_DIR:YoutubeMediaDownloader>/resources/icons
    )

    if (NOT DEFINED CMAKE_TOOLCHAIN_FILE)
        set(DEBUG_SUFFIX)
        if (MSVC AND CMAKE_BUILD_TYPE MATCHES "Debug")
            set(DEBUG_SUFFIX "d")
        endif ()
        set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
        if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
            if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
                set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
            endif ()
        endif ()
        if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
            add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E make_directory
                    "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
            add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy
                    "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                    "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
        endif ()
        foreach (QT_LIB Core Gui Widgets)
            add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy
                    "${QT_INSTALL_PATH}/bin/Qt6${QT_LIB}${DEBUG_SUFFIX}.dll"
                    "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
        endforeach (QT_LIB)
    endif ()

elseif (APPLE)
    set_target_properties(YoutubeMediaDownloader PROPERTIES
            MACOSX_BUNDLE TRUE
            MACOSX_BUNDLE_ICON_FILE AppIcon.icns
    )

    add_custom_command(TARGET YoutubeMediaDownloader POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/translations
            $<TARGET_BUNDLE_DIR:YoutubeMediaDownloader>/Contents/Resources/translations
    )

    add_custom_command(TARGET YoutubeMediaDownloader POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/resources/icons
            $<TARGET_BUNDLE_DIR:YoutubeMediaDownloader>/Contents/Resources/icons
    )

    add_custom_command(TARGET YoutubeMediaDownloader POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_SOURCE_DIR}/resources/icons/AppIcon.icns
            $<TARGET_BUNDLE_DIR:YoutubeMediaDownloader>/Contents/Resources/AppIcon.icns
    )
endif ()
