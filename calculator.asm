;  Unversidade de Brasilia
;  Instituto de Ciências Exatas
;  Departamento de Ciencia da Computaçao
;  Software Básico - 2/2019
;  
;  Trabalho II - Entrada e Saida de dados em IA-32 - Calculadora
;  Porfessor: Bruno Macciavelo
;  @author Gustavo Costa Crispim De Souza
;  @author Gabriel F P Araujo
; 

section .data
lineBreak db 0x0d,0x0a
char_zero db 00h
usr_ask_name db "Hóla caro usuário, qual o seu nome? ",0x0d,0x0a
usr_ask_name_tam equ $-usr_ask_name
hola db "Hóla, "
hola_tam equ $-hola
hola_cont db ", bem vindo ao programa de CALC IA-32",0x0d,0x0a
hola_cont_tam equ $-hola_cont
; usr_name dd 0   ; pointer to user name string
usr_name_tam dd 16   ; pointer to user name string
menu db "ESCOLHA UMA OPÇÃO:",0x0d,0x0a,"- 1: SOMA",0x0d,0x0a,"- 2: SUBSTRAÇÃO",0x0d,0x0a,"- 3: MULTIPLICAÇÃO",0x0d,0x0a,"- 4: DIVISÂO",0x0d,0x0a,"- 5: MOD",0x0d,0x0a,"- 6: SAIR",0x0d,0x0a,""
menu_tam equ $-menu
menu_opcao dw 0
char_getchar db 0
test_str db "is",0x0a
test_tam equ $-test_str
soma_str db "SOMA",0x0d,0x0a
soma_str_tam equ $-soma_str
op_1_ask db "Digite o primeiro operando",0x0d,0x0a,0x00
op_1_ask_tam equ $-op_1_ask
op_2_ask db "Digite o segundo operando",0x0d,0x0a,0x00
op_2_ask_tam equ $-op_2_ask
res_str db "Resultado ",0x0d,0x0a,0x00
res_str_tam equ $-res_str
TAM32B equ 10

num1 db "1245",0x0a
num1_tam equ $-num1
num2 db "35",0x0a
num2_tam equ $-num2

section .bss
usr_name resb 16
op1 resb TAM32B
op2 resb TAM32B

section .text
global _start

_start:
    ; enter 0,0
    ; mov ecx, [esp]
    ; inc esi
    push dword usr_ask_name_tam     ; show "Hóla, " string
    push dword usr_ask_name
    call show_string

    mov eax, usr_name
    mov ebx, usr_name_tam
    push dword usr_name_tam ; get user name string and its length
    push dword usr_name
    call get_string
calc:
    push dword hola_tam     ; show "Hóla, " string
    push dword hola
    call show_string

    mov eax, usr_name_tam
    ; sub eax, 1
    push dword usr_name_tam     ; show user name string
    push dword usr_name
    call show_string_lf

    push dword hola_cont_tam     ; continue to show presentation string
    push dword hola_cont
    call show_string

    push dword menu_tam     ; show menu to user and get the option
    push dword menu
    call show_string
    push dword menu_opcao
    call get_menu_option

    mov eax, menu_opcao
    cmp byte [eax], 1
    je soma
    cmp byte [eax], 2
    je subtracao
    cmp byte [eax], 3
    je multipl
    cmp byte [eax], 4
    je divisao
    cmp byte [eax], 5
    je mod
    cmp byte [eax], 6
    je _exit
    ; jge getchar
    ; cmp byte [eax], 0
    ; jle getchar
    jmp getchar

soma:
    push dword op_1_ask_tam     ; show user name string
    push dword op_1_ask
    mov eax, op_1_ask
    mov ebx, op_1_ask_tam
    call show_string
    mov eax, op1
    mov ebx, TAM32B
    push dword TAM32B
    push dword op1
    call get_num
    push dword op1
    call count_num
    push dword op1
    push dword eax
    call LeerInteiro

    push dword op_2_ask_tam     ; show user name string
    push dword op_2_ask
    call show_string
    mov eax, op2
    mov ebx, TAM32B
    push dword TAM32B
    push dword op2
    call get_num
    push dword op2
    call count_num
    push dword op2
    push dword eax
    call LeerInteiro

    mov ebx, op1
    mov eax, [ebx]
    mov ebx, op2
    add eax, [ebx]              ; op1 + op2
    ; push dword res_str_tam     ; show user name string
    ; push dword res_str
    ; call show_string
    push dword op1
    push eax
    call EscreverInteiro
    push dword 2
    push dword lineBreak
    call show_string

    jmp getchar

