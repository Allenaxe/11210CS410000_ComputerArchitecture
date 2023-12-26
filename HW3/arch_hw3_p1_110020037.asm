.data
XINPUT: .asciiz "input x: "
YINPUT: .asciiz "input y: "
ZINPUT: .asciiz "input z: "
RESULT: .asciiz "result = "

.text
main:
# printf("input x: ");
la $a0 XINPUT
li $v0 4
syscall

# input x
li $v0 5
syscall

# save x into $s0
move $s0 $v0

# printf("input y: ");
la $a0 YINPUT
li $v0 4
syscall

# input y
li $v0 5
syscall

# save y into $s1
move $s1 $v0

# printf("input z: ");
la $a0 ZINPUT
li $v0 4
syscall

# input z
li $v0 5
syscall

# save z into $s2 
move $s2 $v0

# $v0 = compare(x, y)
move $a0 $s0
move $a1 $s1
jal compare

# $v0 = smod(compare(x, y), z)
move $a0 $v0
move $a1 $s2
jal smod

# save result into $s3
move $s3 $v0

# output result string
la $a0 RESULT
li $v0 4
syscall

# output result
move $a0 $s3
li $v0 1
syscall

j Exit

compare:
# if(p <= q) jump to L1
slt $t0 $a1 $a0
beq $t0 $0 L1
# else return p + q 
add $v0 $a0 $a1
jr $ra

L1:
# return p
move $v0 $a0
jr $ra

smod:
# if(p <= q) jump to L2
slt $t0 $a1 $a0
beq $t0 $0 L2
# $t0 = p % 4
div $t0 $a0 4
mfhi $t0
# pow(2, p % 4) = 1 << (p % 4)
addi $t1 $0 1
sllv $t1 $t1 $t0
# div = 2 + pow(2, p % 4)
addi $t1 $t1 2
# div = div * 5
addi $t0 $0 5
mul $t1 $t1 $t0
# divd = p * 4 + q
addi $t0 $0 4
mul $a0 $a0 $t0
add $t2 $a0 $a1
# return div % divd
div $v0 $t2 $t1
mfhi $v0
jr $ra

L2:
# $t0 = q % 4
div $t0 $a1 4
mfhi $t0
# pow(2, q % 4) = 1 << (q % 4)
addi $t1 $0 1
sllv $t1 $t1 $t0
# div = 4 + pow(2, q % 4)
addi $t1 $t1 4
# div = div * 5
addi $t0 $0 5
mul $t1 $t1 $t0
# divd = p * 4 + q
addi $t0 $0 4
mul $a0 $a0 $t0
add $t2 $a0 $a1
# return div % divd
div $v0 $t2 $t1
mfhi $v0
jr $ra

Exit:
# =====================================================
li $v0, 10	#Exit
syscall
