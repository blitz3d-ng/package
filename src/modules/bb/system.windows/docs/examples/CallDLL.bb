in_bank=CreateBank(...)

out_bank=CreateBank(...)

;poke input parameters into in_bank

result=CallDLL( "mydll","my_dll_func",bank1,bank2 )

<br>
;peek output results from out_bank
