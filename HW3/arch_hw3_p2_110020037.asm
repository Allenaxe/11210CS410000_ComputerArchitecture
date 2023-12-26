.data
INPUT: .asciiz "Please input the total number of disks: "
A: .asciiz "A"
B: .asciiz "B"
C: .asciiz "C"
MOVEDISK: .asciiz "Move disk"
FROM: .asciiz "from"
TO: .asciiz "to"
SPACE: .asciiz " "
RESULT: .asciiz "Total number of movement = "
Pnewline: .asciiz "\n"

.text
move $s7 $0	#COUNT

main:
# print input string
la $a0 INPUT
li $v0 4
syscall
# get input numDisks
li $v0 5
syscall

# set parameters MoveTower(numDisks - 1, 'A', 'B', 'C')
addi $a0 $v0 -1
la $a1 A
la $a2 B
la $a3 C
# jump and link to MoveTower
jal MoveTower
# print result string
la $a0 RESULT
li $v0 4
syscall
# print result
move $a0 $s7
li $v0 1
syscall

j Exit

MoveTower:
# memory allocation
addi $sp $sp -20
# store previous register into memory
sw $ra 16($sp)
sw $s0 12($sp)
sw $s1 8($sp)
sw $s2 4($sp)
sw $s3 0($sp)
# move parameters to registers
move $s0 $a0
move $s1 $a1
move $s2 $a2
move $s3 $a3
# if(numDisks == 0) jump to L1
beq $a0 $0 L1
# set parameters MoveTower(disk - 1, source, spare, dest)
addi $a0 $s0 -1
move $a1 $s1
move $a2 $s3
move $a3 $s2
jal MoveTower
# print Move disk {disk} from {source} to {dest}
# Move disk
la $a0 MOVEDISK
li $v0 4
syscall

jal PrintSpace
# disk
move $a0 $s0
li $v0 1
syscall

jal PrintSpace
# from
la $a0 FROM
li $v0 4
syscall

jal PrintSpace
# source
move $a0 $s1
li $v0 4
syscall

jal PrintSpace
# to
la $a0 TO
li $v0 4
syscall

jal PrintSpace
# dest
move $a0 $s2
li $v0 4
syscall

jal PrintNewline

# COUNT++
addi $s7 $s7 1
# set parameters MoveTower(disk - 1, spare, dest, source)
addi $a0 $s0 -1
move $a1 $s3
move $a2 $s2
move $a3 $s1
jal MoveTower

# load previous register from memory
lw $s3 0($sp)
lw $s2 4($sp)
lw $s1 8($sp)
lw $s0 12($sp)
lw $ra 16($sp)
# recover the stack point
addi $sp $sp 20
jr $ra

L1:
# print Move disk {disk} from {source} to {dest}
# Move disk
la $a0 MOVEDISK
li $v0 4
syscall

jal PrintSpace
# disk
move $a0 $s0
li $v0 1
syscall

jal PrintSpace
# from
la $a0 FROM
li $v0 4
syscall

jal PrintSpace
# source
move $a0 $s1
li $v0 4
syscall

jal PrintSpace
# to
la $a0 TO
li $v0 4
syscall

jal PrintSpace
# dest
move $a0 $s2
li $v0 4
syscall

jal PrintNewline
# COUNT++
addi $s7 $s7 1
# load previous register from memory
lw $s3 0($sp)
lw $s2 4($sp)
lw $s1 8($sp)
lw $s0 12($sp)
lw $ra 16($sp)
# recover the stack point
addi $sp $sp 20
jr $ra

PrintSpace:
la $a0, SPACE 	 # load address of string to print
li $v0, 4	 # ready to print string
syscall	         # print
jr $ra           # return

PrintNewline:
la $a0, Pnewline # load address of string to print
li $v0, 4	 # ready to print string
syscall	         # print
jr $ra           # return

Exit:
# =====================================================
li $v0, 10	#Exit
syscall