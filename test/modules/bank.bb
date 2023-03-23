Context "Bank"

bank = CreateBank( 1024 )
Expect bank<>0,""
Expect BankSize(bank)=1024, "Bank size starts at 1024"

PokeByte bank,4,123
Expect PeekByte(bank, 4)=123, "Expected byte 4 to equal 123"

ResizeBank bank,2048
Expect BankSize(bank)=2048, "Bank size is now 2048"

bank2 = CreateBank( 1024 )
Expect bank2<>0,""
CopyBank bank,0,bank2,4,1024-4
Expect PeekByte(bank2, 8)=123, "Expected byte 8 to equal 123"
