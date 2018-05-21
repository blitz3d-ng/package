# ExecFile( file$ )

## Parameters

-  file$ - the file to be executed

## Description

The usefulness of this command is really mostly for calling some system level command, launching a browser, etc.

Note: This command uses ShellExecute to allow you to 'open' any file (like a  .doc or .txt) file with its default associated program.

Also: if the filename path has spaces in it you may will need to change the call like so:

```blitzbasic
ExecFile (Chr$(34)+"example file name.exe"+Chr$(34))
```

This adds the " (quotation mark) character to the beginning and end of the text string.
