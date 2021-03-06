#size of sub-mat, B = 8
add $r10, $zero, $zero, 0						#pc:0, R10 - my core number is 0
add $r2, $zero, $imm, 0x100						#pc:1,address of second mat
add $r3, $zero, $imm, 0x200						#pc:2,address of result mat
add $r4, $zero, $imm, 0							#pc:3, i = 0
add $r5, $zero, $imm, 0							#pc:4, j = 0
add $r6, $zero, $imm, 0							#pc:5, k = 0
add $r7, $zero, $imm, 8							#pc:6, B = 8
add $r8, $zero, $imm, 0							#pc:7, r = 0
add $r9, $zero, $imm, 16						#pc:8, N = 16
#Perform A0 * B1, then A1 * B3
iLoop1:
	bge $imm, $r4, $r7, StartLoop2				#pc:9, if i >= B, exit iLoop1
	add $zero, $zero, $zero, 0					#pc:A, nop
	add $r5, $zero, $r7, 0						#pc:B, j = B
	jLoop1:
		bge $imm, $r5, $r9, jLoop1E				#pc:C, if j >= N, exit jLoop1
		add $zero, $zero, $zero, 0				#pc:D, nop
		add $r6, $zero, $zero, 0				#pc:E, k = 0
		add $r8, $zero, $zero, 0				#pc:F, r = 0
		kLoop1:
			bge $imm, $r6, $r7, kLoop1E			#pc:10, if k >= B, exit kLoop1
			add $zero, $zero, $zero, 0			#pc:11, nop
			mul $r11, $r9, $r4, 0				#pc:12, temp1 = i*N
			add $r11, $r11, $r6, 0				#pc:13, temp1 = i*N +k = address1 in memory
			lw $r11, $r11, $zero, 0				#pc:14, temp1 = MEM[i*N + k] = Y[i][k]
			mul $r12, $r9, $r6, 0				#pc:15, temp2 = k*N
			add $r12, $r12, $r5, 0				#pc:16, temp2 = k*N + j
			lw $r12, $r12, $r2, 0				#pc:17, temp2 = MEM[k*N + j + 0x100] = Z[k][j]
			mul $r13, $r11, $r12, 0				#pc:18, temp3 = Y[i][k] * Z[k][j]
			add $r8, $r8, $r13, 0				#pc:19, r = r + Y[i][k] * Z[k][j]
			add $r6 ,$r6, $imm, 1				#pc:1A, k++
			beq $imm, $zero, $zero, kLoop1		#pc:1B, jump to start of kLoop1
			add $zero, $zero, $zero, 0			#pc:1C, nop
		kLoop1E:
		mul $r11, $r4, $r9, 0					#pc:1D, temp1 = i*N
		add $r11, $r11, $r5, 0					#pc:1E, temp1 = i*N + j
		lw $r12, $r11, $r3, 0					#pc:1F, temp2 = MEM[i*N + j + 0x200] = X[i][j]
		add $r12, $r12, $r8, 0					#pc:20, temp2 = X[i][j] + r
		sw $r12, $r11, $r3, 0					#pc:21, MEM[i*N + j + 0x200] = X[i][j] = X[i][j] + r
		add $r14, $r11, $r3, 0					#pc:, temp = i*N + j + 0x200
		lw $r14, $r14, $imm, 256				#pc:, temp = MEM[temp], create conflict miss on X[i][j]
		add $r5 ,$r5, $imm, 1					#pc:22, j++
		beq $imm, $zero, $zero, jLoop1			#pc:23, jump to start of jLoop1 
		add $zero, $zero, $zero, 0				#pc:24, nop
	jLoop1E:
	add $r4 ,$r4, $imm, 1						#pc:25, i++
	beq $imm, $zero, $zero, iLoop1				#pc:26, jump to start of iLoop1
	add $zero, $zero, $zero, 0					#pc:27, nop	
#for i=0 i<B i++:
#	for j=B j<N j++:
#		for k=0 k<B k++:
#			r= r+ A[i][k]*B[k,j]
#		Z[i][j]= Z[i][j] + r
StartLoop2:
add $r4, $zero, $zero, 0						#pc:28, i = 0
iLoop2:
	bge $imm, $r4, $r7, iLoop2E					#pc:, if i >= B, exit iLoop1
	add $zero, $zero, $zero, 0					#pc:, nop
	add $r5, $zero, $r7, 0						#pc:, j = B
	jLoop2:
		bge $imm, $r5, $r9, jLoop2E				#pc:, if j >= N, exit jLoop1
		add $zero, $zero, $zero, 0				#pc:, nop
		add $r6, $r7, $zero, 0					#pc:, k = B
		add $r8, $zero, $zero, 0				#pc:, r = 0
		kLoop2:
			bge $imm, $r6, $r9, kLoop2E			#pc:, if k >= N, exit kLoop1
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
			beq $imm, $zero, $zero, kLoop2		#pc:, jump to start of kLoop1
			add $zero, $zero, $zero, 0			#pc:, nop
		kLoop2E:
		mul $r11, $r4, $r9, 0					#pc:, temp1 = i*N
		add $r11, $r11, $r5, 0					#pc:, temp1 = i*N + j
		lw $r12, $r11, $r3, 0					#pc:, temp2 = MEM[i*N + j + 0x200] = X[i][j]
		add $r12, $r12, $r8, 0					#pc:, temp2 = X[i][j] + r
		sw $r12, $r11, $r3, 0					#pc:, MEM[i*N + j + 0x200] = X[i][j] = X[i][j] + r
		add $r14, $r11, $r3, 0					#pc:, temp = i*N + j + 0x200
		lw $r14, $r14, $imm, 256				#pc:, temp = MEM[temp], create conflict miss on X[i][j]
		add $r5 ,$r5, $imm, 1					#pc:, j++
		beq $imm, $zero, $zero, jLoop2			#pc:, jump to start of jLoop1 
		add $zero, $zero, $zero, 0				#pc:, nop
	jLoop2E:
	add $r4 ,$r4, $imm, 1						#pc:, i++
	beq $imm, $zero, $zero, iLoop2				#pc:, jump to start of iLoop1
	add $zero, $zero, $zero, 0					#pc:, nop	
iLoop2E:
#for i=0 i<B i++:
#	for j=B j<N j++:
#		for k=B k<N k++:
#			r= r+ A[i][k]*B[k,j]
#		Z[i][j]= Z[i][j] + r
halt $zero, $zero, $zero, 0						#pc:48, HALT execution
