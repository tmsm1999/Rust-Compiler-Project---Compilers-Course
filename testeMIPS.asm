.data
	x: .space 4
	y: .space 4
	z: .space 4
	newline: .asciiz "\n"
.text
main:
	addi $t1, $0, 2
	addi $t2, $0, 2
	seq $t0, $t1, $t2
	bne $t0, $zero, l0
	j l1
l0:
	addi $t3, $0, 2
	sw $t3, x
	addi $t4, $0, 5
	sw $t4, z
l2:
	lw $t6, x
	addi $s0, $0, 5
	slt $t5, $t6, $s0
	bne $t5, $zero, l3
	j l4
l3:
	lw $s2, x
	addi $s3, $0, 1
	add $s1, $s2, $s3
	sw $s1, x
	li $v0, 1
	lw $a0, x
	syscall
	li $v0, 4
	la $a0, newline
	syscall
	j l2
l4:
l1:
	addi $s4, $0, 0
	sw $s4, y
	li $v0, 1
	lw $a0, y
	syscall
	li $v0, 4
	la $a0, newline
	syscall