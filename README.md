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
`Source/Managed/SampleProject.Net`. The default is `OFF`. Then the build does not need .NET.

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
- `Source/Launcher/CMakeLists.txt` makes the launcher and all C# parts with one call to `rbfx_csharp_add_project()`.
  The call binds the C++ user code to C# (`HEADERS`, `IMPLEMENTATION`, `NAMESPACE`), builds the C# plugin, links the
  C++ plugins (`PLUGINS`), packs the data on Android, iOS and Web (`DATA`) and adds the install rules (`INSTALL`).
  The native library of the user code and the plugin are next to the launcher.
- The bound types are `PlayerController` and `GameSaveData` in the C++ namespace `SampleProjectApi`
  (`Source/Application/SampleProjectApi.h`). The generator ignores other namespaces.
  The plugin creates a `PlayerController` and a `GameSaveData` and logs their values. The C++ plugin `App.Main` registers
  the factory of `PlayerController`. A second registration in C# fails with a duplicate reflection error.
  The classes of `App.Main` use the export macro `SAMPLE_PROJECT_API`.
- The plugin project is generated from the folder `Source/Managed/SampleProject.Net`.
  To use an own project file, pass `CSPROJ` to the call.
  CI of SharpCpp.Rbfx builds this sample against its SDK.

### CMake presets

`CMakePresets.json` has presets for the platforms of the sample. The preset names follow the names of the rbfx presets.

- Configure presets: `windows-msvc-x64-{lib,dll}`, `linux-{gcc,clang}-x64-{lib,dll}`, `macos-clang-{x64,arm64}-{lib,dll}`,
  `android-clang-{arm,arm64,x64}-dll`, `web-emscripten-wasm-lib` and `ios-clang-arm64-lib`.
  `lib` is a static engine SDK. `dll` is a shared engine SDK.
- Build presets: the configure preset name plus `-debug` or `-release`.
- `cmake --list-presets` shows the presets that the host can use.
- The build directory is `cmake-build/<preset>`.

The presets do not contain paths. Set these environment variables before the configure step:

- `RBFX_SDK_ROOT`: the engine SDK (the folder with `share/Urho3D`). It must match the preset (`lib` or `dll`, and the platform).
- `SHARPCPP_SDK_ROOT`: the SharpCpp.Rbfx SDK or `cmake/SharpCppRbfx` of a source tree. It is only for C# hosting.
- `RBFX_HOST_SDK_ROOT`: the SDK for the host. Android presets use it.
- `ANDROID_NDK_HOME` for Android presets and `EMSDK` for the Web preset.

The presets set `CMAKE_PREFIX_PATH` to `$RBFX_SDK_ROOT;$SHARPCPP_SDK_ROOT`. To use other paths, pass `-DCMAKE_PREFIX_PATH=...`.
The Web and iOS presets also set `CMAKE_FIND_ROOT_PATH`.

```
export RBFX_SDK_ROOT=/path/to/rbfx-SDK
cmake --preset linux-gcc-x64-dll
cmake --build --preset linux-gcc-x64-dll-release
```

C# hosting is off in the presets. Add `-DSAMPLE_PROJECT_CSHARP=ON` to the configure step to switch it on.
The Windows presets use the generator `Visual Studio 18 2026`. Pass `-G "Visual Studio 17 2022"` for an older Visual Studio.

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
