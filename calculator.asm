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
op_1_ask_tam dd 5
op_2_ask db "Digitse o segundo operando",0x0d,0x0a,0x00
op_2_ask_tam dd 5

num1 db "1245",0x0a
num1_tam equ $-num1
num2 db "35",0x0a
num2_tam equ $-num2

section .bss
usr_name resb 16
op1 resb 10
op2 resb 10

section .text
global _start

_start:
    sub esi, esi
    mov ebp, esp
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
    cmp byte [eax], 0
    jle getchar

_exit:
    mov eax, 1
    mov ebx, 0
    int 80h

show_string:
    mov eax, [ebp + 4]
    mov ecx, [ebp + 8]
    sub esi, esi
    mov ebp, esp
    mov eax, 4
    mov ebx, 1
    mov ecx, [ebp + 4]
    mov edx, [ebp + 8]
    int 0x80
    ret 3

show_string_lf:
    sub esi, esi
    mov ebp, esp
count_string:
    ; mov ebx, test_str
    mov ebx, [ebp + 4]
    mov edx, 0
strLoop:
    cmp byte [ebx], 0x0a
    je countDone
    cmp edx, [ebp + 8]
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
    mov ecx, [ebp + 4]
    ; mov edx, [ebp + 8]
    int 0x80
    ret 3

get_string:
    ; get stdin string
    sub esi, esi
    mov ebp, esp
    mov eax, 3
    mov ebx, 0
    mov ecx, [ebp + 4]
    mov edx, [ebp + 8]
    int 80h
    ret 3

get_menu_option:
    sub esi, esi
    mov ebp, esp
    ; get stdin number
    mov eax, 3
    mov ebx, 0
    mov ecx, [ebp + 4]
    mov edx, 1
    int 80h
    mov ebx, [ebp + 4]
    mov cl, [ebx]
    sub cl, 0x30
    mov byte [ebx], cl
    ret 3

get_num:
    sub esi, esi
    mov ebp, esp
    ; add ecx, 10
    mov eax, 3
    mov ebx, 0
    mov ecx, [ebp + 4]
    ; mov edx, [ebp + 8]
    mov edx, 1
    int 80h
    ret 3

soma:
    push dword op_1_ask_tam     ; show user name string
    push dword op_1_ask
    mov eax, op_1_ask
    mov ebx, op_1_ask_tam
    call show_string
    ; push dword 10
    push dword op1
    call get_num

    push dword op_2_ask_tam     ; show user name string
    push dword op_2_ask
    call show_string
    push dword op2
    call get_num

    mov eax, op1
    add eax, op2

    jmp getchar

subtracao:
    jmp getchar

multipl:
    jmp getchar

divisao:
    jmp getchar

mod:
    jmp getchar

getchar:
    mov eax, 3
    mov ebx, 0
    mov ecx, char_getchar
    mov edx, 1
    int 80h
    jmp calc
