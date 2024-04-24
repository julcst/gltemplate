# OpenGL Template
[![en](https://img.shields.io/badge/lang-en-red.svg)](README.md)

## Über das Framework

Die OpenGL-API ist primär für die Benutzung mit C geschrieben, deswegen ist die Speicherverwaltung weitesgehend manuell zu erledigen. C++ baut auf C auf und erlaubt es Pointer und Structs durch Objekte und Klassen zu ersetzen, deren Speicherverwaltung weitestgehend automatisch abläuft. Dieses Framework benutzt die erweiterten Features von C++, um den Code von fehleranfälliger manueller Pointerverwaltung zu befreien. Dazu wenden wir die Programmiertechnik [RAII](https://de.wikipedia.org/wiki/Ressourcenbelegung_ist_Initialisierung) an und kapseln alle GPU-Ressourcen in C++-Objekte, die Allokation und Freigabe von Ressourcen in den Konstruktor/Destruktor auslagern.

Außerdem stellt das Framework Funktionen zum Laden von Modellen, Texturen und Shadern bereit und lagert die mühsame Erstellung von Fenstern und Kontexten in eine übergeordnete App-Klasse aus. Euer Code kann diese dann einfach von der App-Klasse erben.

Das Framework umfasst jedoch keine Rendering-Pipeline, diese müsst ihr selber einbauen indem ihr die Renderschleife `App::render()` überschreibt. GUI-Elemente könnt ihr in der Funktion `App::buildImGui()` spezifiziern, Assets ladet ihr am besten innerhalb des Konstruktors und initiale OpenGL Konfigurationen könnt ihr in der Funktion `App::init()` vornehmen.

Assets und neue Codedateien werden nicht automatisch zu eurem Projekt hinzugefügt. Damit das Buildskript neue Dateien erkennt müsst ihr diese in [`CMakeLists.txt`](CMakeLists.txt) explizit auflisten. Dieses Verhalten ist erwünscht, damit CMake erkennt, dass ihr eine Änderung vorgenommen habt, und die Builddateien neu konfiguriert.

Um beispielsweise eine Textur `new_texture.png` hinzuzufügen, ergänzt ihr in [`CMakeLists.txt`](CMakeLists.txt)

```CMake
...
set(TEXTURES
    textures/checker.png
    textures/checkerbw.png
)
...
```

zu

```CMake
...
set(TEXTURES
    textures/checker.png
    textures/checkerbw.png
    textures/new_texture.png
)
...
```

Das Vorgehen für Quelldateien (`SRC`), Modelle (`MESHES`) und Shader (`SHADERS`) erfolgt analog.

## Installation

Zur Installation benötigt ihr:

* Einen C++ Compiler
* [CMake](https://cmake.org)
* (optional: eine IDE z.B. [VSCode](https://code.visualstudio.com) mit [CMake Tools](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools) und dem [C/C++ Extension Pack
](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools-extension-pack))

### Linux

In Linux könnt ihr CMake, Clang und VSCode über euren Package-Manager beziehen:

```sh
sudo apt install clang 
sudo apt install cmake
sudo snap install --classic code
```

### macOS

Unter macOS könnt ihr einen C++ Compiler zusammen mit den Xcode Command Line Tools installieren:

```sh
xcode-select --install
```

Über [Homebrew](https://brew.sh) könnt ihr dann noch CMake und optional VSCode installieren:

```sh
brew install cmake
brew install visual-studio-code
```

### Windows

Unter Windows könnt ihr einen C++ Compiler z.B. mit den [Visual Studio Build Tools](https://visualstudio.microsoft.com/downloads/#build-tools-for-visual-studio-2022) installieren oder über den [Visual Studio Installer](https://visualstudio.microsoft.com/downloads/#visual-studio-community-2022). Alternativ ist die Installation auch über den Windows-Package-Manager [winget](https://learn.microsoft.com/de-de/windows/package-manager/winget/) möglich:

```sh
winget install Microsoft.VisualStudio.2022.BuildTools
winget install Kitware.CMake
winget install Microsoft.VisualStudioCode
```

## Dependencies

Das Framework benutzt und bindet folgende Bibliotheken ein:

- [GLFW](https://github.com/glfw/glfw) für die Fensterverwaltung
- [Glad](https://github.com/Dav1dde/glad) zum Laden von OpenGL
- [GLM](https://github.com/g-truc/glm) für Vektor-mathematik
- [Dear ImGui](https://github.com/ocornut/imgui) für GUI-Elemente
- [tinyobjloader](https://github.com/tinyobjloader/tinyobjloader) zum Parsen von .OBJ-Dateien
- [json](https://github.com/nlohmann/json) zum Parsen von .JSON-Datein
- [stb](https://github.com/nothings/stb) zum Parsen von Texturen und Bildern

## Ausführung

### Ohne IDE

Zuerst müssen wir unser CMake-Skript mit folgenedem Befehl ausführen:

```sh
cmake -B build
```

Mit `-B build` spezifizieren wir den Ordner, in dem CMake Builddateien ablegt. CMake lädt nun alle notwendigen Bibliotheken herunter und konfiguriert ein platformabhängiges Buildsystem, z.B. durch Makefiles, Ninja Builddateien oder Visual Studio bzw. Xcode Projektdateien.

Danach können wir dieses Buildsystem plattformübergreifenden mit `cmake --build` ausführen:

```sh
cmake --build build
```

Dieser Befehl generiert jetzt ausführbare Dateien und legt diese im `build`-Ordner ab, manchmal noch in einem Unterordner mit dem Namen `Debug` oder `Release`. Diese Ordner trennen verschiedene Buildvarianten, die mit dem Parameter `--config` ausgewählt werden können.

Die Ausführung unseres Programms variert je nach Betriebssystem.

### Mit VSCode

Mit VSCode reicht es den Projekt-Ordner zu öffnen und auf den Ausführen-Button in der unteren Statusleiste zu klicken, [CMake Tools](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools) sollte das Buildskript automatisch erkennen und in den Standardeinstellungen ist der Build-Ordner bereits richtig auf `${workspaceFolder}/build` gesetzt.

### Anmerkung
Es kann sein, dass die Ausführung fehlschlägt, weil Shader/Modelle/Texturen nicht geladen werden konnten. Dieser Fehler tritt auf, wenn das Arbeitzverzeichnis nicht richtig gesetzt wurde und kann behoben werden, indem man das Programm von der Wurzel des Projektordners aus aufruft.

## Verteilung

Wenn ihr ein Projekt beendet habt und es mit anderen Teilen wollt, stellt das beigelegte CMake-Buildskript euch dafür verschiedene Tools zur Hand.

### Generierung von Installern mit CPack

Im einfachsten Fall wechselt ihr dafür in den `build` Ordner und führt dort den Befehl `cpack` aus, der bereits in eurer CMake-Installation enthalten ist:

```sh
cd build
cpack
```

oder ihr ruft im Wurzelverzeichnis `cmake --build build --target package` auf.

`cpack` generiert unter macOS ein DragNDrop-Archiv mit einem `.app` Bundle und unter Windows einen Zip-Ordner mit einer `.exe` und allen notwendigen Ressourcen. Auf Windows besteht dazu auch noch die Möglichkeit einen .EXE oder .MSI Installer zu bauen, dafür wird jedoch zusätzlich [NSIS](http://nsis.sourceforge.net/) bzw. [WiX Toolset](http://wixtoolset.org/) benötigt.

`cpack` versucht standardmäßig die Release-Variante zu verpacken, das könnt ihr ändern mit dem Parameter `-C Debug`.

### Quelldateien verpacken

Die Quelldateien werden standardmäßig bei jedem Build verpackt. Im Build-Ordner erscheint dann eine zip-Datei mit eurem Code aber ohne temporäre Build-Dateien. Das erleichtert es euch euren Code mit anderen zu teilen.