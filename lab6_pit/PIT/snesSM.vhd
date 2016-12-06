library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;


entity user_logic is
  generic
  (
    -- ADD USER GENERICS BELOW THIS LINE ---------------
    --USER generics added here
    -- ADD USER GENERICS ABOVE THIS LINE ---------------

    -- DO NOT EDIT BELOW THIS LINE ---------------------
    -- Bus protocol parameters, do not add to or delete
    C_NUM_REG                      : integer              := 4;
    C_SLV_DWIDTH                   : integer              := 32
    -- DO NOT EDIT ABOVE THIS LINE ---------------------
  );
  port
  (
    -- ADD USER PORTS BELOW THIS LINE ------------------
    latch     :   out std_logic;
    ctr_clk   :   out std_logic;
    data      :   in  std_logic;
    intr      :   out std_logic;
    -- ADD USER PORTS ABOVE THIS LINE ------------------

    -- DO NOT EDIT BELOW THIS LINE ---------------------
    -- Bus protocol ports, do not add to or delete
    Bus2IP_Clk                     : in  std_logic;
    Bus2IP_Resetn                  : in  std_logic;
    Bus2IP_Data                    : in  std_logic_vector(C_SLV_DWIDTH-1 downto 0);
    Bus2IP_BE                      : in  std_logic_vector(C_SLV_DWIDTH/8-1 downto 0);
    Bus2IP_RdCE                    : in  std_logic_vector(C_NUM_REG-1 downto 0);
    Bus2IP_WrCE                    : in  std_logic_vector(C_NUM_REG-1 downto 0);
    IP2Bus_Data                    : out std_logic_vector(C_SLV_DWIDTH-1 downto 0);
    IP2Bus_RdAck                   : out std_logic;
    IP2Bus_WrAck                   : out std_logic;
    IP2Bus_Error                   : out std_logic
    -- DO NOT EDIT ABOVE THIS LINE ---------------------
  );

  attribute MAX_FANOUT : string;
  attribute SIGIS : string;

  attribute SIGIS of Bus2IP_Clk    : signal is "CLK";
  attribute SIGIS of Bus2IP_Resetn : signal is "RST";

end entity user_logic;

------------------------------------------------------------------------------
-- Architecture section
------------------------------------------------------------------------------

architecture IMP of user_logic is

  --USER signal declarations added here, as needed for user logic
  --USER signal declarations added here, as needed for user logic
  
  --   ____
  --__|    |__________________________________________________ 12us latch
  --           __    __    __    __    __    __    __    __
  --__________|  |__|  |__|  |__|  |__|  |__|  |__|  |__|  |__ 12 ctr_clk cycle (driven by latch, 16 total)
  --__________________________________________________________
  --__|___B___|__Y__|_Sel_|_Str_|__U__|__D__|__L__|__R__|_____ Data Signal (Low asserted)
  --
  --
  
  --states in the SM
	constant wait_for_latch : integer range 0 to 17 := 0;
	constant wait0 : integer range 0 to 17 := 1;
	constant btn0 : integer range 0 to 17 := 2;
	constant wait1 : integer range 0 to 17 := 3;
	constant btn1 : integer range 0 to 17 := 4;
	constant wait2 : integer range 0 to 17 := 5;
	constant btn2 : integer range 0 to 17 := 6;
	constant wait3 : integer range 0 to 17 := 7;
	constant btn3 : integer range 0 to 17 := 8;
	constant wait4 : integer range 0 to 17 := 9;
	constant btn4 : integer range 0 to 17 := 10;
	constant wait5 : integer range 0 to 17 := 11;
	constant btn5 : integer range 0 to 17 := 12;
	constant wait6 : integer range 0 to 17 := 13;
	constant btn6 : integer range 0 to 17 := 14;
	constant wait7 : integer range 0 to 17 := 15;
	constant btn7 : integer range 0 to 17 := 16;
	constant wait8 : integer range 0 to 17 := 17;
	
	signal sm_next, sm_curr : integer range 0 to 17 := 0;
	
	signal clock_next, clock_curr : std_logic := '0';
	signal latch_next, latch_curr : std_logic := '0';
  signal intr_next, intr_curr   : std_logic := '0';
	signal start_controller_clock : std_logic := '0';
	signal slv_reg0_curr, slv_reg0_next	:	std_logic_vector(31 downto 0) := (others => '0');
	signal counter_next, counter_curr: unsigned(10 downto 0) := (others => '0');
	signal latch_count_next, latch_count_curr: unsigned(20 downto 0) := (others => '0');
  ------------------------------------------
  -- Signals for user logic slave model s/w accessible register example
  ------------------------------------------
  signal slv_reg0                       : std_logic_vector(C_SLV_DWIDTH-1 downto 0);
  signal slv_reg1                       : std_logic_vector(C_SLV_DWIDTH-1 downto 0);
  signal slv_reg2                       : std_logic_vector(C_SLV_DWIDTH-1 downto 0);
  signal slv_reg3                       : std_logic_vector(C_SLV_DWIDTH-1 downto 0);
  signal slv_reg_write_sel              : std_logic_vector(3 downto 0);
  signal slv_reg_read_sel               : std_logic_vector(3 downto 0);
  signal slv_ip2bus_data                : std_logic_vector(C_SLV_DWIDTH-1 downto 0);
  signal slv_read_ack                   : std_logic;
  signal slv_write_ack                  : std_logic;

