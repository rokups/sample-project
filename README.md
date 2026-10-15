# Sample project

Simple example project that uses [the Framework](https://github.com/rbfx/rbfx).

It is automatically deployed to [itch.io](https://eugeneko.itch.io/sample-project) and it can be played in browser.

TODO: Expand this page and the sample.

## Building the Sample

Checkout and build this project like any other CMake project.

Provide `rbfx`:
- Build engine: provide `-DCMAKE_PREFIX_PATH=/path/to/rbfx/CMake`
- Use engine SDK:
  - Windows: provide `-DCMAKE_PREFIX_PATH=/path/to/rbfx-SDK/share`
  - Unix: provide `-DCMAKE_PREFIX_PATH=/path/to/rbfx-SDK`

### Optional .NET hosting (C# plugin)

Use `-DSAMPLE_PROJECT_CSHARP=ON` to start the .NET runtime in the launcher and load the C# plugin
`Source/Managed/SampleProject.Managed`. The default is `OFF`. Then the build does not need .NET.

- It needs the dotnet SDK. Desktop needs a shared (dynamic library) engine SDK. Web and iOS need a static engine SDK.
- It works on Linux, Windows, macOS, Android, Web and iOS (device, arm64). The configure step fails on other mobile platforms
  and on the iOS simulator.
- On Android, iOS and Web a .NET head is the entry and no hostfxr is used. The launcher is the same on all platforms.
  The Web build uses source mode with wasm managed assemblies. The head makes the web folder, and it packs
  `Data.pak` and `CoreData.pak`. The iOS build makes an app bundle and links the static bindings into it.
  See the README of SharpCpp.Rbfx for the setup of these two platforms. These two builds are not tested here.
- Put the engine and [SharpCpp.Rbfx](https://github.com/rbfx/SharpCpp.Rbfx) on `CMAKE_PREFIX_PATH`.
  Use one of these two forms:
  - SharpCpp.Rbfx SDK (made with `cmake --install`):
    `-DCMAKE_PREFIX_PATH="/path/to/rbfx-SDK;/path/to/sharpcpp-rbfx-SDK"`
  - SharpCpp.Rbfx source tree (it needs a SharpCpp checkout; set `SHARPCPP_ROOT` if it is not in `../SharpCpp`):
    `-DCMAKE_PREFIX_PATH="/path/to/rbfx-SDK;/path/to/SharpCpp.Rbfx/cmake/SharpCppRbfx"`
- `Source/Launcher/CMakeLists.txt` makes the launcher with one call to `rbfx_csharp_add_application()`.
  The target `SampleProject.CSharp` of that call builds the C# plugin and copies the native wrapper next to the launcher.
  It is part of the default build.
- `Source/Bindings/CMakeLists.txt` binds the C++ user code to C# with `rbfx_csharp_add_usercode()`.
  The bound types are `PlayerController` and `GameSaveData` in the C++ namespace `SampleProjectApi`
  (`Source/Application/SampleProjectApi.h`). The generator ignores other namespaces.
  The target `SampleProject.UserCode` links the library `App.Main`, so the wrapper and the engine use one copy of the code.
  The launcher call passes it with `USERCODE SampleProject.UserCode`.
  The plugin project references the generated bindings project (the MSBuild property `SampleProjectUserCodeProject`).
  The plugin creates a `PlayerController` and a `GameSaveData` and logs their values. The C++ plugin `App.Main` registers
  the factory of `PlayerController`. A second registration in C# fails with a duplicate reflection error.
  The native library `libSampleProject.UserCode.Native` is copied next to the launcher, and the runtime loads it from there.
  On Web and iOS there is no copy. The native library is static code in the head.
  The type of this library follows `BUILD_SHARED_LIBS`, so it is always equal to the type of the engine.
  The classes of `App.Main` use the export macro `SAMPLE_PROJECT_API`.
  The build of the bindings is tested on Linux with both forms above.
  CI of SharpCpp.Rbfx builds this sample against its SDK.

Check out `Scripts/` folder and `.github/workflows/deploy.yml` to see how this sample can be built.

## Running the Sample

Run Editor:

1) Launch `Editor.exe`;

2) Open `sample-project/Project` folder;

3) Open `Scenes/Scene.xml` in Editor;

4) Press `Ctrl+P` to play the sample.

Run Player:

Launch `Player.exe`

## Portability of Resource Access

The engine needs to somehow locate project resources on launch.

By default, the engine attempts to find `Data`, `Cache` and `CoreData` folders next to the executable.
However, this behavior may be inconvenient during development:
* `Data` and `Cache` folders are located in `Project` folder,
* `CoreData` is located in `bin` folder of `rbfx` repository,
* The executable is located somewhere in CMake build folder.

This issue can be mitigated by having `ResourceRoot.ini` file.
See the comments in `ResourceRoot.ini` file in this repository for details.

![](/screenshot.png)
