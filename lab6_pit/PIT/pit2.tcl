wave add / -radix hex
isim force add Bus2IP_Clk 0 -value 1 -time 10ns -repeat 20ns

# Reset system
isim force add Bus2IP_Resetn 0
run 40ns
isim force add Bus2IP_Resetn 1
run 20ns

