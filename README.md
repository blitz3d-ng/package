# Blitz3D-NG

This project is an attempt to revive & modernize Blitz3D.

## Building

You'll need a copy of [Visual Studio](https://www.visualstudio.com/vs/community/) and [premake](https://premake.github.io/download.html).

Open up a command prompt via `Start -> Visual Studio 2015 -> Developer Command Prompt for VS2015`.

```
> git clone --recursive https://github.com/kfprimm/blitz3d-ng
> cd blitz3d-ng
> premake5 vs2015 && devenv build\blitz3d.sln /build release
```

## Roadmap

- Add misc. quick fixes & improvements (i.e., add "missing" features like [BlitzClose](http://www.blitzbasic.com/codearcs/codearcs.php?code=832).)
- Improve help files & styling.
- Swap out FMOD for something with a more permissive license.
- Integrate [Assimp](http://www.assimp.org/) to that we don't depend on the DirectX X library anymore.
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
