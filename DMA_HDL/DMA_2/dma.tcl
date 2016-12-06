wave add / -radix hex
isim force add Bus2IP_Clk 0 -value 1 -time 10ns -repeat 20ns

# Reset system
isim force add Bus2IP_Resetn 0
run 40ns
isim force add Bus2IP_Resetn 1
isim force add mst_byte_we 0x00
run 20ns

# Set source address
isim force add slv_reg_write_sel 0x80
isim force add Bus2IP_BE 0xf
isim force add Bus2IP_Data 0x00000001
run 100ns

# Set dest address
isim force add slv_reg_write_sel 0x40
isim force add Bus2IP_Data 0x10000000
run 100ns

# Set control reg (read operation)
isim force add mst_byte_we 0x01
isim force add slv_reg_write_sel 0x0
isim force add Bus2IP_Data 0x00000001
run 100ns
isim force add mst_byte_we 0x00

# Set transfer length
isim force add slv_reg_write_sel 0x20
isim force add Bus2IP_Data 0x00000004
run 100ns

# GO
isim force add mst_byte_we 0x8000
isim force add slv_reg_write_sel 0x0
isim force add Bus2IP_Data 0x0A000000
run 100ns
isim force add mst_byte_we 0x00

# Let it go, let it go!!
for {set i 0} {$i < 16} {incr i} {
	isim force add Bus2IP_Mst_CmdAck 1
	run 40 ns
	isim force add Bus2IP_Mst_Cmplt 1
	run 40 ns
	isim force add Bus2IP_Mst_Cmplt 0
	isim force add Bus2IP_Mst_CmdAck 0
	run 100 ns
}
run 6us
