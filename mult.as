        lw      0   1   0       store final result
        lw      0   2   mcand        
        lw      0   3   mplier       
        lw      0   4   mask       
        lw      0   5   0       store nor1 and final nor outcome            
        lw      0   6   count   count from 0 to 14
        lw      0   7   0       store nor2 outcome and store 1 when needed
        nor     3   3   5       this line and next 2 lines are doing mask & mplier nor(~reg3, ~reg4)
        nor     4   4   7
        nor     5   7   5
        lw      0   7   1       loading 1 into reg7
        add     6   7   6       adding reg6(current count) to reg7(1) and storing in reg6 to increment count
        beq     5   0   loop
        add     1   2   1
loop    add     4   4   4       add reg4(mask) to itself to left shift it by 1
        add     2   2   2       left shift mcand and store it in reg2
        nor     3   3   5       this line and next 2 lines are doing mask & mplier nor(~reg3, ~reg4)
        nor     4   4   7
        nor     5   7   5
        lw      0   7   1       loading 1 into reg7  
        add     6   7   6       adding reg6(current count) to reg7(1) and storing in reg6 to increment count
        lw      0   7   maxbit  set reg7 to max bit allowed
        beq     6   7   done    compare reg6(current count) to reg7( max bit allowed) to check if done
        beq     5   0   loop    if reg5(outcome to mplier & mask) == 0, restart the loop
        add     1   2   1       if previous line didn't run, program ends up here and adds result with mplier
        beq     0   0   loop    if this line is reached, means not done, restart loop
done    halt
mcand   .fill   6203
mplier  .fill   1429
mask    .fill   1
count   .fill   0
maxbit  .fill   2