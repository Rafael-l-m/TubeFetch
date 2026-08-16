cmake_minimum_required(VERSION 3.16)

project(YoutubeMediaDownloader VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS
    Quick
    QuickControls2
    QuickDialogs2
    Sql
)

qt_standard_project_setup(REQUIRES 6.10)

# Content of downloaded design from Figma to Qt or similar
if (EXISTS "${CMAKE_CURRENT_LIST_DIR}/importedcontent/CMakeLists.txt")
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/importedcontent)
endif()

# Sanitizer -> stderr
# if(CMAKE_BUILD_TYPE STREQUAL "Debug")
#    add_compile_options(-fsanitize=address -fno-omit-frame-pointer)
#    add_link_options(-fsanitize=address)
# endif()

# include all .h files
include_directories(${CMAKE_SOURCE_DIR}/include)

qt_add_executable(appYoutubeMediaDownloader
    main.cpp
)

qt_add_qml_module(appYoutubeMediaDownloader
    URI YoutubeMediaDownloader
    QML_FILES
        Main.qml
        QML_FILES qml/components/P_Background_Rectangle.qml
        QML_FILES qml/components/P_Button.qml
        QML_FILES qml/components/P_ComboBox.qml
        QML_FILES qml/components/P_GroupBox_Label.qml
        QML_FILES qml/components/P_Label.qml
        QML_FILES qml/components/P_LoadingOverlay.qml
        QML_FILES qml/components/P_MarqueeText.qml
        QML_FILES qml/components/P_MenuBarItem.qml
        QML_FILES qml/components/P_MessageDialog.qml
        QML_FILES qml/components/P_TerminalView.qml
        QML_FILES qml/components/P_TextField.qml
        QML_FILES qml/components/P_Toast.qml
        QML_FILES qml/download/DownloadDelegate.qml
        QML_FILES qml/download/DownloadList.qml
        QML_FILES qml/pages/AddNewDownloadPage.qml
        QML_FILES qml/pages/addnewdownloadpage/GroupBox_Config.qml
        QML_FILES qml/pages/addnewdownloadpage/GroupBox_Output.qml
        QML_FILES qml/pages/addnewdownloadpage/GroupBox_URL.qml
        QML_FILES qml/pages/MainPage.qml
        QML_FILES qml/pages/mainpage/ButtonLayout_Control.qml
        QML_FILES qml/pages/mainpage/ButtonLayout_UI.qml
        QML_FILES qml/pages/mainpage/MainPage_FileMenu.qml
        QML_FILES qml/pages/mainpage/MainPage_HelpMenu.qml
        QML_FILES qml/pages/mainpage/MainPage_LanguageMenu.qml
        QML_FILES qml/pages/mainpage/MainPage_MenuBar.qml
        QML_FILES qml/pages/mainpage/MainPage_MenuBarItem.qml
        QML_FILES qml/pages/mainpage/MainPage_MenuItem.qml
        QML_FILES qml/pages/mainpage/MainPage_OrganizeMenu.qml
        QML_FILES qml/pages/mainpage/MainPage_PreferencesMenu.qml
        QML_FILES qml/window/AddNewDownloadWindow.qml
        QML_FILES qml/window/preferences/ToolsWindow.qml
        QML_FILES qml/window/ShowItagsWindow.qml
        SOURCES include/Backend.hpp
        SOURCES include/core/APP.hpp
        SOURCES include/core/BasicTools.hpp
        SOURCES include/core/Constants.hpp
        SOURCES include/core/SingleApplication.hpp
        SOURCES include/db/DataBase.hpp
        SOURCES include/db/DbClient.hpp
        SOURCES include/db/DbManager.hpp
        SOURCES include/db/DbRequest.hpp
        SOURCES include/db/DbServer.hpp
        SOURCES include/db/Operation.hpp
        SOURCES include/db/Specification.hpp
        SOURCES include/download/Download.hpp
        SOURCES include/download/DownloadManager.hpp
        SOURCES include/download/DownloadModel.hpp
        SOURCES include/download/DownloadStatus.hpp
        SOURCES include/download/DownloadWorker.hpp
        SOURCES include/download/ProcessPool.hpp
        SOURCES include/LanguageManager.hpp
        SOURCES include/MessageCenter.hpp
        SOURCES include/SharedStorage.hpp
        SOURCES include/tools/ToolsManager.hpp
        SOURCES include/update/UpdateChecker.hpp
        SOURCES include/url/URLInfoManager.hpp
        SOURCES include/WindowManager.hpp
        SOURCES src/Backend.cpp
        SOURCES src/core/SingleApplication.cpp
        SOURCES src/db/DataBase.cpp
        SOURCES src/db/DbClient.cpp
        SOURCES src/db/DbManager.cpp
        SOURCES src/db/DbServer.cpp
        SOURCES src/download/DownloadManager.cpp
        SOURCES src/download/DownloadModel.cpp
        SOURCES src/download/DownloadWorker.cpp
        SOURCES src/download/ProcessPool.cpp
        SOURCES src/LanguageManager.cpp
        SOURCES src/update/UpdateChecker.cpp
        SOURCES src/url/URLInfoManager.cpp
        SOURCES src/WindowManager.cpp
)

if (APPLE)
    # Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
    # If you are developing for iOS or macOS you should consider setting an
    # explicit, fixed bundle identifier manually though.
    set_target_properties(appYoutubeMediaDownloader PROPERTIES
    #    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appYoutubeMediaDownloader
        MACOSX_BUNDLE_ICON_FILE AppIcon.icns
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE TRUE
        WIN32_EXECUTABLE TRUE
    )

    add_custom_command(TARGET appYoutubeMediaDownloader POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        "${CMAKE_SOURCE_DIR}/resources/AppIcon.icns"
        "$<TARGET_BUNDLE_DIR:appYoutubeMediaDownloader>/Contents/Resources/AppIcon.icns"

        COMMAND ${CMAKE_COMMAND} -E copy
        "${CMAKE_SOURCE_DIR}/resources/HelpDoc_v2.pdf"
        "$<TARGET_BUNDLE_DIR:appYoutubeMediaDownloader>/Contents/Resources/HelpDoc_v2.pdf"

        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/resources/translations/ForMachine
        $<TARGET_BUNDLE_DIR:appYoutubeMediaDownloader>/Contents/Resources/translations
    )
endif()

target_link_libraries(appYoutubeMediaDownloader
    PRIVATE Qt6::Quick
    PRIVATE Qt6::QuickControls2
    PRIVATE Qt6::QuickDialogs2
    PRIVATE Qt6::Sql
)

install(TARGETS appYoutubeMediaDownloader
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