begin

  --USER logic implementation added here
  -----------------------------------------------------------------------------------------------------
  process (sm_curr, latch_curr, clock_curr, data, slv_reg0_curr)
	begin
		slv_reg0_next <= slv_reg0_curr;
    intr_next <= '0';
		case sm_curr is
			when wait_for_latch =>
				if latch_curr = '1' then
					sm_next <= wait0;
				end if;
			when wait0 =>
				if latch_curr = '0' then
					sm_next <= btn0;
					slv_reg0_next(0) <= not data;
				end if;
			when btn0 =>
				if clock_curr = '1' then
					sm_next <= wait1;
				end if;
			when wait1 =>
				if clock_curr = '0' then
					sm_next <= btn1;
					slv_reg0_next(1) <= not data;
				end if;
			when btn1 =>
				if clock_curr = '1' then
					sm_next <= wait2;
				end if;
			when wait2 =>
				if clock_curr = '0' then
					sm_next <= btn2;
					slv_reg0_next(2) <= not data;
				end if;
			when btn2 =>
				if clock_curr = '1' then
					sm_next <= wait3;
				end if;
			when wait3 =>
				if clock_curr = '0' then
					sm_next <= btn3;
					slv_reg0_next(3) <= not data;
				end if;
			when btn3 =>
				if clock_curr = '1' then
					sm_next <= wait4;
				end if;
			when wait4 =>
				if clock_curr = '0' then
					sm_next <= btn4;
					slv_reg0_next(4) <= not data;
				end if;
			when btn4 =>
				if clock_curr = '1' then
					sm_next <= wait5;
				end if;
			when wait5 =>
				if clock_curr = '0' then
					sm_next <= btn5;
					slv_reg0_next(5) <= not data;
				end if;
			when btn5 =>
				if clock_curr = '1' then
					sm_next <= wait6;
				end if;
			when wait6 =>
				if clock_curr = '0' then
					sm_next <= btn6;
					slv_reg0_next(6) <= not data;
				end if;
			when btn6 =>
				if clock_curr = '1' then
					sm_next <= wait7;
				end if;
			when wait7 =>
				if clock_curr = '0' then
					sm_next <= btn7;
					slv_reg0_next(7) <= not data;
				end if;
			when btn7 =>
				if clock_curr = '1' then
					sm_next <= wait8;
				end if;
			when wait8 =>
				if clock_curr = '0' then
          intr_next <= '1';
					sm_next <= wait_for_latch;
				end if;
		end case;
	end process;
			
	process (Bus2IP_Clk)
	begin
		if (Bus2IP_Clk'event and Bus2IP_Clk='1') then
			sm_curr <= sm_next;
		end if;
	end process;
	
	
	--Implement the counter
	process(Bus2IP_Clk)
	begin
		if (start_controller_clock = '0') then
			counter_curr <= (others => '0');
		elsif (Bus2IP_Clk'event and Bus2IP_Clk='1') then
			counter_curr <= counter_next;
		end if;
	end process;
	counter_next <= 	(others => '0') when counter_curr	= 1199	else
										unsigned(counter_curr) + 1;
	process(Bus2IP_Clk)
	begin
		if (Bus2IP_Clk'event and Bus2IP_Clk='1') then
			clock_curr <= clock_next;
		end if;
	end process;
	clock_next <=			'0' when latch_count_curr	>= 11400	else
                    '0' when counter_curr	<= 599	else
										'1';
                    
										
	ctr_clk <= clock_curr;
	--Next state logic
  
	process (Bus2IP_Clk)
	begin
		if (Bus2IP_Clk'event and Bus2IP_Clk='1') then
			intr_curr <= intr_next;
		end if;
	end process;
	
	process (Bus2IP_Clk)
	begin
		if (Bus2IP_Clk'event and Bus2IP_Clk='1') then
			slv_reg0_curr <= slv_reg0_next;
		end if;
	end process;
	
  slv_reg0 <= slv_reg0_curr;
  
  intr <= intr_curr;
	
	process(Bus2IP_Clk)
	begin
		if (Bus2IP_Clk'event and Bus2IP_Clk='1') then
			latch_count_curr <= latch_count_next;
			latch_curr <= latch_next;
		end if;
	end process;
	latch_count_next <= 	(others => '0') when latch_count_curr	= 1666666	else
										unsigned(latch_count_curr) + 1;
	latch_next <= '1' when 	latch_count_curr <= 1199 else
								'0';
	start_controller_clock <= '0' when latch_count_curr <= 1199 else
														'1';
														
	latch <= latch_curr;
	
	end IMP;