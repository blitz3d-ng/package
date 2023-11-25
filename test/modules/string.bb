
Context "String"

ExpectStr String( "Kevin",2 ),"KevinKevin"
ExpectStr String( "wörld",2 ),"wörldwörld"

ExpectStr Left( "Kevin",3 ),"Kev"
ExpectStr Left( "Kevin",1000 ),"Kevin"
ExpectStr Left( "wörld",2 ),"wö"

ExpectStr Right( "Kevin",2 ),"in"
ExpectStr Right( "Kevin",1000 ),"Kevin"
ExpectStr Right( "wörld",4 ),"örld"

ExpectStr Replace( "Kevin","vin","lsey" ),"Kelsey"
ExpectStr Replace( "hello, wörld","wörld","world" ),"hello, world"
ExpectStr Replace( "hello, world","world","wörld" ),"hello, wörld"

ExpectInt Instr( "Kevin","v" ),3, "Expects to find a 'v'"
ExpectInt Instr( "Kevin","b" ),0, "Expects to not find a 'b'"
ExpectInt Instr( "wörld","r" ),3, "Expects to find an 'r'"
ExpectInt Instr( "AöBBAA","A",5 ),5
ExpectInt Instr( "AöBBAA","A",100000 ),0
ExpectInt Instr( "AöBBAA","C",100000 ),0

ExpectStr Mid( "Kevin",2,3 ), "evi"
ExpectStr Mid( "wörld",2,2 ), "ör"
ExpectStr Mid( "wörld",1000,1000 ), ""
ExpectStr Mid( "wörld",2,1000 ), "örld"

ExpectStr Upper( "Kevin" ),"KEVIN"
ExpectStr Upper("hello, wörld"),"HELLO, WÖRLD"

ExpectStr Lower( "Kevin" ), "kevin", "Expects all lowercase characters"
ExpectStr Lower( "WÖRLd" ), "wörld", "Expects all lowercase characters"

ExpectStr Trim( "  Kevin   " ), "Kevin", "Expects no leading or trailing whitespace"
ExpectStr Trim( "  wörld   " ), "wörld", "Expects no leading or trailing whitespace"

ExpectStr LSet( "Kevin",8 ), "Kevin   ", "Expects padding on right"
ExpectStr LSet( "wörld",8 ), "wörld   ", "Expects padding on right"
ExpectStr LSet( "wörld",2 ), "wö", "Expects truncation"

ExpectStr RSet( "Kevin",8 ), "   Kevin", "Expects padding on left"
ExpectStr RSet( "wörld",8 ), "   wörld", "Expects padding on left"
ExpectStr RSet( "dlröw",2 ), "öw", "Expects truncation"

ExpectStr Chr( 65 ),"A", "Expects 65 to be 'A'"
ExpectStr Chr( 246 ),"ö", "Expects 246 to be 'ö'"

ExpectInt Asc( "A" ),65
ExpectInt Asc( "ö" ),246

ExpectInt Len( "Kevin" ),5, "Expects 5 characters"
ExpectInt Len("hello, wörld"),12, "'hello wörld' has 12 characters"

ExpectStr Hex( 255 ),"000000FF", "Expects a hex string"

ExpectInt Bin(14),1110 ; convert to int to deal with 32 vs 64 bits

Expect CurrentDate$()<>"", "Gets the current date"

Expect CurrentTime$()<>"", "Gets the current time"
