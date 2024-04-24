# OpenGL Template
[![de](https://img.shields.io/badge/lang-de-red.svg)](README.de.md)

## About the framework
The OpenGL API is primarily written for use with C, which is why memory management is largely manual. C++ is based on C and allows pointers and structs to be replaced by objects and classes whose memory management is largely automatic. This framework uses the advanced features of C++ to free the code from error-prone manual pointer management. To this end, we apply the programming technique [RAII](https://de.wikipedia.org/wiki/Ressourcenbelegung_ist_Initialisierung) and encapsulate all GPU resources in C++ objects, which outsource the allocation and release of resources to the constructor/destructor.

The framework also provides functions for loading models, textures and shaders and outsources the tedious creation of windows and contexts to a higher-level app class. Your code can then simply inherit these from the app class.

However, the framework does not include a rendering pipeline, which you have to write yourself by overwriting the render loop `App::render()`. GUI elements can be specified in the function `App::buildImGui()`, assets are best loaded within the constructor and initial OpenGL configurations can be made in the function `App::init()`.

Assets and new code files are not automatically added to your project. In order for the build script to recognize new files, you must explicitly list them in [`CMakeLists.txt`](CMakeLists.txt). This behavior is desired so that CMake recognizes that you have made a change and reconfigures the build files.

For example, to add a texture `new_texture.png`, add in [`CMakeLists.txt`](CMakeLists.txt)

```CMake
...
set(TEXTURES
    textures/checker.png
    textures/checkerbw.png
)
...
```
to
```CMake
...
set(TEXTURES
    textures/checker.png
    textures/checkerbw.png
    textures/new_texture.png
)
...
```

The procedure for source files (`SRC`), models (`MESHES`) and shaders (`SHADERS`) is the same.

## Installation
For the installation you need:
* A C++ compiler
* [CMake](https://cmake.org)
* (optional: an IDE e.g. [VSCode](https://code.visualstudio.com) with [CMake Tools](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools) and the [C/C++ Extension Pack
](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools-extension-pack))

### Linux
In Linux you can get CMake, Clang and VSCode via your package manager:

```sh
sudo apt install clang 
sudo apt install cmake
sudo snap install --classic code
```

### macOS
On macOS you can install a C++ compiler together with the Xcode Command Line Tools:

```sh
xcode-select --install
```

Via [Homebrew](https://brew.sh) you can then install CMake and optionally VSCode:

```sh
brew install cmake
brew install visual-studio-code
```

### Windows
Under Windows you can install a C++ compiler e.g. with the [Visual Studio Build Tools](https://visualstudio.microsoft.com/downloads/#build-tools-for-visual-studio-2022) or via the [Visual Studio Installer](https://visualstudio.microsoft.com/downloads/#visual-studio-community-2022). Alternatively, installation is also possible via the Windows Package Manager [winget](https://learn.microsoft.com/de-de/windows/package-manager/winget/):

```sh
winget install Microsoft.VisualStudio.2022.BuildTools
winget install Kitware.CMake
winget install Microsoft.VisualStudioCode
```

## Dependencies
The framework uses and integrates the following libraries:
- [GLFW](https://github.com/glfw/glfw) for window management
- [Glad](https://github.com/Dav1dde/glad) for loading OpenGL
- [GLM](https://github.com/g-truc/glm) for vector math
- [Dear ImGui](https://github.com/ocornut/imgui) for GUI elements
- [tinyobjloader](https://github.com/tinyobjloader/tinyobjloader) for parsing .OBJ files
- [json](https://github.com/nlohmann/json) for parsing .JSON files
- [stb](https://github.com/nothings/stb) for parsing textures and images

## Execution

### Without IDE
First we have to execute our CMake script with the following command:

```sh
cmake -B build
```

With ``-B build`` we specify the folder in which CMake stores build files. CMake now downloads all the necessary libraries and configures a platform-dependent build system, e.g. using Makefiles, Ninja build files or Visual Studio or Xcode project files.

We can then execute this cross-platform build system with `cmake --build`:

```sh
cmake --build build
```

This command now generates executable files and stores them in the `build` folder, sometimes in a subfolder called `Debug` or `Release`. These folders separate different build variants, which can be selected with the `--config` parameter.
The execution of our program varies depending on the operating system.

### With VSCode
With VSCode it is sufficient to open the project folder and click on the Execute button in the lower status bar, [CMake Tools](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools) should automatically recognize the build script and in the default settings the build folder is already correctly set to `${workspaceFolder}/build`.

### Note
Execution may fail because shaders/models/textures could not be loaded. This error occurs if the working directory has not been set correctly and can be fixed by calling the program from the root of the project folder.

## Distribution
If you have finished a project and want to share it with others, the included CMake build script provides you with various tools to do so.

### Generating installers with CPack
In the simplest case, go to the `build` folder and execute the `cpack` command, which is already included in your CMake installation:

```sh
cd build
cpack
```

or alternatively call `cmake --build build --target package` in the root directory.

`cpack` generates a DragNDrop archive with an `.app` bundle under macOS and a zip folder with an `.exe` and all necessary resources under Windows. On Windows it is also possible to build an .EXE or .MSI installer, but this also requires [NSIS](http://nsis.sourceforge.net/) or [WiX Toolset](http://wixtoolset.org/).

By default `cpack` tries to pack the release version, you can change this with the parameter `-C Debug`.

### Pack source files
The source files are packed by default with every build. A zip file with your code but without temporary build files then appears in the build folder. This makes it easier for you to share your code with others.