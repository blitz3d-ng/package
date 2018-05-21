# SystemProperty (property$)

## Parameters

- property$ - system property information required (valid strings listed below)

## Description

`SystemProperty` is used to 'find out' certain system-specific things that are external to the currently running program.

There are broadly two different uses for `SystemProperty`, one for finding out the location of certain folders on the Windows OS, and one for finding out the handles/objects being used by the Win32/DX APIs to run Blitz programs. These objects and handles can then be used via third party DLLs to add extra functionality to Blitz, although this is recommended for advanced users only.

### Properties (folders)
- systemdir - System folder
- windowsdir - Windows folder
- tempdir - Temp folder
- appdir - Program Files folder

### Properties (objects/handles)
- Direct3D7
- Direct3DDevice7
- DirectDraw7
- DirectInput7
- AppHWND
- AppHINSTANCE
