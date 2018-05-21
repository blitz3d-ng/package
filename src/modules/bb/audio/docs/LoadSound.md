# LoadSound (filename$)

## Parameters

- filename$ - name of sound file. Formats supported: raw/wav/mp3/ogg

## Description

This command loads a sound file into memory. It returns a number if successful,  or 0 if there was a problem loading the sound. You must assign the value this  returns to a variable (preferably a Global variable)  for subsequent playback using (PlaySound). Look at the example.
