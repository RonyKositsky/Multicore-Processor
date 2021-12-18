#size of sub-mat, B = 8
add $r10, $zero, $zero, 0			#pc:, R10 - my core number is 0
add $r2, $zero, $imm, 0x100			#pc:1,address of second mat
add $r3, $zero, $imm, 0x200			#pc:2,address of result mat
#iteration 0

for i=0 i<B i++:
	for j=0 j<B j++:
		for k=0 k<B k++:
			r= r+ A[i][k]*B[k,j]
		Z[i][j]= Z[i][j] + r
