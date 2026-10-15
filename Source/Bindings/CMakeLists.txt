# C# bindings of the C++ user code of the sample (namespace SampleProjectApi in Source/Application).
# The bindings library is a native library with generated wrapper code. The managed plugin references
# the generated managed assembly. The library type follows BUILD_SHARED_LIBS, so it equals the engine type.
# The user code is in the library App.Main. It is a shared library when the engine is shared (desktop and Android),
# and a static library when the engine is static (Web and iOS). The bindings library links it, so the wrapper
# and the engine use one copy of PlayerController.
rbfx_csharp_add_usercode (SampleProject.UserCode
    HEADERS "${CMAKE_SOURCE_DIR}/Source/Application/SampleProjectApi.h"
    INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/Source/Application"
    LINK_LIBRARIES ${APP_PLUGIN_NAME}
    NAMESPACE SampleProjectApi)

# The runtime loads the native library from the folder of the launcher. The launcher target is added
# later, so the folder comes from a generator expression. Web and iOS have no such folder: the launcher is an
# object library (Web) or a static library (iOS), and the .NET head links the static bindings.
if (NOT WEB AND NOT IOS)
    add_custom_command (TARGET SampleProject.UserCode POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different
            "$<TARGET_FILE:SampleProject.UserCode>" "$<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>"
        COMMENT "Copying the native library of the user code next to the launcher"
        VERBATIM)
endif ()
