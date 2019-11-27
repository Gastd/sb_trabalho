section .data
lineBreak db 0x0d,0x0a
char_zero db 00h
hola db "Hóla, "
hola_tam equ $-hola
hola_cont db ", bem vindo ao programa de CALC IA-32",0x0d,0x0a
hola_cont_tam equ $-hola_cont
usr_name dd 0   ; pointer to user name string
usr_name_tam dd 0   ; pointer to user name string
menu db "ESCOLHA UMA OPÇÃO:",0x0d,0x0a,"- 1: SOMA",0x0d,0x0a,"- 2: SUBSTRAÇÃO",0x0d,0x0a,"- 3: MULTIPLICAÇÃO",0x0d,0x0a,"- 4: DIVISÂO",0x0d,0x0a,"- 5: MOD",0x0d,0x0a,"- 6: SAIR",0x0d,0x0a,""
menu_tam equ $-menu
menu_opcao dw 0

section .text
global _start

_start:
    sub esi, esi
    mov ebp, esp
    ; mov ecx, [esp]
    ; inc esi
    push dword hola_tam     ; show "Hóla, " string
    push dword hola
    call show_string

    push dword usr_name_tam ; get user name string and its length
    push dword usr_name
    call get_string

    push dword hola_cont_tam     ; continue to show presentation string
    push dword hola_cont
    call show_string

    push dword menu_tam     ; show menu to user and get the option
    push dword menu
    call show_string
    push dword menu_opcao
    call get_menu_option


_exit:
    mov eax, 1
    mov ebx, 0
    int 80h

show_string:
    sub esi, esi
    mov ebp, esp
    mov eax, 4
    mov ebx, 1
    mov ecx, [ebp + 4]
    mov edx, [ebp + 8]
    int 0x80
    ret 3

get_string:
    sub esi, esi
    mov ebp, esp
    ret 3

get_menu_option:
    sub esi, esi
    mov ebp, esp
    ret 3

soma:
    ret

subtracao:
    ret

multipl:
    ret

divisao:
    ret

mod:
    ret
