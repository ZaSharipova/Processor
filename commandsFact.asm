        CALL :main
        HLT

:main      
        IN
        POPR RAX
        CALL :countfact
        OUT
        RET

:countfact     
        PUSHR RAX
        PUSH 1
        JBE :basecase
        PUSHR RAX
        PUSH 1
        SUB
        POPR RAX
        CALL :countfact
        PUSHR RAX
        PUSH 1
        ADD
        POPR RAX
        PUSHR RAX
        MUL
        RET

:basecase     
        PUSH 1
        RET