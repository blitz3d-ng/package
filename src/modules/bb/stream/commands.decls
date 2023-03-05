Eof%( stream.bbStream ):"bbEof"
ReadAvail%( stream.bbStream ):"bbReadAvail"

ReadByte%( stream.bbStream ):"bbReadByte"
ReadShort%( stream.bbStream ):"bbReadShort"
ReadInt%( stream.bbStream ):"bbReadInt"
ReadFloat#( stream.bbStream ):"bbReadFloat"
ReadString$( stream.bbStream ):"bbReadString"
ReadLine$( stream.bbStream ):"bbReadLine"

WriteByte( stream.bbStream,byte% ):"bbWriteByte"
WriteShort( stream.bbStream,short% ):"bbWriteShort"
WriteInt( stream.bbStream,int% ):"bbWriteInt"
WriteFloat( stream.bbStream,float# ):"bbWriteFloat"
WriteString( stream.bbStream,string$ ):"bbWriteString"
WriteLine( stream.bbStream,string$ ):"bbWriteLine"

CopyStream( src_stream.bbStream,dest_stream.bbStream,buffer_size%=16384 ):"bbCopyStream"
