#size of sub-mat, B = 8
add $r2, $zero, $zero, 0			#pc:0,R2 = temp
add $r3, $zero, $imm, 0x100			#pc:1,address of second mat
add $r4, $zero, $imm, 0x200			#pc:2,address of result mat
add $r5, $zero, $zero,16			#pc:3, R5 = 16, size of mat, N
add $r6, $zero, $zero, 0			#pc:4, R6 = temp
add $r7, $zero, $zero, 0			#pc:5, R7 = 0, i
add $r8, $zero, $zero, 0			#pc:6, R8 = 0, j
add $r9, $zero, $zero, 0			#pc:7, R9 = 0, k
add $r10, $zero, $zero, 0			#pc:8, R10 = 0, jj
add $r11, $zero, $zero, 0			#pc:9, R11 = 0, kk
add $r12, $zero, $zero, 0			#pc:10, R12 = 0, address inside first mat 
add $r13, $zero, $zero, 0			#pc:11, R13 = 0, calculation result, r
add $r14, $zero, $zero, 0			#pc:12, R14 = 0, temp
add $r15, $zero, $zero, 0			#pc:13, R15 = 0, address inside second mat 
bge $imm, $r10, $r5, 55				#pc:14, if jj >= N, exit jj (main) loop
add $r11, $zero, $zero, $zero		#pc:15, kk = 0
bge $imm, $r11, $r5, 53				#pc:16, if kk >= N, exit kk loop
add $r7, $zero, $zero, 0			#pc:17, i = 0
bge $imm, $r7, $r5, 51				#pc:18, if i >= N, exit i loop
add $r8, $zero, $r10, 0				#pc:19, j = jj
add $r14, $r10, $imm, 8				#pc:20, temp = jj + B    j loop jumps here
blt $imm, $r14, $r5, 23				#pc:21, if temp < N, skip next instruction
add $r14, $zero, $r5, 0				#pc:22, temp = N
bge $imm, $r14, $r8, 49				#pc:23, if min(jj + B, N) >= j, exit j loop
add $r13, $zero, $zero, $zero		#pc:24, r = 0
add $r9, $r10, $zero, 0				#pc:25, k = kk
add $r14, $r11, $imm, 8				#pc:26, temp = kk + B    k loop jumps here
blt $imm, $r14, $r5, 29				#pc:27, if temp < N, skip next instruction
add $r14, $zero, $r5, 0				#pc:28, temp = N
bge $imm, $r14, $r9, 41				#pc:29, if min(kk + B, N) >= k, exit k loop
mul $r12, $r7, $r5, 0				#pc:30, address1 = i*N
add $r12, $r12, $r9, 0				#pc:31, address1 = i*N + k = address1 in memory
mul $r15, $r9, $r5, 0				#pc:32, address2 = k*N
add $r15, $r15, $r8, 0				#pc:33, address2 = k*N + j
add $r15, $r15, $r3, 0				#pc:34, address2 = address2 in memory
lw $r2, $r12, $zero, 0				#pc:35, R2 = MEM[i*N + k] = Y[i][k]
lw $r14, $r15, $zero, 0				#pc:36, R14 = MEM[k*N + j] = Z[k][j]
mul $r6, $r2, $r14, 0				#pc:37, R6 = R2 * R14 = Y[i][k] * Z[k][j]
add $r13, $r13, $r6, 0				#pc:38, r = r + Y[i][k] * Z[k][j]
add $r9, $r9, $imm, 1				#pc:39, k++
beq $imm, $zero, $zero, 26			#pc:40, jump to start of k loop
mul $r2, $r7, $r5, 0				#pc:41, R2 = i * N
add $r2, $r2, $r8, 0				#pc:42, R2 = i * N + j
add $r2, $r2, $r4, 0				#pc:43, R2 = address of X[i][j] in memory
lw $r6, $r2, $zero, 0				#pc:44, R6 = MEM[i*N + j] = X[i][j]
add $r6, $r6, $r13, 0				#pc:45, R6 = R6 + r
sw $r6, $r2, $zero, 0				#pc:46, X[i][j] = R6 = X[i][j] + r
add $r8, $r8, $imm, 1				#pc:47, j++
beq $imm, $zero, $zero, 20			#pc:48, jump to start of j loop
add $r7, $r7, $imm, 1				#pc:49, i++
beq $imm, $zero, $zero, 18			#pc:50, jump to start of i loop
add $r11, $r11, $imm, 8				#pc:51, kk =  kk + B
beq $imm, $zero, $zero, 16			#pc:52, jump to start of kk loop
add $r10, $r10, $imm, 8				#pc:53, jj =  jj + B
beq $imm, $zero, $zero, 14			#pc:54, jump to start of jj loop
halt $zero, $zero, $zero, 0			#pc:55, HALT execution