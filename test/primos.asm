.data
primos:	.word 1, 3, 5, 7, 11, 13, 17, 19
size:	.word 8
msg:	.asciiz "Os oito primeiros numeros primos sao: "
space:	.asciiz " "

.text
	la $t0, primos
	la $t1, size
	lw $t1,0($t1)
	li $v0, 4
	la $a0, msg
	syscall
loop:	beq $t1, $zero, exit	#se processou todo o array, encerra
	li $v0, 1	#serviço de impressão de inteiros
	lw $a0, 0($t0)	#inteiro a ser exibido
	syscall
	li $v0, 4	#imprime separador
	la $a0, space
	syscall
	addi $t0, $t0, 4	#incrementa indice array
	addi $t1, $t1, -1	#decrementa contador
	j loop	#novo loop
exit:	li $v0, 10
	syscall