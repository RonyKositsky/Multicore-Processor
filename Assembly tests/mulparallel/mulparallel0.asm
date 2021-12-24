#size of sub-mat, B = 8
add $r10, $zero, $zero, 0				#pc:, R10 - my core number is 0
add $r2, $zero, $imm, 0x100				#pc:1,address of second mat
add $r3, $zero, $imm, 0x200				#pc:2,address of result mat
add $r4, $zero, $imm, 0					#pc:, i = 0
add $r5, $zero, $imm, 0					#pc:, j = 0
add $r6, $zero, $imm, 0					#pc:, k = 0
add $r7, $zero, $imm, 8					#pc:, B = 8
add $r8, $zero, $imm, 0					#pc:, r = 0
add $r9, $zero, $imm, 16				#pc:, N = 16
#Perform A0 * B0, then A0 * B1
iLoop1:
	bge $imm, $r4, $r7, iLoop2					#pc:, if i >= B, exit iLoop1
	add $zero, $zero, $zero, 0					#pc:, nop
	add $r5, $zero, $zero, $zero, 0				#pc:, j = 0
	jLoop1:
		bge $imm, $r5, $r7, jLoop1E				#pc:, if j >= B, exit jLoop1
		add $zero, $zero, $zero, 0				#pc:, nop
		add $r6, $zero, $zero, $zero, 0			#pc:, k = 0
		add $r8, $zero, $zero, $zero, 0			#pc:, r = 0
		kLoop1:
			bge $imm, $r6, $r7, kLoop1E			#pc:, if k >= B, exit kLoop1
			add $zero, $zero, $zero, 0			#pc:, nop
			mul $r11, $r9, $r4, 0				#pc:, temp1 = i*N
			add $r11, $r11, $r6, 0				#pc:, temp1 = i*N +k = address1 in memory
			lw $r11, $r11, $zero, 0				#pc:, temp1 = MEM[i*N + k] = Y[i][k]
			mul $r12, $r9, $r6, 0				#pc:, temp2 = k*N
			add $r12, $r12, $r5, 0				#pc:, temp2 = k*N + j
			lw $r12, $r12, $r2, 0				#pc:, temp2 = MEM[k*N + j + 0x100] = Z[k][j]
			mul $r13, $r11, $r12, 0				#pc:, temp3 = Y[i][k] * Z[k][j]
			add $r8, $r8, $r13, 0				#pc:, r = r + Y[i][k] * Z[k][j]
			add $r6 ,$r6, $imm, 1				#pc:, k++
			beq $imm, $zero, $zero, kLoop1		#pc:, jump to start of kLoop1
			add $zero, $zero, $zero, 0			#pc:, nop
		kLoop1E:
		mul $r11, $r4, $r9, 0					#pc:, temp1 = i*N
		add $r11, $r11, $r5, 0					#pc:, temp1 = i*N + j
		lw $r12, $r11, $r3, 0					#pc:, temp2 = MEM[i*N + j + 0x200] = Z[i][j]
		add $r12, $r12, $r8, 0					#pc:, temp2 = Z[i][j] + r
		sw $r12, $r11, $r3, 0					#pc:, MEM[i*N + j + 0x200] = Z[i][j] = Z[i][j] + r
		add $r5 ,$r5, $imm, 1					#pc:, j++
		beq $imm, $zero, $zero, jLoop1			#pc:, jump to start of jLoop1 
		add $zero, $zero, $zero, 0				#pc:, nop
	jLoop1E:
	add $r4 ,$r4, $imm, 1						#pc:, i++
	beq $imm, $zero, $zero, iLoop1				#pc:, jump to start of iLoop1
	add $zero, $zero, $zero, 0					#pc:, nop	
#for i=0 i<B i++:
#	for j=0 j<B j++:
#		for k=0 k<B k++:
#			r= r+ A[i][k]*B[k,j]
#		Z[i][j]= Z[i][j] + r
add $r4, $zero, $zero, 0						#pc:, i = 0
iLoop2:
	bge $imm, $r4, $r7, kLoop2E					#pc:, if i >= B, exit iLoop2
	add $zero, $zero, $zero, 0					#pc:, nop
	add $r5, $7, $zero, $zero, 0				#pc:, j = B
	jLoop2:
		bge $imm, $r5, $r9, jLoop2E				#pc:, if j >= N, exit jLoop2
		add $zero, $zero, $zero, 0				#pc:, nop
		add $r6, $zero, $zero, $zero, 0			#pc:, k = 0
		add $r8, $zero, $zero, $zero, 0			#pc:, r = 0
		kLoop2:
			bge $imm, $r6, $r7, kLoop2E			#pc:, if k >= B, exit kLoop1
			add $zero, $zero, $zero, 0			#pc:, nop
			mul $r11, $r9, $r4, 0				#pc:, temp1 = i*N
			add $r11, $r11, $r6, 0				#pc:, temp1 = i*N +k = address1 in memory
			lw $r11, $r11, $zero, 0				#pc:, temp1 = MEM[i*N + k] = Y[i][k]
			mul $r12, $r9, $r6, 0				#pc:, temp2 = k*N
			add $r12, $r12, $r5, 0				#pc:, temp2 = k*N + j
			lw $r12, $r12, $r2, 0				#pc:, temp2 = MEM[k*N + j + 0x100] = Z[k][j]
			mul $r13, $r11, $r12, 0				#pc:, temp3 = Y[i][k] * Z[k][j]
			add $r8, $r8, $r13, 0				#pc:, r = r + Y[i][k] * Z[k][j]
			add $r6 ,$r6, $imm, 1				#pc:, k++
			beq $imm, $zero, $zero, kLoop2		#pc:, jump to start of kLoop2
			add $zero, $zero, $zero, 0			#pc:, nop
		kLoop2E:
		mul $r11, $r4, $r9, 0					#pc:, temp1 = i*N
		add $r11, $r11, $r5, 0					#pc:, temp1 = i*N + j
		lw $r12, $r11, $r3, 0					#pc:, temp2 = MEM[i*N + j + 0x200] = Z[i][j]
		add $r12, $r12, $r8, 0					#pc:, temp2 = Z[i][j] + r
		sw $r12, $r11, $r3, 0					#pc:, MEM[i*N + j + 0x200] = Z[i][j] = Z[i][j] + r
		add $r5 ,$r5, $imm, 1					#pc:, j++
		beq $imm, $zero, $zero, jLoop2			#pc:, jump to start of jLoop2
		add $zero, $zero, $zero, 0				#pc:, nop
	jLoop2E:
	add $r4 ,$r4, $imm, 1						#pc:, i++
	beq $imm, $zero, $zero, iLoop2				#pc:, jump to start of iLoop2
	add $zero, $zero, $zero, 0					#pc:, nop
iLoop2E:
#for i=0 i<B i++:
#	for j=B j<N j++:
#		for k=0 k<B k++:
#			r= r+ A[i][k]*B[k,j]
#		Z[i][j]= Z[i][j] + r
halt $zero, $zero, $zero, 0						#pc:, HALT execution
