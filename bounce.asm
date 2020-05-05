movi $3, %r1            #int sprite_skin = 3
store %r1, 4(%rpp)
movi $120, %r1          #int sprite_x = 120;
store %r1, 5(%rpp)      #x pos is stored in %r1
movi $100, %r2          #int sprite_y = 100;
store %r2, 6(%rpp)      #y pos is stored in %r2
movi $1, %r3            #int sprite_enabled = 1;
store %r3, 3(%rpp)
movi $1, %r3            #int is_right = 1 #store is_right in %r3
movi $1, %r4            #int is_down = 1 #store is_down in %r4
movi $10, %r5            #store bounds in r5,r6,r7,r8
movi $620, %r6
movi $460, %r7
movi $1, %r8
store %r1, 5(%rpp)      #store x position
store %r2, 6(%rpp)      #store y position
jl %r6, %r1, $18        #-----------------------------
movi $1, %r3
jg %r5, %r1, $20
movi $0, %r3            # seeing if within bounds, checking
jl %r7, %r2, $22        # if direction needs to be changed
movi $1, %r4
jg %r5, %r2, $24
movi $0, %r4            #-----------------------------
jne %r3, %r8, $27       #-----------------------------
addi $1, %r1            #   change sprite x position
j $28                   #
subi $1, %r1            #-----------------------------
jne %r4, %r8, $31       #-----------------------------
addi $1, %r2            #   change sprite y position
j $32                   #
subi $1, %r2            #-----------------------------
wvb                     #wait on vblank
j $14                   #jump to store statements
