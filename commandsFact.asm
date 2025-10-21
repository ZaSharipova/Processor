        CALL :main
        HLT

:main      
        IN
        POPR RAX
        CALL :countfact
        PUSHR RBX
        OUT
        RET

:countfact     
        PUSHR RAX
        PUSH 1
        JBE :basecase
        PUSHR RAX
        PUSHR RAX
        PUSH 1
        SUB
        POPR RAX
        CALL :countfact
        PUSHR RBX
        MUL
        POPR RBX
        RET

:basecase     
        PUSH 1
        POPR RBX
        RET