OpenFile.BBFile( filename$ ):"bbOpenFile"
ReadFile.BBFile( filename$ ):"bbReadFile"
WriteFile.BBFile( filename$ ):"bbWriteFile"
CloseFile( file_stream.BBFile ):"bbCloseFile"
FilePos%( file_stream.BBFile ):"bbFilePos"
SeekFile%( file_stream.BBFile,pos% ):"bbSeekFile"
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
