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

- It needs the dotnet SDK, and the engine must be a shared (dynamic library) SDK.
- It works on Linux and Windows only. The configure step fails on Web, mobile and Apple platforms.
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
  The type of this library follows `BUILD_SHARED_LIBS`, so it is always equal to the type of the engine.
  The build of the bindings is tested on Linux with both forms above.
  The classes of `App.Main` have no export macros yet, so the bindings are not tested on Windows.

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
