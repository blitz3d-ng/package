# Blitz3D "NG"

[![Build status](https://github.com/blitz3d-ng/blitz3d-ng/actions/workflows/ci.yml/badge.svg?branch=master)](https://github.com/blitz3d-ng/blitz3d-ng/actions?query=branch%3Amaster)
[![Help](https://img.shields.io/badge/help-discord-7289DA.svg?logo=discord)](https://discord.gg/E6kTHXn)

This project is an attempt to modernize Blitz3D by adding cross-platform & 64-bit support.

So far, we've made huge strides in making this happen. Some of the basic samples (such as [castle](_release/samples/mak/castle), [driver](_release/samples/mak/driver), and [tron](_release/samples/mak/tron)) can run on macOS with little-to-no modifications.

This is achieved by adding [LLVM](#LLVM)-powered code generation to the original compiler. A basic test suite [test/all.bb](test/all.bb)

(Note: The 32-bit Windows build still uses the original Blitz code generation.)

## Download

You can get the latest Windows & macOS builds from the [releases page](https://github.com/blitz3d-ng/blitz3d-ng/releases).

If you're looking to customize your build further or interested in Linux/other platforms, see [Building](#Building).

## Support

If you need help with anything, please ask on the [Discord server](https://discord.gg/E6kTHXn).

For bug reports & feature requests, feel free to open an [issue](https://github.com/blitz3d-ng/blitz3d-ng/issues).

## Building

All 3rd-party dependencies are included in the repo or linked as submodules to keep the project simple to build,

To start, you'll need [cmake (3.16+)](https://cmake.org/download/).

### Windows

Install [Visual Studio 2022](https://visualstudio.microsoft.com/vs/community/). Be sure to install the MFC components.

Open up a command prompt via `Start -> Visual Studio 2022 -> Developer PowerShell for VS2022`.

```
> git clone --recursive https://github.com/blitz3d-ng/blitz3d-ng b3d
> cd b3d
> make release win32
> make release win64
```

Once the build is complete, you can open the `_release/` directory and run `Blitz3D.exe`
as expected.

### macOS

Install [Xcode](https://developer.apple.com/xcode/) and [brew](http://brew.sh/).

```bash
$ brew install cmake
$ bundle
$ git clone --recursive https://github.com/blitz3d-ng/blitz3d-ng b3d
$ cd b3d
$ make llvm
$ make ENV=release
```

### Linux

Setup will vary across distributions, but here is an example for Ubuntu.

```bash
$ sudo apt-get install -y libxml2-dev zlib1g-dev build-essential autoconf libtool gettext autopoint gperf cmake clang libwxgtk3.0-gtk3-dev libxrandr-dev libxinerama-dev libxcursor-dev uuid-dev libfontconfig1-dev
$ bundle
$ git clone --recursive https://github.com/blitz3d-ng/blitz3d-ng b3d
$ cd b3d
$ make llvm
$ make ENV=release
```

Alternatively, you can build everything with [docker](https://docker.io). See
[env.sh](env.sh) and the [env](https://github.com/blitz3d-ng/env) repo for some
example Dockerfiles.

## Documentation

The original Blitz3D help is available in the [\_release/help](_release/help) directory in HTML form. We've
started replacing it with a Markdown based system. You'll need a [ruby 3.1.2](https://www.ruby-lang.org/en/) install
to generate the documentation.

```bash
$ bundle install
$ bin/blitz3d help --build
```

## Roadmap

- Add misc. quick fixes & improvements (i.e., add "missing" features like [BlitzClose](http://www.blitzbasic.com/codearcs/codearcs.php?code=832).)
- Improve help files & styling.
- Swap out FMOD for something with a more permissive license.
- OpenGL, Vulkan, Metal, and newer Direct3D renderers.
- macOS, Linux, Android, iOS support.

## License

```
The zlib/libpng License

Copyright (c) 2013 Blitz Research Ltd

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.

2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any source distribution.
```
