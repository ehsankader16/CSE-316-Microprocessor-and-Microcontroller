.MODEL SMALL

.STACK 100H

.DATA
    CR EQU 0DH
    LF EQU 0AH  
    MSG DB 'ENTER A CHARACTER FROM A-H$'
    X DB ?                         
            
MAIN PROC
    
    MOV AX, @DATA
    MOV DS, AX
    
    ;PROMPT MESSAGE
    LEA DX, MSG
    MOV AH, 9
    INT 21H
     
        
    ;INPUT (A TO H)
    MOV AH,1
    INT 21H
    MOV X,AL  ;(LET X ='B')
        
    ;CONVERT X TO DECIMAL (7 TO 0)    
    SUB X,'A' ;X=X-'A'(66-65=1)
    MOV BL,7
    SUB BL,X  ;7-X (7-1)
    ADD BL,'0' ;7-x+'0' (6+30 = 36('6')
    
    ;OUTPUT
    MOV AH,2
    MOV DL, '1' ;WILL PRINT 1
    INT 21H
    MOV DL, BL  ;WILL PRINT 6
    INT 21H     ;B = 16 IN REVERSE HEPTADECIMAL
    
    
    MOV AH, 4CH
    INT 21H
    MAIN ENDP
END MAIN