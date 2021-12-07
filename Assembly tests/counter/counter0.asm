#R2 - global counter
#R3 - local counter
#R4 - modulu result
#R10 - my core number
add $r10, $zero, $zero, 0		#pc:0, R10 = 0 my core number is 0 
lw, $r2, $zero, $zero, 0		#pc:1, R2 = MEM[0]
add, $r3, $zero, $imm, 128		#pc:2, R3 = 128, local counter
add, $r2, $r2, $imm, 1			#pc:3, R2 = R2 + 1
sw, $r2, $zero, $zero, 0		#pc:4, MEM[0] = R2
sub, $r3, $r3, $imm, 1			#pc:5, R3 = R3 - 1
lw, $r2, $zero, $zero, 0		#pc:6, R2 = MEM[0]
and $r4, $r2, $imm, 3			#pc:7, R4 = R2&3 (equivalent to MEM[0] modulu 4)
bne, $imm, $r4, $r10, 6			#pc:8, if R4 != R10, jump to pc=6 (if not my turn, keep polling)
add $zero, $zero, $zero, 0		#pc:9, nop
add, $r2, $r2, $imm, 1			#pc:10, R2 = R2 + 1
sw, $r2, $zero, $zero, 0		#pc:11, MEM[0] = R2
sub, $r3, $r3, $imm, 1			#pc:12, R3 = R3 - 1
bne, $imm, $r3, $zero, 6		#pc:13, if R3 != 0, jump to pc=6, else stop execution
add $zero, $zero, $zero, 0		#pc:14, nop
halt, $zero, $zero, $zero, 0	#pc:15, HALT