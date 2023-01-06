.MODEL SMALL 
.STACK 100H 
.DATA

N DW ?
CR EQU 0DH
LF EQU 0AH
ERRORMSG DB "INVALID INPUT$"

.CODE 
MAIN PROC 
    MOV AX, @DATA
    MOV DS, AX
    
    ; fast BX = 0
    XOR BX, BX
    
    INPUT_LOOP:
    ; char input 
    MOV AH, 1
    INT 21H
    
    ; if \n\r, stop taking input
    CMP AL, CR    
    JE END_INPUT_LOOP
    CMP AL, LF
    JE END_INPUT_LOOP
    
    ; fast char to digit
    ; also clears AH
    AND AX, 000FH
    
    ; save AX 
    MOV CX, AX
    
    ; BX = BX * 10 + AX
    MOV AX, 10
    MUL BX
    ADD AX, CX
    MOV BX, AX
    JMP INPUT_LOOP
    
    END_INPUT_LOOP:
    MOV N, BX
    
    ; printing CR and LF
    MOV AH, 2
    MOV DL, CR
    INT 21H
    MOV DL, LF
    INT 21H
    
    
    ;------------------------------------
    ; start from here
    ; input is in N
    CMP N,100
    JLE A_PLUS
    ;ERRORCASE
    MOV AH, 9
    LEA DX,ERRORMSG 
    INT 21H
    JMP EXIT
    
    A_PLUS:
    CMP N,80
    JL A
    MOV AH, 2
    MOV DL, 'A'
    INT 21H
    MOV AH, 2
    MOV DL, '+'
    INT 21H
    JMP EXIT
    
    A:
    CMP N,75
    JL A_MINUS
    MOV AH, 2
    MOV DL, 'A'
    INT 21H
    JMP EXIT
    
    A_MINUS:
    CMP N,70
    JL B_PLUS
    MOV AH, 2
    MOV DL, 'A'
    INT 21H
    MOV AH, 2
    MOV DL, '-'
    INT 21H
    JMP EXIT
    
    B_PLUS:
    CMP N,65
    JL B
    MOV AH, 2
    MOV DL, 'B'
    INT 21H
    MOV AH, 2
    MOV DL, '+'
    INT 21H
    JMP EXIT
    
    B:
    CMP N,60
    JL F
    MOV AH, 2
    MOV DL, 'B'
    INT 21H
    JMP EXIT
    
    
    F:
    MOV AH, 2
    MOV DL, 'F'
    INT 21H
    JMP EXIT
    
      

	EXIT: 
	MOV AH, 2
    MOV DL, CR
    INT 21H
    MOV DL, LF
    INT 21H
	; interrupt to exit
    MOV AH, 4CH
    INT 21H
    
  
MAIN ENDP 
END MAIN 


