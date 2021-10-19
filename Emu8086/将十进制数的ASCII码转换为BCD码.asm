
; You may customize this and other start-up templates; 
; The location of this template is c:\emu8086\inc\0_com_template.txt


;将十进制数的ASCII码转换为BCD码
SSTACK SEGMENT STACK
    DW 64 DUP(?)
SSTACK ENDS

CODE   SEGMENT 
       ASSUME CS:CODE
       
START: MOV CX,0005H
       MOV DI,3500H
       
A1:    MOV BL,0FFH
       MOV AL,[DI]
       CMP AL,3AH
       JNB A2
       MOV BL,AL
       
A2:    MOV AL,BL
       MOV [DI+0AH],AL
       INC DI
       LOOP A1 
       
A3:    JMP A3 

CODE   ENDS
       END START


