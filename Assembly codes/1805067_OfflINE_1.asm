                      .MODEL SMALL 
.STACK 100H 
.DATA

PROMPT_N DB 'ENTER N (N MUST BE GREATER THAN 0):$'
PROMPT_ARR DB 'ENTER ARRAY ELEMENTS:$'
MSG_SORT DB 'ARRAY AFTER SORTING:',0DH,0AH,'$'
PROMPT_SEARCH DB 'ENTER A NUMBER TO SEARCH: $'
NOTFOUND_MSG DB 'NOT FOUND',0DH,0AH,'$'
MSG DB 'INVALID INPUT$'
ARR DW 100 DUP(0)  
N DW ?
CR EQU 0DH
LF EQU 0AH
SIGN DB ?
NEWLINE DB 0DH, 0AH,'$'


.CODE 
MAIN PROC 
    MOV AX, @DATA
    MOV DS, AX
    
    MOV AH, 9
    LEA DX, PROMPT_N
    INT 21H
    
    INPUT_N:
        CALL INPUT
        MOV  N, BX
        CMP N,0
        JLE INVALID ;IF N IS LESS THAN OR EQUAL TO 0 PROMPT AGAIN FOR INPUT
        
    
    
    ;TAKING INPUT IN ARRAY
     
    PUSH SI
    MOV CX,N
    ARR_INPUT:
        PUSH CX      ;CX IS CHANGED INSIDE INPUT, SO WE PUSH IT IN THE STACK
        CALL INPUT
        MOV ARR[SI],BX
        ADD SI,2
        POP CX      ;WE RETRIEVE CX's ORIGINAL VALUE
        LOOP ARR_INPUT 
    POP SI    
    
    
    
    
    
    ;INSERTION SORT
    MOV CX,2    ;WE START FROM SECOND ELEMENT
    PUSH SI
    ADD SI,2
    SORT_OUTER:
        MOV AX,ARR[SI]     ;CURRENT ELEMENT IS STORED IN AX. key = ARR[i]
        MOV BX,CX ; CX IS THE INDEX OF THE CURRENT ELEMENT
        DEC BX    ; WE SET BX TO BE ONE LESS THAN CX
        PUSH SI
        SUB SI,2; WE SET SI SO THAT ARR[SI] POINT TO PREVIOUS ELEMENT
        SORT_INNER:
            CMP BX,0
            JE END_INNER
            
            CMP AX,ARR[SI] ;COMPARE KEY TO ARR[j]
            JGE END_INNER
            MOV DX,ARR[SI]
            ADD SI,2
            MOV ARR[SI],DX  ;ARR[j+1]=ARR[j];
            SUB SI,2
            DEC BX        ;j--
            SUB SI,2
            JMP SORT_INNER  
        END_INNER:
   
        ADD SI,2
        MOV ARR[SI],AX  ;ARR[j+1]=key;
        POP SI
  
        ADD SI,2
        INC CX
        CMP CX,N
        JLE SORT_OUTER
    END_OUTER:
    

    MOV AH,9
    LEA DX,MSG_SORT
    INT 21H 
    POP SI
    
    PUSH SI
    MOV CX,N
    ARR_OUTPUT:
        PUSH CX
        MOV BX, ARR[SI]
        CALL OUTPUT
        ADD SI,2
        POP CX
        LOOP ARR_OUTPUT
         
    POP SI
    MOV AH, 9
    LEA DX,NEWLINE
    INT 21H   
    
    

    BIN_SEARCH:
    
        MOV AH, 9
        LEA DX,PROMPT_SEARCH
        INT 21H
        
        CALL INPUT ;INPUT WILL BE IN BX
        
        MOV CX,0    ; CX=left; STARTS WITH 0
        MOV DX,N    ; DX=right
        DEC DX      ;STARTS WITH N-1
        

        BIN_SEARCH_LOOP:    ; CX->left,AX->mid,DX->right
                        
            PUSH SI
            CMP CX,DX       ;break when left>right
            JG NOT_FOUND
            MOV AX,CX   ;AX=CX
            ADD AX,DX   ;AX=CX+DX
            SHR AX,1 ;RIGH SHIFT AX 1 PLACE.Then AX=mid
            
            ADD SI,AX
            ADD SI,AX
            CMP BX,ARR[SI]
            JE FOUND_BIN_SEARCH            
            JG RIGHT_BIN_SEARCH
            JL LEFT_BIN_SEARCH
            
            RIGHT_BIN_SEARCH:
                INC AX 
                MOV CX,AX ; left=mid+1
                POP SI
                JMP BIN_SEARCH_LOOP
    
            LEFT_BIN_SEARCH:
                DEC AX
                MOV DX,AX ; right=mid-1
                POP SI
                JMP BIN_SEARCH_LOOP
    
            FOUND_BIN_SEARCH: ; index already in AX
                MOV BX,AX
                CALL OUTPUT
                INT 21H
                POP SI
                
                MOV AH,9
                LEA DX,NEWLINE
                INT 21H
                
                JMP END_BIN_SEARCH
        
            NOT_FOUND:
                MOV AH,9
                LEA DX,NOTFOUND_MSG
                INT 21H
                POP SI
                JMP END_BIN_SEARCH
        

            END_BIN_SEARCH:
                JMP BIN_SEARCH
    INVALID:
        MOV AH, 9
        LEA DX, MSG
        INT 21H
    
    EXIT:  
    	; interrupt to exit
        MOV AH, 4CH
        INT 21H
    
  
MAIN ENDP
     INPUT PROC
        ; fast BX = 0
        XOR BX, BX
        MOV SIGN, 0
        
        INPUT_LOOP:
            ; char input 
            MOV AH, 1
            INT 21H
            
            CMP AL,'-'
            JNE PROCESS_INPUT
            MOV SIGN,1
            JMP INPUT_LOOP
        
        
        PROCESS_INPUT: ;STORES INPUT IN BX
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
            CMP SIGN,1
            JNE PRINT_NEWLINE 
            NEG BX
            MOV SIGN,0
            
        PRINT_NEWLINE:
            MOV AH,2
            MOV DL,CR
            INT 21H
            MOV AH,2
            MOV DL,LF
            INT 21H
        RET
        INPUT ENDP
     
     OUTPUT PROC    ;OUTPUT IS IN BX
        MOV CX,0
        
        CMP BX,0
        JNL PROCESS_OUTPUT
        MOV AH,2
        MOV DL,'-'
        INT 21H
        NEG BX  ;MAKE BX POSITIVE
        
        
        PROCESS_OUTPUT:
            ; fast DX = 0
            XOR DX, DX
            INC CX      ;CX IS USED TO KEEP COUNT OF NUMBER OF CHARACTERS IN THIS NUMBER
            MOV AX, BX ;AX DIVIDEND
            MOV BX, 10  ;BX DIVISOR
            DIV BX      ;AX = AX / BX                    
            ;NOW QUOTIENT IS IN  AX, AND REMAINDER IS IN DX
            PUSH DX         
            MOV BX, AX
            CMP BX, 0
            JNE PROCESS_OUTPUT
                 
        MOV AH,2
        PRINT_OUTPUT:
            POP DX
            ADD DX,'0'
            INT 21H
        LOOP PRINT_OUTPUT
        
        MOV DX,' ' ;PRINT SPACE AFTER EVERY NUMBER
        INT 21H
        
        RET
        OUTPUT ENDP
END MAIN 