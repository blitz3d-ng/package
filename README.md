# Blitz3D "NG"
[![Build status](https://ci.appveyor.com/api/projects/status/ww8qjywqm6rb5rnu/branch/master?svg=true)](https://ci.appveyor.com/project/kfprimm/blitz3d-ng-gj3xh/branch/master)
[![Build Status](https://travis-ci.org/blitz3d-ng/blitz3d-ng.svg?branch=master)](https://travis-ci.org/blitz3d-ng/blitz3d-ng)

This project is an attempt to revive & modernize Blitz3D by adding cross-platform & 64-bit support.

So far, we've made huge strides in making this happen. Some of the basic samples (such as [castle](_release/samples/mak/castle), [driver](_release/samples/driver), and [tron](_release/samples/mak/tron)) can run on macOS (64 bit!) with little-to-no modifications.

This is achieved by translating Blitz source into C code.

(Note: The Windows build still uses the original Blitz code generation.)

## Download

You can get the latest Windows build directly from our [CI tool](https://ci.appveyor.com/api/projects/kfprimm/blitz3d-ng-gj3xh/artifacts/release.zip?branch=master&job=Environment:%20TOOLSET=vs2015;%20Configuration:%20release;%20Platform:%20win32). This build includes Direct3D 7 & OpenGL runtimes, without DirectPlay.

If you're looking to customize your build further or interested in macOS or Linux, please see the next section.

## Building

To keep the project simple to build, all 3rd-party dependencies are included in the repo or linked as submodules.

### Prerequisites (All platforms)

You'll need [premake (alpha 11)](https://premake.github.io/download.html) and a valid [ruby 2+](https://www.ruby-lang.org/en/) install.

We use these tools to generate the appropriate build files per-platform.

### Windows

Install [Visual Studio](https://www.visualstudio.com/vs/community/).

Open up a command prompt via `Start -> Visual Studio 2015 -> Developer Command Prompt for VS2015`.

```
> git clone --recursive https://github.com/blitz3d-ng/blitz3d-ng
> cd blitz3d-ng
> ruby bin\blitz3d config && premake5 vs2015 && devenv build\blitz3d.sln /build "release|win32"
```

### macOS

Install [Xcode](https://developer.apple.com/xcode/) and [brew](http://brew.sh/).

```bash
$ git clone --recursive https://github.com/blitz3d-ng/blitz3d-ng
$ cd blitz3d-ng
$ brew install wxmac glfw3
$ bin/blitz3d config && premake5 gmake && make config=release_macos
```

### Linux

```bash
$ git clone --recursive https://github.com/blitz3d-ng/blitz3d-ng
$ cd blitz3d-ng
$ apt-get install -y build-essential libwxgtk3.0-dev
$ bin/blitz3d config && premake5 gmake && make config=release_linux
```

Alternatively, you can build everything with [docker](https://docker.io).

```bash
$ git clone --recursive https://github.com/blitz3d-ng/blitz3d-ng
$ cd blitz3d-ng
$ docker-compose build
$ docker-compose run env
```

## Documentation

The original Blitz3D help is available in the [\_release/help](_release/help) directory in HTML form. We've started replacing it with a Markdown based system.

```bash
$ bundle install
$ bin/makedocs
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
