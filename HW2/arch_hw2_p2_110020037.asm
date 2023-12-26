.data
PInput:	.asciiz "Please enter a number(Input 0 to exit): "
PResult: .asciiz "The Roman is: "
PBye: .asciiz "bye"
Pnewline: .asciiz "\n"

# ones[] = {"","I","II","III","IV","V","VI","VII","VIII","IX"}
ones: .asciiz "\0\0\0\0"
      .asciiz "I\0\0\0"
      .asciiz "II\0\0"
      .asciiz "III\0"
      .asciiz "IV\0\0"
      .asciiz "V\0\0\0"
      .asciiz "VI\0\0"
      .asciiz "VII\0"
      .asciiz "VIII"
      .asciiz "IX\0\0"

# tens[] = {"","X","XX","XXX","XL","L","LX","LXX","LXXX","XC"}
tens: .asciiz "\0\0\0\0"
      .asciiz "X\0\0\0"
      .asciiz "XX\0\0"
      .asciiz "XXX\0"
      .asciiz "XL\0\0"
      .asciiz "L\0\0\0"
      .asciiz "LX\0\0"
      .asciiz "LXX\0"
      .asciiz "LXXX"
      .asciiz "XC\0\0"

# hrns[] = {"","C","CC","CCC","CD","D","DC","DCC","DCCC","CM"}
hrns: .asciiz "\0\0\0\0"
      .asciiz "C\0\0\0"
      .asciiz "CC\0\0"
      .asciiz "CCC\0"
      .asciiz "CD\0\0"
      .asciiz "D\0\0\0"
      .asciiz "DC\0\0"
      .asciiz "DCC\0"
      .asciiz "DCCC"
      .asciiz "CM\0\0"

# thos[]={"","M","MM","MMM"};
thos:  .asciiz "\0\0\0\0"
      .asciiz "M\0\0\0"
      .asciiz "MM\0\0"
      .asciiz "MMM\0"

.text

while:			# while loop
la $a0 PInput		# Ouput input string
li $v0 4
syscall

li $v0 5		# Input
syscall

beq $v0 $0 Exit		# If(Input == 0) Exit
move $s0 $v0

la $a0 PResult		# Ouput result string
li $v0 4
syscall

#Calculate Thousand Position
div $t0 $s0 1000 	# $t0 = num / 1000
mul $t0 $t0 5		# $t0 = (num / 1000) * 5 (memory offset)
la $a0 thos($t0)	# $a0 = ths[num / 1000] => ths($t0)
li $v0 4
syscall

#Calculate Hundred Position
div $t0 $s0 1000	# $t0 = num / 1000
mfhi $t0		# $t0 = num % 1000
div $t0 $t0 100		# $t0 = (num % 1000) / 100
mul $t0 $t0 5		# $t0 = ((num % 1000) / 100) * 5 (memory offset)
la $a0 hrns($t0)	# $a0 = hrns[(num % 1000) / 100] => hrns($t0)
li $v0 4
syscall

#Calculate Ten Position
div $t0 $s0 100		# $t0 = num / 100
mfhi $t0		# $t0 = num % 100
div $t0 $t0 10		# $t0 = (num % 100) / 10
mul $t0 $t0 5		# $t0 = ((num % 100) / 10) * 5 (memory offset)
la $a0 tens($t0)	# $a0 = tens[(num % 100) / 10] => tens($t0)
li $v0 4
syscall

#Calculate One Position
div $t0 $s0 10		# $t0 = num / 10
mfhi $t0		# $t0 = num % 10
mul $t0 $t0 5		# $t0 = (num % 10) * 5 (memory offset)
la $a0 ones($t0)	# $a0 = tens[num % 10] => ones($t0)
li $v0 4
syscall

jal PrintNewline
j while

PrintNewline:
la $a0, Pnewline # load address of string to print
li $v0, 4	 # ready to print string
syscall	         # print
jr $ra           # return

Exit:
# =====================================================
la $a0 PBye	# Ouput bye
li $v0 4
syscall

li $v0, 10	#Exit
syscall

