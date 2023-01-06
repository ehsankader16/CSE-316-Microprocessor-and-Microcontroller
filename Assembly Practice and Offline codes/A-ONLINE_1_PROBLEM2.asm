.MODEL SMALL
.STACK 100H

.DATA
    CR EQU 0DH
    LF EQU 0AH
    
    LINE1 DB '*****$'
    LINE2 DB '* C *$'
    LINE3 DB '*ABC*$'
    LINE4 DB '* C *$'
    LINE5 DB '*****$'
    LINE  DB CR, LF 
    
.CODE

MAIN PROC
;INITIALIZE DS
    MOV AX, @DATA
    MOV DS, AX
       
    ;PRINT LINE 1-5
    LEA DX, LINE1
    MOV AH, 9
    INT 21H     
    
    ; appending a new line
    MOV DX, CR
    MOV AH, 2
    INT 21h  
    MOV DX, LF
    MOV AH, 2
    INT 21h
    
    LEA DX, LINE2
    MOV AH, 9
    INT 21H  
    
    ; appending a new line
    MOV DX, CR
    MOV AH, 2
    INT 21h  
    MOV DX, LF
    MOV AH, 2
    INT 21h
    
    LEA DX, LINE3
    MOV AH, 9
    INT 21H 
    
    ; appending a new line
    MOV DX, CR
    MOV AH, 2
    INT 21h  
    MOV DX, LF
    MOV AH, 2
    INT 21h
    
    LEA DX, LINE4
    MOV AH, 9
    INT 21H
              
    ; appending a new line
    MOV DX, CR
    MOV AH, 2
    INT 21h  
    MOV DX, LF
    MOV AH, 2
    INT 21h
    
    LEA DX, LINE5
    MOV AH, 9
    INT 21H
    
    ;DOX EXIT
    MOV AH, 4CH
    INT 21H
MAIN ENDP
    END MAIN