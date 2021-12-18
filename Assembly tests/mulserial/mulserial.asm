#size of sub-mat, B = 8
add $r2, $zero, $zero, 0			#pc:0,R2 = temp
add $r3, $zero, $imm, 0x100			#pc:1,address of second mat
add $r4, $zero, $imm, 0x200			#pc:2,address of result mat
add $r5, $zero, $imm,16				#pc:3, R5 = 16, size of mat, N
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
bge $imm, $r10, $r5, 67				#pc:14, if jj >= N, exit jj (main) loop
add $zero, $zero, $zero, 0			#pc:15, nop
add $r11, $zero, $zero, $zero		#pc:16, kk = 0
bge $imm, $r11, $r5, 64				#pc:17, if kk >= N, exit kk loop	this was 16
add $zero, $zero, $zero, 0			#pc:18, nop
add $r7, $zero, $zero, 0			#pc:19, i = 0
bge $imm, $r7, $r5, 61				#pc:20, if i >= N, exit i loop	this was 18
add $zero, $zero, $zero, 0			#pc:21, nop
add $r8, $zero, $r10, 0				#pc:22, j = jj
add $r14, $r10, $imm, 8				#pc:23, temp = jj + B    j loop jumps here	this was 20
blt $imm, $r14, $r5, 27				#pc:24, if temp < N, skip next instruction
add $zero, $zero, $zero, 0			#pc:25, nop
add $r14, $zero, $r5, 0				#pc:26, temp = N
bge $imm, $r8, $r14, 58				#pc:27, if min(jj + B, N) <= j, exit j loop	this was 23
add $zero, $zero, $zero, 0			#pc:28, nop
add $r13, $zero, $zero, $zero		#pc:29, r = 0
add $r9, $r11, $zero, 0				#pc:30, k = kk
add $r14, $r11, $imm, 8				#pc:31, temp = kk + B    k loop jumps here	this was 26
blt $imm, $r14, $r5, 35				#pc:32, if temp < N, skip next instruction
add $zero, $zero, $zero, 0			#pc:33, nop
add $r14, $zero, $r5, 0				#pc:34, temp = N
bge $imm, $r9, $r14, 49				#pc:35, if min(kk + B, N) <= k, exit k loop	this was 29
add $zero, $zero, $zero, 0			#pc:36, nop
mul $r12, $r7, $r5, 0				#pc:37, address1 = i*N
add $r12, $r12, $r9, 0				#pc:38, address1 = i*N + k = address1 in memory
mul $r15, $r9, $r5, 0				#pc:39, address2 = k*N
add $r15, $r15, $r8, 0				#pc:40, address2 = k*N + j
add $r15, $r15, $r3, 0				#pc:41, address2 = address2 in memory
lw $r2, $r12, $zero, 0				#pc:42, R2 = MEM[i*N + k] = Y[i][k]
lw $r14, $r15, $zero, 0				#pc:43, R14 = MEM[k*N + j] = Z[k][j]
mul $r6, $r2, $r14, 0				#pc:44, R6 = R2 * R14 = Y[i][k] * Z[k][j]
add $r13, $r13, $r6, 0				#pc:45, r = r + Y[i][k] * Z[k][j]
add $r9, $r9, $imm, 1				#pc:46, k++
beq $imm, $zero, $zero, 31			#pc:47, jump to start of k loop
add $zero, $zero, $zero, 0			#pc:48, nop
mul $r2, $r7, $r5, 0				#pc:49, R2 = i * N	this was 41
add $r2, $r2, $r8, 0				#pc:50, R2 = i * N + j
add $r2, $r2, $r4, 0				#pc:51, R2 = address of X[i][j] in memory
lw $r6, $r2, $zero, 0				#pc:52, R6 = MEM[i*N + j] = X[i][j]
add $r6, $r6, $r13, 0				#pc:53, R6 = R6 + r
sw $r6, $r2, $zero, 0				#pc:54, X[i][j] = R6 = X[i][j] + r
add $r8, $r8, $imm, 1				#pc:55, j++
beq $imm, $zero, $zero, 23			#pc:56, jump to start of j loop
add $zero, $zero, $zero, 0			#pc:57, nop
add $r7, $r7, $imm, 1				#pc:58, i++	this was 49
beq $imm, $zero, $zero, 20			#pc:59, jump to start of i loop
add $zero, $zero, $zero, 0			#pc:60, nop
add $r11, $r11, $imm, 8				#pc:61, kk =  kk + B	this was 51
beq $imm, $zero, $zero, 17			#pc:62, jump to start of kk loop
add $zero, $zero, $zero, 0			#pc:63, nop
add $r10, $r10, $imm, 8				#pc:64, jj =  jj + B	this was 53
beq $imm, $zero, $zero, 14			#pc:65, jump to start of jj loop
add $zero, $zero, $zero, 0			#pc:66, nop
lw $r2, $imm, $zero, 15				#pc:67, create conflict for memory write
lw $r2, $imm, $zero, 31				#pc:68, create conflict for memory write
lw $r2, $imm, $zero, 47				#pc:69, create conflict for memory write
lw $r2, $imm, $zero, 63				#pc:70, create conflict for memory write
lw $r2, $imm, $zero, 79				#pc:71, create conflict for memory write
lw $r2, $imm, $zero, 95				#pc:72, create conflict for memory write
lw $r2, $imm, $zero, 111			#pc:73, create conflict for memory write
lw $r2, $imm, $zero, 127			#pc:74, create conflict for memory write
lw $r2, $imm, $zero, 511			#pc:75, create conflict for memory write
halt $zero, $zero, $zero, 0			#pc:76, HALT execution this was 55