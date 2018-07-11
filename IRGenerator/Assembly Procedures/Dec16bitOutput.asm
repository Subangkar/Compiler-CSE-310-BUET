;--------------------------------  OUTDEC  --------------------------------;


OUTDEC PROC
; this procedure will display a decimal number
; input : AX
; output : none
; uses : MAIN

  PUSH BX                        
  PUSH CX                        
  PUSH DX                        

  CMP AX, 0                      ; compare AX with 0
  JGE @START                     ; jump to label @START if AX>=0

  MOV DL, "-"                    ; set DL=\'-\'
  CALL OUTDEC_printByteIn_DL

  NEG AX                         ; take 2\'s complement of AX

  @START:                        ; jump label

    XOR CX, CX                     ; CX counts number of digits
    MOV BX, 10                     ; set BX=10

  @OUTPUT:                       
    XOR DX, DX                   ; clear DX
    DIV BX                       ; divide AX by BX
    PUSH DX                      ; push DX onto the STACK
    INC CX                       
    OR AX, AX                    ; take OR of Ax with AX
    JNE @OUTPUT                    ; jump to label @OUTPUT if ZF=0

  MOV AH, 2                      ; set output function

  @DISPLAY:                      
    POP DX                       
    OR DL, 30H                   ; convert decimal to ascii code
    INT 21H                      
    LOOP @DISPLAY                  ; jump to label @DISPLAY if CX!=0

  POP DX                         
  POP CX                         
  POP BX                         

  RET                           
OUTDEC ENDP




; prints the Byte in DL
OUTDEC_printByteIn_DL proc
  PUSH AX

  MOV AH,2
  INT 21h

  POP AX    
  RET
OUTDEC_printByteIn_DL endp