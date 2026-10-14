cmake_minimum_required(VERSION 3.14)
project (SampleProject)

set (PROJECT_UWP_GUID "D84454DA-854B-3661-9EDA-0B587569493A")

# C++17 is required for this project.
set (CMAKE_CXX_STANDARD 17)

# Setup output directories.
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Configure engine options. This is required in-source engine builds.
set(URHO3D_SAMPLES OFF)

# Engine must be available on CMAKE_PREFIX_PATH:
# Option 1 - prebuilt SDK:
#   - Download a prebuilt SDK and extract it somewhere
#   - Configure with `-DCMAKE_PREFIX_PATH=/path/to/your/sdk -DCMAKE_CONFIGURATION_TYPES="Debug;RelWithDebInfo"`
#     - CMAKE_CONFIGURATION_TYPES may or may not be necessary, depending on your SDK configuration
# Option 2 - in-source build:
#   - Clone https://github.com/rbfx/rbfx somewhere
#   - Configure with `-DCMAKE_PREFIX_PATH=/path/to/rbfx/CMake`
find_package(Urho3D REQUIRED)

# Optional .NET hosting. The launcher starts the .NET runtime and loads the C# plugin Source/Managed/SampleProject.Net.
# It needs the dotnet SDK, and SharpCpp.Rbfx on CMAKE_PREFIX_PATH in addition to the engine (see README.md).
# Desktop needs a shared engine SDK. Web and iOS need a static engine SDK.
option(SAMPLE_PROJECT_CSHARP "Host .NET and load a C# plugin" OFF)
if (SAMPLE_PROJECT_CSHARP)
    if (MOBILE AND NOT ANDROID AND NOT IOS)
        message(FATAL_ERROR "SAMPLE_PROJECT_CSHARP is for Linux, Windows, macOS, Android, Web and iOS only.")
    endif ()
    find_package(SharpCppRbfx CONFIG REQUIRED)
    if (SharpCppRbfx_IS_SDK AND NOT SharpCppRbfx_MANAGED_DIR)
        message(FATAL_ERROR "The SharpCpp.Rbfx SDK has no managed assemblies. Use an SDK with managed files.")
    endif ()
endif ()

# Check if Core.SamplePlugin exists
if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/Plugins/Core.SamplePlugin/CMakeLists.txt")
    message(FATAL_ERROR
        "Missing required file: ${CMAKE_CURRENT_SOURCE_DIR}/Plugins/Core.SamplePlugin/CMakeLists.txt\n"
        "It looks like git submodules are not initialized.\n"
        "Please run the following command to fetch them:\n"
        "  git submodule update --init --recursive\n"
    )
endif()

# Add plugins (optional).
include_directories (${CMAKE_SOURCE_DIR}/Plugins)
add_subdirectory (${CMAKE_SOURCE_DIR}/Plugins/Core.SamplePlugin)

# List all plugins used by the application.
set (APP_PLUGIN_NAME "App.Main")
set (APP_PLUGIN_LIST "Plugin.Core.SamplePlugin;${APP_PLUGIN_NAME}")

# Add main application library with all user code.
add_subdirectory (${CMAKE_SOURCE_DIR}/Source/Application)

# Add custom editor linked with user code.
# With dynamic linking, pre-built editor can be used.
add_subdirectory (${CMAKE_SOURCE_DIR}/Source/Editor)

# Add custom player.
add_subdirectory (${CMAKE_SOURCE_DIR}/Source/Launcher)

# Package resources from Project directory.
# With C#, rbfx_csharp_add_project packs the data.
if ((WEB OR MOBILE) AND NOT SAMPLE_PROJECT_CSHARP)
    create_pak("${CMAKE_SOURCE_DIR}/Project/Data"    "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Data.pak")
    # Some mobile SDKs have no CoreData. Then use the folder of another SDK on CMAKE_PREFIX_PATH (the host SDK).
    set (SAMPLE_CORE_DATA "${Urho3D_PACKAGE_ROOT}/bin/CoreData")
    if (NOT EXISTS "${SAMPLE_CORE_DATA}")
        find_path (SAMPLE_CORE_DATA_ROOT bin/CoreData PATHS ${CMAKE_PREFIX_PATH} NO_DEFAULT_PATH NO_CMAKE_FIND_ROOT_PATH)
        if (SAMPLE_CORE_DATA_ROOT)
            set (SAMPLE_CORE_DATA "${SAMPLE_CORE_DATA_ROOT}/bin/CoreData")
        endif ()
    endif ()
    create_pak("${SAMPLE_CORE_DATA}" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/CoreData.pak")
endif ()

# Link resources to Player for Web. With C#, ${CMAKE_PROJECT_NAME} is an object library and the .NET head
# packs the resources (the DATA keyword in Source/Launcher/CMakeLists.txt).
if (WEB AND NOT SAMPLE_PROJECT_CSHARP)
    web_executable (${CMAKE_PROJECT_NAME})
    package_resources_web(
        FILES        "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Data.pak"
                     "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/CoreData.pak"
        RELATIVE_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
        OUTPUT       "Resources.js"
        INSTALL_TO   "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
    )
    web_link_resources(${CMAKE_PROJECT_NAME} Resources.js)
    target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE "--shell-file ${Urho3D_PACKAGE_ROOT}/bin/shell.html")
endif ()

# Generate Visual Studio props file for csproj.
rbfx_configure_cmake_props()

# Install the project outputs consumed by CI artifact uploads and host-side
# cross-build steps. The engine/SDK remains a separate caller-provided prefix.
# With C#, rbfx_csharp_add_project installs the application (INSTALL keyword).
set (INSTALL_TARGETS ${CMAKE_PROJECT_NAME}Editor)
if (NOT SAMPLE_PROJECT_CSHARP)
    list (PREPEND INSTALL_TARGETS ${CMAKE_PROJECT_NAME})
endif ()
foreach (TARGET
    ${INSTALL_TARGETS}
    ${APP_PLUGIN_NAME}
    Plugin.Core.SamplePlugin)
    # An object library (the launcher on Web with C#) has no file to install. The .NET head makes the web folder.
    set (TARGET_TYPE "")
    if (TARGET ${TARGET})
        get_target_property (TARGET_TYPE ${TARGET} TYPE)
    endif ()
    if (TARGET ${TARGET} AND NOT TARGET_TYPE STREQUAL "OBJECT_LIBRARY")
        install(TARGETS ${TARGET}
            RUNTIME DESTINATION bin/$<CONFIG>
            LIBRARY DESTINATION bin/$<CONFIG>
            ARCHIVE DESTINATION lib/$<CONFIG>
            BUNDLE DESTINATION bin/$<CONFIG>)
    endif ()
endforeach ()
install(DIRECTORY "${CMAKE_SOURCE_DIR}/Project/" DESTINATION share/${CMAKE_PROJECT_NAME})
