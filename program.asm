movi $3, %r1                    #set sprite 1's skin to 3
store %r1, 4(%rpp)
movi $320, %r1                  #set sprite 1's x position to 320
store %r1, 5(%rpp)
movi $1, %r1                    #enable sprite 1
store %r1, 3(%rpp)
movi $5, %r1                    #save sprite y position in r1
store %r1, 6(%rpp)              #store y position in ppu register
addi $5, %r1
wvb                             #wait on vblank interrupt
addi $0, %rsp
wvb
j $7                            #jump back to store statement
