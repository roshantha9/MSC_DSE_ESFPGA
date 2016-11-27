library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity Driver_8Bit is
	Generic ( CLK_MHZ : NATURAL := 33 );
Port (
	clk 		: in STD_LOGIC;
	data_in 	: in STD_LOGIC_VECTOR (7 downto 0);
	RS_in 		: in STD_LOGIC;
	load 		: in STD_LOGIC;
	ack_busy 	: out STD_LOGIC;
	LCD_data 	: out STD_LOGIC_VECTOR (3 downto 0);
	LCD_RS 		: out STD_LOGIC;
	LCD_RW 		: out STD_LOGIC;
	LCD_E	 	: out STD_LOGIC
);
end Driver_8Bit;

architecture Behavioral of Driver_8Bit is
	type STATE_TYPE is ( INIT, INIT_WAIT_1, INIT1, INIT1_WAIT,
						INIT2, INIT2_WAIT, INIT3, INIT_WAIT_2,
						IDLE, MSB, MSB_WAIT, LSB, LSB_WAIT,
						WAIT_FOR_LOAD_LOW );
	
	signal state : STATE_TYPE := INIT;
	subtype STATE_TIMER_TYPE is INTEGER range 0 to (CLK_MHZ*50*1000); -- 50ms max wait
	
	shared variable state_timer : STATE_TIMER_TYPE := 0;
	signal driver_busy : STD_LOGIC := '0';
	signal driver_load : STD_LOGIC := '0';
	signal data_reg : STD_LOGIC_VECTOR (7 downto 0) := (others=>'0');
	signal data_nibble : STD_LOGIC_VECTOR (3 downto 0) := (others=>'0');
	signal RS_reg : STD_LOGIC := '0';
	
begin
	-- State machine
	STATE_PROC: process (clk, load, state) is
	begin
		if rising_edge(clk) then
			if state_timer /= 0 then
				state_timer := state_timer - 1;
			end if;
			
			case state is
				when INIT =>
					if state_timer = 0 then
						state_timer := CLK_MHZ * 50 * 1000; -- 50ms initial delay
						state <= INIT_WAIT_1;
					end if;
				when INIT_WAIT_1 =>
					if state_timer = 0 then
						state <= INIT1;
					end if;
				when INIT1 =>
					if driver_busy = '1' then
						state <= INIT1_WAIT;
					end if;
				when INIT1_WAIT =>
					if driver_busy = '0' then
						state <= INIT2;
					end if;
				when INIT2 =>
					if driver_busy = '1' then
						state <= INIT2_WAIT;
					end if;
				when INIT2_WAIT =>
					if driver_busy = '0' then
						state <= INIT3;
					end if;
				when INIT3 =>
					if driver_busy = '1' then
						state_timer := CLK_MHZ * 15 * 1000; -- 15ms delay after initialisation
						state <= INIT_WAIT_2;
					end if;
				when INIT_WAIT_2 =>
					if state_timer = 0 then
						if load = '1' then
							state <= WAIT_FOR_LOAD_LOW;
						else	
							state <= IDLE;
						end if;
					end if;
				when IDLE =>
					if driver_busy = '0' and load = '1' then
						data_reg <= data_in;
						RS_reg <= RS_in;
						state <= MSB;
					end if;
				when MSB =>
					if driver_busy = '1' then
						state <= MSB_WAIT;
					end if;
				when MSB_WAIT =>
					if driver_busy = '0' then
						state <= LSB;
					end if;
				when LSB =>
					if driver_busy = '1' then
						state <= LSB_WAIT;
					end if;
				when LSB_WAIT =>
					if driver_busy = '0' then
						if RS_reg = '1' then
							state_timer := CLK_MHZ * 45; -- Write operations take 43us
						elsif data_reg (7 downto 2) = "000000" then
							state_timer := CLK_MHZ * 2000; -- Clear or Home operations take 1.53ms in theory
						else
							state_timer := CLK_MHZ * 40; -- Config operations take 39us
						end if;
						
						if load = '1' or state_timer /= 0 then
							state <= WAIT_FOR_LOAD_LOW;
						else
							state <= IDLE;
						end if;
					end if;	
				when WAIT_FOR_LOAD_LOW =>
					if load = '1' or state_timer /= 0 then
						state <= WAIT_FOR_LOAD_LOW;
					else
						state <= IDLE;
					end if;
				end case;
		end if;
	end process STATE_PROC;
	
	with state select
		ack_busy <= '0' when IDLE,
		'1' when others;
	
	with state select
		driver_load <= '1' when MSB | LSB | INIT1 | INIT2 | INIT3,
		'0' when others;
	
	with state select
		data_nibble <= data_reg (7 downto 4) when MSB | MSB_WAIT,
		data_reg (3 downto 0) when LSB | LSB_WAIT,
		"0010" when INIT1 | INIT1_WAIT | INIT2 |
		INIT2_WAIT | INIT3 | INIT_WAIT_2,
		(others=>'0') when others;
	
	
	driver: entity work.Driver_4Bit(Behavioral)
			generic map (CLK_MHZ => CLK_MHZ	)
			port map (
					clk => clk,
					data_in => data_nibble,
					load => driver_load,
					RS_in => RS_reg,
					ack_busy => driver_busy,
					LCD_data => LCD_data,
					LCD_RS => LCD_RS,
					LCD_RW => LCD_RW,
					LCD_E => LCD_E
			);
			
end Behavioral;
