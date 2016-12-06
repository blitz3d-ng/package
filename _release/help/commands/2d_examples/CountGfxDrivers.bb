; GfxDriver Examples

; Count how many drivers there are
totalDrivers=CountGfxDrivers()
Print "Choose a driver to use:"

; Go through them all and print their names (most people will have only 1)
For t = 1 To totalDrivers
Print t+") " + GfxDriverName$(t)
Next

; Let the user choose one
driver=Input("Enter Selection:")

; Set the driver!
SetGfxDriver driver
Print "Your driver has been selected!"
