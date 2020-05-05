movi $3, %r1            #int sprite_skin = 3
store %r1, 4(%rpp)
movi $120, %r1          #int sprite_x = 120;
store %r1, 5(%rpp)      
movi $0, %r2          #int sprite_y = 100;
store %r2, 6(%rpp)      #y pos is stored in %r2
movi $1, %r3            #int sprite_enabled = 1;
store %r3, 3(%rpp)      #enable sprite
movi $1, %r1            #up flag in r1 (1 => down)
movi $1, %r5            #static one
movi $0, %r3            #min y position
movi $460, %r4          #max y position
store %r2, 6(%rpp)
jne %r2, %r3, $15        #if equal to 0, set up flag to 1 (go down, add)
movi $1, %r1
j $17
jne %r2, %r4, $18
movi $0, %r1
jne %r1, %r5, $21        #if up flag isn't one
addi $1, %r2
j $22
subi $1, %r2
wvb
j $12
