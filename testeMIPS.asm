.data
x: .space 4
y: .space 4
z: .space 4
.text
addi $t1, $0, 2
addi $t2, $0, 7
sgt $t0, $t1, $t2
sw $t0, x
li $v0, 1
lw $a0, x
syscall
addi $t4, $0, 2
addi $t5, $0, 1
sgt $t3, $t4, $t5
sw $t3, y
li $v0, 1
lw $a0, y
syscall
addi $s1, $0, 2
addi $s2, $0, 7
sgt $s0, $s1, $s2
addi $s4, $0, 2
addi $s5, $0, 1
sgt $s3, $s4, $s5
and $t6, $s0, $s3
sw $t6, z
li $v0, 1
lw $a0, z
syscall