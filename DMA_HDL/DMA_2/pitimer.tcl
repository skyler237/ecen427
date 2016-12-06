wave add / -radix hex
isim force add Bus2IP_Clk 0 -value 1 -time 10ns -repeat 20ns

# Reset system
isim force add Bus2IP_Resetn 0
run 40ns
isim force add Bus2IP_Resetn 1
run 20ns

# Test ctrl register (interrupts, looping, decrementing)
isim force add slv_reg_write_sel 0x4
isim force add Bus2IP_BE 1
isim force add Bus2IP_Data 0x0000000f
run 100ns

# Test no interrupt, no loop
isim force add slv_reg_write_sel 0x8
isim force add Bus2IP_Data 0x00000001
run 600ns

# Test no interrupt, with loop
isim force add slv_reg_write_sel 0x8
isim force add Bus2IP_Data 0x00000005
run 600ns

# Test interrupt, with loop
isim force add slv_reg_write_sel 0x8
isim force add Bus2IP_Data 0x00000007
run 600ns

# Test interrupt, no loop
isim force add slv_reg_write_sel 0x8
isim force add Bus2IP_Data 0x00000003
run 600ns

# Try start and stop
isim force add slv_reg_write_sel 0x8
isim force add Bus2IP_Data 0x00000007
run 100ns
isim force add slv_reg_write_sel 0x8
isim force add Bus2IP_Data 0x00000006
run 100ns
isim force add slv_reg_write_sel 0x8
isim force add Bus2IP_Data 0x00000007
run 100ns

# Test system reset
isim force add Bus2IP_Resetn 0
run 40ns
isim force add Bus2IP_Resetn 1
run 20ns