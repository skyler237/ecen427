
#restart
isim force add Bus2IP_Clk 1 -value 0 -time 5 ns -repeat 10 ns
isim force add data 1 -value 0 -time 8 us -repeat 16 us


#dhecks to see if the clock increments properly, and rolls over.
#Checks to make sure that the an bits change, and the blank logic is sound
run 1ms