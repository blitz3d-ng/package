# Blitz3D "NG"
[![Build status](https://ci.appveyor.com/api/projects/status/ww8qjywqm6rb5rnu/branch/master?svg=true)](https://ci.appveyor.com/project/kfprimm/blitz3d-ng-gj3xh/branch/master)
[![Build Status](https://travis-ci.org/blitz3d-ng/blitz3d-ng.svg?branch=master)](https://travis-ci.org/blitz3d-ng/blitz3d-ng)
[![Help](https://img.shields.io/badge/help-discord-7289DA.svg?logo=discord)](https://discord.gg/E6kTHXn)

This project is an attempt to revive & modernize Blitz3D by adding cross-platform & 64-bit support.

So far, we've made huge strides in making this happen. Some of the basic samples (such as [castle](_release/samples/mak/castle), [driver](_release/samples/driver), and [tron](_release/samples/mak/tron)) can run on macOS (64 bit!) with little-to-no modifications.

This is achieved by translating Blitz source into C code.

(Note: The Windows build still uses the original Blitz code generation.)

## Download

You can get the latest Windows build directly from our [CI tool](https://ci.appveyor.com/api/projects/kfprimm/blitz3d-ng-gj3xh/artifacts/release.zip?branch=master&job=Environment:%20TOOLSET=vs2017;%20Configuration:%20release;%20Platform:%20win32). This build includes Direct3D 7 & OpenGL runtimes, without DirectPlay.

If you're looking to customize your build further or interested in macOS or Linux, please see the next section.

## Support

If you need help with anything, please ask on the [Discord server](https://discord.gg/E6kTHXn).

For bug reports & feature requests, feel free to open an [issue](https://github.com/blitz3d-ng/blitz3d-ng/issues).

## Building

To keep the project simple to build, all 3rd-party dependencies are included in the repo or linked as submodules.

### Prerequisites (All platforms)

You'll need [cmake (3.16+)](https://cmake.org/download/) and a [ruby 2+](https://www.ruby-lang.org/en/) install.

We use these tools to generate the appropriate build files per-platform as well as documentation.

### Windows

Install [Visual Studio 2019](https://www.visualstudio.com/vs/community/).

Open up a command prompt via `Start -> Visual Studio 2019 -> Developer Command Prompt for VS2019`.

```
> git clone --recursive https://github.com/blitz3d-ng/blitz3d-ng
> cd blitz3d-ng
> make release win32
> make release win64
```

### macOS

Install [Xcode](https://developer.apple.com/xcode/) and [brew](http://brew.sh/).

```bash
$ brew install cmake wxmac
$ bundle
$ git clone --recursive https://github.com/blitz3d-ng/blitz3d-ng
$ cd blitz3d-ng
$ bin/blitz3d config && make ENV=release
```

### Linux

Setup will vary across distributions, but here is an example for Ubuntu.

```bash
$ sudo apt-get install -y libxml2-dev zlib1g-dev build-essential autoconf libtool gettext autopoint gperf cmake clang libwxgtk3.0-gtk3-dev libxrandr-dev libxinerama-dev libxcursor-dev uuid-dev libfontconfig1-dev
$ bundle
$ git clone --recursive https://github.com/blitz3d-ng/blitz3d-ng
$ cd blitz3d-ng
$ bin/blitz3d config && make ENV=release
```

Alternatively, you can build everything with [docker](https://docker.io).

```bash
$ git clone --recursive https://github.com/blitz3d-ng/blitz3d-ng
$ cd blitz3d-ng
$ docker build -t blitz3d - < Dockerfile
$ docker run -t -i -v `pwd`:/blitz3d-ng blitz3d make ENV=${ENV} PLATFORM=linux
```

## Documentation

The original Blitz3D help is available in the [\_release/help](_release/help) directory in HTML form. We've started replacing it with a Markdown based system.

```bash
$ bundle install
$ bin/blitz3d help --build
```

## Roadmap

- Add misc. quick fixes & improvements (i.e., add "missing" features like [BlitzClose](http://www.blitzbasic.com/codearcs/codearcs.php?code=832).)
- Improve help files & styling.
- Swap out FMOD for something with a more permissive license.
- Integrate [Assimp](http://www.assimp.org/) so that we don't depend on the DirectX X library anymore.
- OpenGL & newer Direct3D renderers.
- 64-bit support.
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
