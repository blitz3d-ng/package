
Context "Filesystem"

Expect FileType("modules/filesystem.bb") = 1, "Expect this source file to exist on the disk."
Expect FileType("modules/a-bad-path.bb") = 0, "Expect an invalid path to be detected."
Expect FileType("modules") = 2, "Expect a this directory to be detected."
