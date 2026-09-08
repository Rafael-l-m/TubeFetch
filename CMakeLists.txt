cmake_minimum_required(VERSION 3.16)

project(TubeFetch VERSION 0.1 LANGUAGES CXX)

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
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-fsanitize=address -fno-omit-frame-pointer)
    add_link_options(-fsanitize=address)
endif()

# include all .h files
include_directories(${CMAKE_SOURCE_DIR}/include)

qt_add_executable(TubeFetch
    main.cpp
)

qt_add_qml_module(TubeFetch
    URI TubeFetch
    QML_FILES
        Main.qml
        QML_FILES qml/components/LiquidGlassButton.qml
        QML_FILES qml/components/Background.qml
        QML_FILES qml/components/LiquidGlassSegmentedSelection.qml
        QML_FILES qml/components/LiquidGlassSwitch.qml
        QML_FILES qml/components/LiquidGlassTextField.qml
)

if (APPLE)
    # Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
    # If you are developing for iOS or macOS you should consider setting an
    # explicit, fixed bundle identifier manually though.
    set_target_properties(TubeFetch PROPERTIES
    #    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.TubeFetch
        MACOSX_BUNDLE_ICON_FILE AppIcon.icns
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE TRUE
    )

    add_custom_command(TARGET TubeFetch POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        "${CMAKE_SOURCE_DIR}/resources/AppIcon.icns"
        "$<TARGET_BUNDLE_DIR:TubeFetch>/Contents/Resources/AppIcon.icns"

        #COMMAND ${CMAKE_COMMAND} -E copy
        #"${CMAKE_SOURCE_DIR}/resources/HelpDoc_v2_1_0.pdf"
        #"$<TARGET_BUNDLE_DIR:TubeFetch>/Contents/Resources/HelpDoc_v2_1_0.pdf"

        #COMMAND ${CMAKE_COMMAND} -E copy_directory
        #${CMAKE_SOURCE_DIR}/resources/translations/ForMachine
        #$<TARGET_BUNDLE_DIR:TubeFetch>/Contents/Resources/translations
    )

elseif (WIN32)
    set_target_properties(TubeFetch PROPERTIES WIN32_EXECUTABLE TRUE)

    #set_source_files_properties(resources/app.rc PROPERTIES LANGUAGE RC)
    #target_sources(TubeFetch PRIVATE resources/app.rc)

    #add_custom_command(TARGET TubeFetch POST_BUILD
    #    COMMAND ${CMAKE_COMMAND} -E copy
    #    "${CMAKE_SOURCE_DIR}/resources/AppIcon.ico"
    #    "$<TARGET_FILE_DIR:TubeFetch>/resources/AppIcon.ico"

    #    COMMAND ${CMAKE_COMMAND} -E copy
    #    "${CMAKE_SOURCE_DIR}/resources/HelpDoc_v2_1_0.pdf"
    #    "$<TARGET_FILE_DIR:TubeFetch>/resources/HelpDoc_v2_1_0.pdf"

    #    COMMAND ${CMAKE_COMMAND} -E copy_directory
    #    ${CMAKE_SOURCE_DIR}/resources/translations/ForMachine
    #    $<TARGET_FILE_DIR:TubeFetch>/resources/translations
    #)

elseif (UNIX)
    set_target_properties(TubeFetch PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    )

    #add_custom_command(TARGET TubeFetch POST_BUILD
    #    COMMAND ${CMAKE_COMMAND} -E copy
    #    "${CMAKE_SOURCE_DIR}/resources/AppIcon.png"
    #    "$<TARGET_FILE_DIR:TubeFetch>/resources/AppIcon.png"

    #    COMMAND ${CMAKE_COMMAND} -E copy
    #    "${CMAKE_SOURCE_DIR}/resources/HelpDoc_v2_1_0.pdf"
    #    "$<TARGET_FILE_DIR:TubeFetch>/resources/HelpDoc_v2_1_0.pdf"

    #    COMMAND ${CMAKE_COMMAND} -E copy_directory
    #    ${CMAKE_SOURCE_DIR}/resources/translations/ForMachine
    #    $<TARGET_FILE_DIR:TubeFetch>/resources/translations
    #)

endif()

target_link_libraries(TubeFetch
    PRIVATE Qt6::Quick
    PRIVATE Qt6::QuickControls2
    PRIVATE Qt6::QuickDialogs2
    PRIVATE Qt6::Sql
)

install(TARGETS TubeFetch
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
