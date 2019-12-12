.data
	x: .space 4
	y: .space 4

.text
	li $v0, 5
	syscall
	sw $v0, x
	
	li $v0, 5
	syscall
	sw $v0, y
	
	li $v0, 1
	lw $a0, x
	syscall
	
	li $v0, 1
	lw $a0, y
	syscall