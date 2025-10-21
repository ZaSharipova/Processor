CALL :main
HLT

:main
    IN        ; push A
    POPR RAX
    IN        ; push B
    POPR RBX
    IN        ; push C
    POPR RCX

    CALL :nulla  ; проверяем на нулевой А

    CALL :discriminant
    CALL :findroot1
    CALL :findroot2
    RET

:nulla
    PUSHR RAX
    PUSH 0
    JE :dolinear
    RET

:discriminant
    PUSHR RBX
    PUSHR RBX
    MUL
    PUSH 1
    PUSHR RAX
    MUL
    PUSH 4
    MUL
    PUSHR RCX
    MUL
    SUB
    POPR RDX
    PUSHR RDX
    PUSH 0
    CALL :checkdis
    PUSHR RDX
    SQRT
    POPR RDX
    PUSHR RDX
    RET

:findroot1
    PUSH 0    ; root1
    PUSHR RBX
    SUB
    ADD
    PUSH 2
    PUSHR RAX
    MUL
    DIV
    OUT
    PUSHR RFX
    PUSH 1
    JE :doout
    RET

:findroot2
    PUSH 0    ; root2
    PUSHR RBX
    SUB
    PUSHR RDX
    SUB
    PUSH 2
    PUSHR RAX
    MUL
    DIV
    OUT
    PUSH 1
    RET

:checkdis
    PUSHR RDX
    PUSH -1
    JB :printzeroroots
    JE :checkzerodis
    PUSH 2
    OUT
    RET

:printzeroroots
    PUSH 0
    OUT
    HLT

:dolinear
    PUSHR RBX
    PUSH 0
    JE :checkc
    PUSH 0
    PUSHR RCX
    SUB
    PUSHR RBX

    DIV
    PUSH 1
    OUT
    OUT
    HLT
    RET

:checkc
    PUSHR RCX
    PUSH 0
    JE :printinfroots
    PUSH 0
    OUT
    HLT
    RET

:printinfroots
    PUSH 12345678
    OUT
    HLT

:checkzerodis
    PUSHR RDX 
    PUSH 0
    JE :printzerodis

:printzerodis
    PUSH 1
    OUT
    PUSH 1
    POPR RFX
    RET

:doout
    HLT