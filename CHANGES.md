# Change Log

This document lays out what we've changed from the original Blitz3D.

## Runtime

### Graphics

- Apps are now DPI "aware." This means you will need to scale your windows & UIs based on user DPI. See DPIScaleX()/DPIScaleY().

### Input

- DirectInput has been bumped to DirectInput8.

### Networking

- DirectPlay related commands have been disabled.

## IDE

- It is now DPI "aware."
- `$BLITZPATH/tmp` is automatically created if it does not exist.
- Improved indentation handling.
- Added BB icon to executable & window.
- Fixed pasting junk data.
- Added 64-bit build.

## Debugger

- It is now DPI "aware" and scales based on user DPI setting.
- Added BB rocket icon to executable & window.

## Compiler

- Removed rocket icon from generated executables.
