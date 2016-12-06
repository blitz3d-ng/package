# Change Log

This document lays out what we've changed from the original Blitz3D.

## Runtime

- DirectPlay related commands has been disabled.
- Apps are now DPI "aware." This means you will need to scale your windows & UIs based on user DPI. See DPIScaleX()/DPIScaleY().

## IDE

- It is now DPI "aware."
- `$BLITZPATH/tmp` is automatically created if it does not exist.
- Improved indentation handling.
