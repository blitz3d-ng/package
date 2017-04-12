# GetEnv$ ( env_var$ )

## Parameters

- env_var$ - the name of the environment variable to 'get'

## Description

Returns the value of the specified environment variable.
Unlike the SetEnv command, which is only capable of setting environment variables local to Blitz, the GetEnv command is capable of getting Windows environment variables.
Windows environment variables tell your computer what kind of machine it is, and where to install programs.If you're curious to find out what environment variables are set on your Windows install, then you can find out in WindowsXP by running 'cmd', and then typing 'set'.
### See Also
- SetEnv
## [Example](examples/GetEnv.bb)
