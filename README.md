# OpenCOLLADA Cleanup Fork

A fork of [OpenCOLLADA](https://github.com/KhronosGroup/OpenCOLLADA) which seems to be not updated since 2018.

*"Bbbbut why?"*

Blender has Collada import/export via OpenCOLLADA library. Now, at this point there aren't _many_ Collada users left within Blender,
but there are _some_. However, full OpenCOLLADA embeds several external libraries, many of which were not updated for many years.
That is not bad per se, except said externals are known to have 18 "critical" CVEs and 42 "high" CVEs. It did not _feel good_ to
keep embedding a library that embeds this many known security issues.

Thus this fork. It removes everything (a lot!) that is not actually needed by Blender, and either removes usages of some externals,
or updates them to more recent versions. Blender is switching to this fork for Blender 4.2. FYI, Collada support overall within
Blender is still marked as "legacy" and is on a chopping block for possible removal in Blender 5.0. But at least for now,
fewer embedded security issues. See [this thread](https://devtalk.blender.org/t/moving-collada-i-o-to-legacy-status/34621) for
more details.

### Changes from official project

TL;DR: removed everything that Blender does not need, updated externals to more recent versions.

* Applied [Blender patch](https://projects.blender.org/blender/blender/src/commit/63f211475/build_files/build_environment/patches/opencollada.diff)
  that is mostly fixing performance issues where hot loops were copying things being iterated on by value.
* Build: Fix build with Visual Studio 2022 / Clang 14, and in general require at least C++11.
* Build: CMake parallel compilation in MSVC.
* Build: Removed SCons build system and Visual Studio / Xcode project files.
* Build: Updated embedded `libxml2` version from 2.6.32 (2008) to 2.11.8 (2024).
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

### External Libraries

* `Externals/LibXML` - [libxml2](https://gitlab.gnome.org/GNOME/libxml2) version 2.11.8 (2024 May).
  * Only the files in root folder, plus `include` and `win32` subfolders. Everything else is not needed.
* `Externals/MathMLSolver` - origin unknown, Copyright (c) 2007 netAllied GmbH, feels like it is not really "external".
* `Externals/UTF` - origin unknown, Copyright 2001-2004 Unicode, Inc, from around 2004.
