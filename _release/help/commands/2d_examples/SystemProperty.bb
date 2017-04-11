Print "Your OS is " + SystemProperty ("os")
Print "Your CPU is " + SystemProperty ("cpu")
Print
Print "System folder location: " + SystemProperty ("systemdir")
Print "Windows folder location: " + SystemProperty ("windowsdir")
Print "Temp folder: " + SystemProperty ("tempdir")
Print "Program was run from " + SystemProperty ("appdir")
Print
Print "The handle to this window is " + SystemProperty ("apphwnd")
Print "The handle to this application is " + SystemProperty ("apphinstance")
Print
Print "The handle to the IDirect3D7 interface is " + SystemProperty ("direct3d7")
Print "The handle to the IDirect3DDevice7 interface is " + SystemProperty ("direct3ddevice7")
Print "The handle to the IDirectDraw7 interface is " + SystemProperty ("directdraw7")

WaitKey
End
