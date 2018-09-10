
Context "Filesystem"

Expect FileType("test/modules/filesystem.bb") = 1, "Expect this source file to exist on the disk."
Expect FileType("test/modules/a-bad-path.bb") = 0, "Expect an invalid path to be detected."
Expect FileType("test/modules") = 2, "Expect a this directory to be detected."
