.start:
    movi $4, %r1        ;int sprite_skin = 4
    store %r1, 4(%rpp)
    movi $1, %r3        ;int sprite_enabled = 1;
    store %r3, 3(%rpp)  ;enable sprite
    
   
    movi $7, %r1        ;int sprite_skin = 7
    store %r1, 8(%rpp)
    movi $1, %r3        ;int sprite_enabled = 1;
    store %r3, 7(%rpp)  ;enable sprite 2
    
    movi $50, %r15       ; set sprite 2 x to 50, y to 120
    movi $120, %r16
    store %r15, 9(%rpp)
    store %r16, 10(%rpp)
    
    
    movi $10, %r1        ;modified y value
    movi $0, %r2        ;lower y bound
    movi $463, %r3      ;upper y bound
    movi $1, %r4        ;y flag value, 1 means add, 0 means subtract
    movi $20, %r5        ;modified x value
    movi $0, %r6        ;lower x bound
    movi $624, %r7      ;upper x bound
    movi $1, %r8        ;x flag value, 1 means add, 0 means subtract

    
.gameLoop:


    load 0(%rcp), %r10  ;store controller values in r10
    mov %r10, %r11      ; transfer to register 10 (up)
    mov %r10, %r12      ; and 11 (down) for bitwise operations
    
    andi $1, %r11      ;1 means up is pressed
    andi $2, %r12      ;2 means down is pressed

    movi $1, %r13
    jne %r13, %r11, upNotPressed
    subi $2, %r16
    
.upNotPressed:

    movi $2, %r13
    jne %r13, %r12, downNotPressed
    addi $2, %r16
    
.downNotPressed:




    store %r15, 9(%rpp)
    store %r16, 10(%rpp)
    store %r1, 6(%rpp)
    store %r5, 5(%rpp)
    jg %r2, %r1, greater     ; if <= lower bound, set flag to 1
    movi $1, %r4
    
    
.greater:
    jl %r3, %r1, less         ; if >= upper bound, set flag to 0
    movi $0, %r4
    
    
.less:
    jne %r4, %r2, doAdd   ; if flag is 0, subtract, else add
    subi $2, %r1
    j xStart
    
.doAdd:
    addi $2, %r1
    
    
.xStart:
    jg %r6, %r5, greater1    ; if <= lower bound, set flag to 1
    movi $1, %r8
    
    
.greater1:
    jl %r7, %r5, less1        ; if >= upper bound, set flag to 0
    movi $0, %r8
    
    
.less1:
    jne %r8, %r6, ne   ; if flag is 0, subtract, else add
    subi $2, %r5
    j vblank
    
    
.ne:
    addi $2, %r5
    
    
.vblank:
    wvb
    j gameLoop
