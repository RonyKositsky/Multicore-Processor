#R2 - global counter
#R3 - local counter
#R4 - modulu result
#R10 - my core number
add $r10, $imm, $zero, 1		#pc:0, R10 = 0 my core number is 1 
add, $r3, $zero, $imm, 128		#pc:1, R3 = 128, local counter
lw, $r2, $zero, $zero, 0		#pc:2, R2 = MEM[0]
and $r4, $r2, $imm, 3			#pc:3, R4 = R2&3 (equivalent to MEM[0] modulu 4)
bne, $imm, $r4, $r10, 2			#pc:4, if R4 != R10, jump to pc=2 (if not my turn, keep polling)
add $zero, $zero, $zero, 0		#pc:5, nop
add, $r2, $r2, $imm, 1			#pc:6, R2 = R2 + 1
sw, $r2, $zero, $zero, 0		#pc:7, MEM[0] = R2
sub, $r3, $r3, $imm, 1			#pc:8, R3 = R3 - 1
bne, $imm, $r3, $zero, 2		#pc:9, if R3 != 0, jump to pc=2, else stop execution
add $zero, $zero, $zero, 0		#pc:10, nop
halt, $zero, $zero, $zero, 0	#pc:11, HALT