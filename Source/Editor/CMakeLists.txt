if (NOT TARGET EditorLibrary)
    return ()
endif ()

if (NOT URHO3D_WIN32_CONSOLE)
    set (TARGET_TYPE WIN32)
endif ()
set (TARGET_NAME "${CMAKE_PROJECT_NAME}Editor")
add_executable (${TARGET_NAME} ${TARGET_TYPE} Editor.cpp)
target_link_libraries (${TARGET_NAME} PUBLIC EditorLibrary)
target_link_plugins (${TARGET_NAME} "${APP_PLUGIN_LIST}")

if (SAMPLE_PROJECT_CSHARP)
    # Host .NET in the editor. The editor includes RbfxCSharpEditorHost.h and starts the runtime (see Editor.cpp).
    target_compile_definitions (${TARGET_NAME} PRIVATE SAMPLE_PROJECT_CSHARP=1)
    # LAUNCHER registers the managed plugin of the launcher, so that the editor shows the C# components (for example
    # Greeter). The launcher target must exist, so Source/Launcher comes before Source/Editor in the root CMakeLists.txt.
    rbfx_csharp_host_editor (${TARGET_NAME} LAUNCHER ${CMAKE_PROJECT_NAME})
endif ()
