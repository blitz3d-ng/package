OpenFile.bbFile( filename$ ):"bbOpenFile"
ReadFile.bbFile( filename$ ):"bbReadFile"
WriteFile.bbFile( filename$ ):"bbWriteFile"
CloseFile( file_stream.bbFile ):"bbCloseFile"
FilePos%( file_stream.bbFile ):"bbFilePos"
SeekFile%( file_stream.bbFile,pos% ):"bbSeekFile"
ReadDir.BBDir( dirname$ ):"bbReadDir"
CloseDir( dir.BBDir ):"bbCloseDir"
NextFile$( dir.BBDir ):"bbNextFile"
CurrentDir$():"bbCurrentDir"
ChangeDir( dir$ ):"bbChangeDir"
CreateDir( dir$ ):"bbCreateDir"
DeleteDir( dir$ ):"bbDeleteDir"
FileSize%( file$ ):"bbFileSize"
FileType%( file$ ):"bbFileType"
CopyFile( file$,to$ ):"bbCopyFile"
DeleteFile( file$ ):"bbDeleteFile"
