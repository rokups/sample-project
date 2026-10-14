set (TARGET_NAME "${CMAKE_PROJECT_NAME}")

# Include source files.
set (SOURCE_FILES Launcher.cpp)

if (WIN32)
    if (UWP)
        add_definitions (-DUWP=1)

        # Configure custom manifest
        set(APP_MANIFEST_NAME package.appxManifest)
        configure_file(
            ${CMAKE_SOURCE_DIR}/StoreArt/UWP/${APP_MANIFEST_NAME}.in
            ${CMAKE_CURRENT_BINARY_DIR}/${APP_MANIFEST_NAME}
            @ONLY)

        set(CONTENT_FILES "${CMAKE_CURRENT_BINARY_DIR}/${APP_MANIFEST_NAME}")

        file(GLOB_RECURSE ASSET_FILES "${CMAKE_SOURCE_DIR}/StoreArt/UWP/*.png")

        # Configure custom icons
        set_property(SOURCE ${ASSET_FILES} PROPERTY VS_DEPLOYMENT_CONTENT 1)
        set_property(SOURCE ${ASSET_FILES} PROPERTY VS_DEPLOYMENT_LOCATION "Assets")
        source_group("Resource Files" FILES ${ASSET_FILES})

        set_property(SOURCE ${CONTENT_FILES} PROPERTY VS_DEPLOYMENT_CONTENT 1)

        set(RESOURCE_FILES
          ${CONTENT_FILES} ${ASSET_FILES})

    else()
        set(RESOURCE_FILES "${CMAKE_SOURCE_DIR}/StoreArt/Windows/appicon.rc")
    endif ()

endif()

# Add launcher target.
if (SAMPLE_PROJECT_CSHARP)
    # The launcher hosts .NET and loads the C# plugin (optional). One call makes the executable and
    # the C# parts. The plugin is built with dotnet next to the launcher.
    # The bindings of the user code (Source/Bindings) are referenced by the plugin project. The property
    # is the generated managed project of the bindings.
    get_target_property (USERCODE_PROJECT SampleProject.UserCode SHARPCPP_RBFX_MANAGED_PROJECT)
    rbfx_csharp_add_application (${TARGET_NAME}
        SOURCES ${SOURCE_FILES} ${RESOURCE_FILES}
        PLUGIN "${CMAKE_SOURCE_DIR}/Source/Managed/SampleProject.Managed/SampleProject.Managed.csproj"
        PROPERTIES "SampleProjectUserCodeProject=${USERCODE_PROJECT}"
        USERCODE SampleProject.UserCode
        LINK_LIBRARIES PlayerLibrary
        # Android packs the resources into the APK. The top-level CMakeLists.txt makes the paks.
        RESOURCES "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Data.pak" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/CoreData.pak")
    target_compile_definitions (${TARGET_NAME} PRIVATE SAMPLE_PROJECT_CSHARP=1)
else ()
    if (ANDROID)
        add_library(${TARGET_NAME} MODULE ${SOURCE_FILES} ${RESOURCE_FILES})
    else ()
        add_executable(${TARGET_NAME} WIN32 MACOSX_BUNDLE ${SOURCE_FILES} ${RESOURCE_FILES})
    endif ()

    # Link launcher with the engine.
    target_link_libraries (${TARGET_NAME} PRIVATE Urho3D PlayerLibrary)
endif ()

# Link launcher with plugins.
target_link_plugins (${TARGET_NAME} "${APP_PLUGIN_LIST}")

if (UWP)
    set_property(TARGET ${TARGET_NAME} PROPERTY VS_DESKTOP_EXTENSIONS_VERSION "${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION}")

    # Add content folders (Data and CoreData) to the UWP project
    foreach (subfolder Data CoreData)
        file(GLOB_RECURSE DATA_FILES RELATIVE "${CMAKE_SOURCE_DIR}/Project" "${CMAKE_SOURCE_DIR}/Project/${subfolder}/**")
        foreach (filename ${DATA_FILES})
            set (filepath "${CMAKE_SOURCE_DIR}/Project/${filename}")
            get_filename_component(dirname "${filename}" DIRECTORY)
            # Include file in UWP app.
            set_property(SOURCE ${filepath} PROPERTY VS_DEPLOYMENT_CONTENT 1)
            # Set file path by which UWP app will access it.
            set_property(SOURCE ${filepath} PROPERTY VS_DEPLOYMENT_LOCATION "${dirname}")
            # Ensure file VS wont try to build resources.
            set_property(SOURCE ${filepath} PROPERTY VS_TOOL_OVERRIDE None)
            # Include file under "Resource Files" in VS project.
            source_group("Resource Files/${dirname}" FILES "${filepath}")
            # Consume file. Without this resources are just ignored.
            target_sources(${TARGET_NAME} PRIVATE "${filepath}")
        endforeach ()
    endforeach ()
endif()
