# SetEnv env_var$,value$

## Parameters
- env_var$ - the name of the environment variable
- value$ - the value of the environment variable

## Description

Sets an environment variable.
However, it should be noted that the command does not set a Windows environment variable. It only sets an environment variable which is local to a single Blitz program - i.e. from the moment the SetEnv command is used, it will only exist for the duration of time the source code file is open in the Blitz IDE, or the program EXE is running.
## See Also

- GetEnv
