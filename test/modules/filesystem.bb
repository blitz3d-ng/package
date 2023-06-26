
Context "Filesystem"

Expect FileType("modules/filesystem.bb") = 1, "Expect this source file to exist on the disk."
Expect FileType("modules/a-bad-path.bb") = 0, "Expect an invalid path to be detected."
Expect FileType("modules") = 2, "Expect a this directory to be detected."

Expect FileSize("all.bb") >= 800, "Expect the root source file to be at least 800 bytes."

dir=ReadDir("./media")
found_axe = False
found_dwarf = False

; Order is not guaranteed on all platforms
Repeat
	file$ = NextFile(dir)
	If file = "" Exit

	Select file
	Case "axe.jpg"
		found_axe = True
	Case "dwarf2.b3d"
		found_dwarf = True
	Case ""
		Exit
	End Select
Forever

Expect found_axe, "Expect to find media/axe.jpg"
Expect found_dwarf, "Expect to find media/dwarf2.b3d"

CloseDir dir