subtracao:
    push dword op_1_ask_tam     ; show user name string
    push dword op_1_ask
    mov eax, op_1_ask
    mov ebx, op_1_ask_tam
    call show_string
    mov eax, op1
    mov ebx, TAM32B
    push dword TAM32B
    push dword op1
    call get_num
    push dword op1
    call count_num
    push dword op1
    push dword eax
    call LeerInteiro

    push dword op_2_ask_tam     ; show user name string
    push dword op_2_ask
    call show_string
    mov eax, op2
    mov ebx, TAM32B
    push dword TAM32B
    push dword op2
    call get_num
    push dword op2
    call count_num
    push dword op2
    push dword eax
    call LeerInteiro

    mov ebx, op1
    mov eax, [ebx]
    mov ebx, op2
    sub eax, [ebx]              ; op1 - op2
    ; push dword res_str_tam     ; show user name string
    ; push dword res_str
    ; call show_string
    push dword op1
    push eax
    call EscreverInteiro
    push dword 2
    push dword lineBreak
    call show_string

    jmp getchar

multipl:
    push dword op_1_ask_tam     ; show user name string
    push dword op_1_ask
    mov eax, op_1_ask
    mov ebx, op_1_ask_tam
    call show_string
    mov eax, op1
    mov ebx, TAM32B
    push dword TAM32B
    push dword op1
    call get_num
    push dword op1
    call count_num
    push dword op1
    push dword eax
    call LeerInteiro

    push dword op_2_ask_tam     ; show user name string
    push dword op_2_ask
    call show_string
    mov eax, op2
    mov ebx, TAM32B
    push dword TAM32B
    push dword op2
    call get_num
    push dword op2
    call count_num
    push dword op2
    push dword eax
    call LeerInteiro

    mov ebx, op1
    mov eax, [ebx]
    mov ebx, op2
    mov ebx, [ebx]
    imul ebx              ; op1 * op2
    ; push dword res_str_tam     ; show user name string
    ; push dword res_str
    ; call show_string
    push dword op1
    push eax
    call EscreverInteiro
    push dword 2
    push dword lineBreak
    call show_string

    jmp getchar

divisao:
    push dword op_1_ask_tam     ; show user name string
    push dword op_1_ask
    mov eax, op_1_ask
    mov ebx, op_1_ask_tam
    call show_string
    mov eax, op1
    mov ebx, TAM32B
    push dword TAM32B
    push dword op1
    call get_num
    push dword op1
    call count_num
    push dword op1
    push dword eax
    call LeerInteiro

    push dword op_2_ask_tam     ; show user name string
    push dword op_2_ask
    call show_string
    mov eax, op2
    mov ebx, TAM32B
    push dword TAM32B
    push dword op2
    call get_num
    push dword op2
    call count_num
    push dword op2
    push dword eax
    call LeerInteiro

    mov ebx, op1
    mov eax, [ebx]
    mov ebx, op2
    mov ebx, [ebx]              ; op1 / op2
    sub edx, edx
    div ebx              ; op1 / op2
    ; push dword res_str_tam     ; show user name string
    ; push dword res_str
    ; call show_string
    push dword op1
    push eax
    call EscreverInteiro
    push dword 2
    push dword lineBreak
    call show_string

    jmp getchar

mod:
    push dword op_1_ask_tam     ; show user name string
    push dword op_1_ask
    mov eax, op_1_ask
    mov ebx, op_1_ask_tam
    call show_string
    mov eax, op1
    mov ebx, TAM32B
    push dword TAM32B
    push dword op1
    call get_num
    push dword op1
    call count_num
    push dword op1
    push dword eax
    call LeerInteiro

    push dword op_2_ask_tam     ; show user name string
    push dword op_2_ask
    call show_string
    mov eax, op2
    mov ebx, TAM32B
    push dword TAM32B
    push dword op2
    call get_num
    push dword op2
    call count_num
    push dword op2
    push dword eax
    call LeerInteiro

    mov ebx, op1
    mov eax, [ebx]
    mov ebx, op2
    mov ebx, [ebx]              ; op1 / op2
    sub edx, edx
    div ebx              ; op1 / op2
    ; push dword res_str_tam     ; show user name string
    ; push dword res_str
    ; call show_string
    push dword op1
    push edx
    call EscreverInteiro
    push dword 2
    push dword lineBreak
    call show_string

    jmp getchar

getchar:
    mov eax, 3
    mov ebx, 0
    mov ecx, char_getchar
    mov edx, 1
    int 80h
    jmp calc

_exit:
    ; leave
    mov eax, 1
    mov ebx, 0
    int 80h

