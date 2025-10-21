CALL :main
HLT

:main   
    PUSH 1024
    POPR RAX
    PUSH 0
    POPR RBX

:fill  
    PUSH 0
    POPM [RBX]
    PUSHR RBX
    PUSH 1
    ADD
    POPR RBX
    PUSHR RBX
    PUSHR RAX
    JB :fill

    IN
    POPR RDX
    IN
    POPR REX
    IN
    POPR RFX
    CALL :checkr
    PUSHR RDX
    PUSHR RDX
    MUL
    POPR RDX
    PUSH 0
    POPR RAX
    PUSH 0
    POPR RBX

:draw   
    CALL :line
    PUSHR RBX
    PUSH 1
    ADD
    POPR RBX
    PUSHR RBX
    PUSH 32
    JB :draw
    DRAW
    RET

:line  
    PUSH 0
    POPR RAX

:line_loop
    CALL :check
    PUSHR RAX
    PUSH 1
    ADD
    POPR RAX
    PUSHR RAX
    PUSH 32
    JB :line_loop
    RET

:check  
    PUSHR RAX
    PUSHR REX
    SUB
    SQUARE
    PUSHR RBX
    PUSHR RFX
    SUB
    SQUARE
    ADD
    PUSHR RDX
    JBE :do
    RET

:do     
    PUSHR RAX
    PUSHR RBX
    PUSH 32
    MUL
    ADD
    POPR RCX
    PUSH 1
    POPM [RCX]
    RET

:checkr 
    PUSHR RDX
    PUSH 16
    JA :err
    RET

:err   
    PUSH 69
    OUTC
    PUSH 82
    OUTC
    PUSH 82
    OUTC
    HLT
    RET