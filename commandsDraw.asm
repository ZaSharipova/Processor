CALL :main
HLT

:main   
PUSH 1024        ; размер RAM
POPR RAX         ; RAX = 1024
PUSH 0
POPR RBX         ; RBX = 0 

:fill  
PUSH 0
POPM [RBX]       ; записать 0 в RAM[RBX]
PUSHR RBX
PUSH 1
ADD
POPR RBX
PUSHR RBX
PUSHR RAX
JB :fill         ; пока RBX < RAX

IN
POPR RDX         ; ввод радиуса
CALL :checkr
PUSHR RDX
PUSHR RDX
MUL
POPR RDX
PUSH 0
POPR RAX         ; x = 0
PUSH 0
POPR RBX         ; y = 0

:draw   
CALL :line
PUSHR RBX
PUSH 1
ADD
POPR RBX
PUSHR RBX
PUSH 32
JB :draw        ; пока y < 32
ROUT
RET

:line  
PUSH 0
POPR RAX        ; x = 0

:line_loop
CALL :check      ; заполнение RAM[x,y] если нужно
PUSHR RAX
PUSH 1
ADD
POPR RAX
PUSHR RAX
PUSH 32
JB :line_loop   ; пока x < 32
RET

:check  
PUSHR RAX
PUSH 16
SUB
PUSHR RAX
PUSH 16
SUB
MUL
PUSHR RBX
PUSH 16
SUB
PUSHR RBX
PUSH 16
SUB
MUL
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
POPM [RCX]       ; записать 1 в RAM[RCX]
RET

:checkr 
PUSHR RDX
PUSH 16
JA :err
RET

:err   
PUSH 69
OUT
PUSH 82
OUT
PUSH 82
OUT
PUSH 10
OUT
RET