show_string:
    ; mov eax, [ebp + 8]
    ; mov ecx, [ebp + 12]
    sub esi, esi
    push ebp
    mov ebp, esp
    mov eax, 4
    mov ebx, 1
    mov ecx, [ebp + 8]
    mov edx, [ebp + 12]
    int 0x80
    pop ebp
    ret 2

count_num:
    enter 0,0
    ; mov ebx, test_str
    mov ebx, [ebp + 8]
    mov edx, 0
strLoop.num:
    cmp byte [ebx], 0x0a
    je countDone.num
    cmp edx, TAM32B
    ; cmp edx, test_tam
    jae countDone.num
    inc ebx
    inc edx
    jmp strLoop.num
countDone.num:
    mov eax, edx
    leave
    ret 3

show_string_lf:
    sub esi, esi
    push ebp
    mov ebp, esp
count_string:
    ; mov ebx, test_str
    mov ebx, [ebp + 8]
    mov edx, 0
strLoop:
    cmp byte [ebx], 0x0a
    je countDone
    cmp edx, [ebp + 12]
    ; cmp edx, test_tam
    jae countDone
    inc ebx
    inc edx
    jmp strLoop
countDone:
    ; dec edx
    ; mov ebx, ebp
    ; add ebx, 8
    ; mov ebx, [ebx]
    ; mov [ebx], edx
    mov eax, 4
    mov ebx, 1
    mov ecx, [ebp + 8]
    ; mov edx, [ebp + 12]
    int 0x80
    pop ebp
    ret 4

get_string:
    ; get stdin string
    sub esi, esi
    push ebp
    mov ebp, esp
    mov eax, 3
    mov ebx, 0
    mov ecx, [ebp + 8]
    mov edx, [ebp + 12]
    int 80h
    pop ebp
    ret 4

get_menu_option:
    sub esi, esi
    push ebp
    mov ebp, esp
    ; get stdin number
    mov eax, 3
    mov ebx, 0
    mov ecx, [ebp + 8]
    mov edx, 2
    int 80h
    mov ebx, [ebp + 8]
    mov cl, [ebx]
    sub cl, 0x30
    mov byte [ebx], cl
    pop ebp
    ret 3

get_num:
    ; get stdin string
    sub esi, esi
    push ebp
    mov ebp, esp
    mov eax, 3
    mov ebx, 0
    mov ecx, [ebp + 8]
    mov edx, TAM32B
    int 80h
    pop ebp
    ret 4

LeerInteiro:
    enter 0,0
    push eax
    mov edi, 0
    mov eax, 0            ; Não está sendo salvo o valor de eax porque se o input foi chamado, o programador já espra encontrar ax com a quantidade de bytes lidos
    mov ebx, [ebp+12]     ; Valor a ser convertido
    mov ecx, [ebp+8]      ; Quantidade de bytes lidos
    ; dec ecx
    cmp byte [ebx], 0x2D  ; Verificando se é negativo
    jne prox1
    mov esi, 1            ; Setando flag de negativo
    dec ecx
    inc edi
prox1:
    cmp ecx, 0
    jle final1
    mov edx, eax
    add eax, edx
    add eax, edx
    add eax, edx
    add eax, edx
    add eax, edx
    add eax, edx
    add eax, edx
    add eax, edx
    add eax, edx
    mov edx, 0
    mov dl, [ebx+edi]      ; Pegando o primeiro char '3' de 3104
    sub dl, 0x30          ; '3' - 0x30
    add eax, edx          ; Valor = Valor*10 + '3' - 0x30
    dec ecx
    inc edi
    jmp prox1
final1:
    cmp esi, 1            ; Verificando flag do negativo
    jne positivo1
    neg eax
positivo1:
    mov esi, [ebp+12]     ; Salvando o resultado na variável 
    mov [esi], eax
    pop eax
    leave
    ret 4


EscreverInteiro:
    enter 0,0
    mov edi, 0
    mov esi, 10
    mov eax, [ebp+8]
    mov ebx, [ebp+12]
    mov ecx, 0
    cmp eax, 0
    jge prox2
    neg eax
    mov edi, 1
prox2:
    mov edx, 0
    div esi
    add edx, 0x30
    mov byte [ebx + ecx], dl
    cmp eax, 0
    je final2
    inc ecx
    jmp prox2
final2:
    cmp edi, 1
    jne positivo2
    inc ecx
    mov byte [ebx + ecx], 0x2D
positivo2:
    push ecx
    add ecx, ebx
    push ebx
    mov eax, 4
    mov ebx, 1
    mov edx, 1
    int 80h
    pop ebx
    pop ecx
    dec ecx
    cmp ecx, 0
    jl termino2
    jmp positivo2
termino2:
    leave
    ret 4
