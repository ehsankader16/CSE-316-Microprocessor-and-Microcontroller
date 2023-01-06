.MODEL SMALL
    
    
.STACK 100H
    
    
.DATA
CR EQU 0DH
LF EQU 0AH
     
PRX DB 'Input X$'
PRY DB 'Input Y$'    
X DB ?
Y DB ?
Z DB ?
     
.CODE

MAIN PROC
    ;DATA SEGMENT INITIALIZATION
    MOV AX, @DATA
    MOV DS, AX
           
    ;PROMPT
    LEA DX, PRX
    MOV AH, 9
    INT 21H  
    
    ;NEW LINE
    MOV DL, CR
    MOV AH, 2
    INT 21H
    MOV DL, LF
    MOV AH, 2
    INT 21H
    
    ;PROMPT
    LEA DX, PRY
    MOV AH, 9
    INT 21H
    
    ;NEW LINE
    MOV DL, CR
    MOV AH, 2
    INT 21H
    MOV DL, LF
    MOV AH, 2
    INT 21H
           
    ;INPUT X
    MOV AH, 1
    INT 21H
    MOV X, AL
    
    ;NEW LINE
    MOV DL, CR
    MOV AH, 2
    INT 21H
    MOV DL, LF
    MOV AH, 2
    INT 21H
            
    ;INPUT Y
    MOV AH, 1
    INT 21H
    MOV Y, AL
               
    ;NEW LINE
    MOV DL, CR
    MOV AH, 2
    INT 21H
    MOV DL, LF
    MOV AH, 2
    INT 21H
                                 
    ;DIV X AND Y
    SUB X, 48
    SUB Y, 48
    MOV AX, 0
    MOV AL, X   ;AL DIVIDEND
    MOV BL, Y   ;BL DIVISOR
    DIV BL      ; AL / BL                    
                                 
    ;PRINT THE UPDATED VALUE OF Z
    MOV X, AH   ; REMAINDER IN AH
    MOV Y, AL   ; QUOTIENT IN AL
    ADD X, 48   
    ADD Y, 48
    ;OUTPUT QUOTIENT
    MOV DL, Y   
    MOV AH, 2
    INT 21H
    MOV DL, CR
    MOV AH, 2
    INT 21H
    MOV DL, LF
    MOV AH, 2
    INT 21H
    ;OUTPUT REMAINDER
    MOV DL, X
    MOV AH, 2
    INT 21H
      
      
    ;DOS EXIT
    MOV AH, 4CH
    INT 21H
    
MAIN ENDP
END MAIN
