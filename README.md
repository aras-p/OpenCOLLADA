# OpenCOLLADA Cleanup Fork

A fork of [OpenCOLLADA](https://github.com/KhronosGroup/OpenCOLLADA) which seems to be not updated since 2018.

### Changes from official project

* Applied [Blender patch](https://projects.blender.org/blender/blender/src/commit/63f211475/build_files/build_environment/patches/opencollada.diff)
* Build: Fix build with Visual Studio 2022 / Clang 14, and in general require at least C++11.
* Build: CMake parallel compilation in MSVC.
* Build: Removed SCons build system and Visual Studio / Xcode project files.
* Cleanup: Removed 3dsmax, Maya plugins; Validator tools; 3ds/ma/ogre converters; G3D browser; website files;
  xsd2cppsax tool; Cg, expat, lib3ds, MayaDataModel, zlib, zziplib embedded externals.
* Refactor: Removed usage of `pcre` regular expressions library; in the two places that were using it (URI and SidAddress)
  do the parsing manually.
* Cleanup: Removed template.cpp/template.h files that I have no idea what were there for.
* Cleanup: Removed all code related to unused GENERATEDSAXPARSER_VALIDATION define.
* Cleanup: Removed various unused code.

### Build

Use CMake. Requires at least C++11 compiler. Available build options:
* `USE_STATIC` (ON) - Build static libraries, mutually exlusive with `USE_SHARED`.
* `USE_SHARED` (OFF) - Build shared libraries, available currently only on Unix-like environments.
* `USE_STATIC_MSVC_RUNTIME` (OFF) - Use static version of the MSVC run-time library, Windows/MSVC-only.
Increases the size of the binaries, but is useful e.g. when wanting to build a standalone application that
uses OpenCOLLADA with no runtime dependencies. Requires that all dependencies in the project use the
same run-time library option.

### Directories

* `COLLADABaseUtils` - utils used by other parts
* `COLLADAFramework` - data model used to load COLLADA files
* `COLLADASaxFrameworkLoader` - loads COLLADA files in SAX like manner into the framework data model
* `COLLADAStreamWriter` - library to write COLLADA files
* `Externals` - third party libraries used for building
* `GeneratedSaxParser` - library used to load xml files in the way used by COLLADASaxFrameworkLoader
