Eof%( stream.BBStream ):"bbEof"
ReadAvail%( stream.BBStream ):"bbReadAvail"

ReadByte%( stream.BBStream ):"bbReadByte"
ReadShort%( stream.BBStream ):"bbReadShort"
ReadInt%( stream.BBStream ):"bbReadInt"
ReadFloat#( stream.BBStream ):"bbReadFloat"
ReadString$( stream.BBStream ):"bbReadString"
ReadLine$( stream.BBStream ):"bbReadLine"

WriteByte( stream.BBStream,byte% ):"bbWriteByte"
WriteShort( stream.BBStream,short% ):"bbWriteShort"
WriteInt( stream.BBStream,int% ):"bbWriteInt"
WriteFloat( stream.BBStream,float# ):"bbWriteFloat"
WriteString( stream.BBStream,string$ ):"bbWriteString"
WriteLine( stream.BBStream,string$ ):"bbWriteLine"

CopyStream( src_stream.BBStream,dest_stream.BBStream,buffer_size%=16384 ):"bbCopyStream"
