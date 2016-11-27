library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity Driver_4Bit is
	Generic (
		CLK_MHZ : NATURAL := 33
				);
	Port (
				clk : in STD_LOGIC;
				RS_in : in STD_LOGIC;
				data_in : in STD_LOGIC_VECTOR (3 downto 0);
				load : in STD_LOGIC;
				ack_busy : out STD_LOGIC;
				LCD_data : out STD_LOGIC_VECTOR (3 downto 0);
				LCD_RS : out STD_LOGIC;
				LCD_RW : out STD_LOGIC;
				LCD_E : out STD_LOGIC
			);
end Driver_4Bit;

architecture Behavioral of Driver_4Bit is

	type STATE_TYPE is ( IDLE, SETUP, ACTIVE, HOLD, WAIT_FOR_LOAD_LOW );
	subtype STATE_TIME_TYPE is INTEGER range 0 to CLK_MHZ; -- max state time 1us
	type STATE_TIMINGS_TYPE is array(STATE_TYPE) of STATE_TIME_TYPE;
	constant state_timings : STATE_TIMINGS_TYPE := (
		IDLE => 0,
		SETUP => STATE_TIME_TYPE(CLK_MHZ * 60 / 1000), -- 60ns min time in the SETUP state
		ACTIVE => STATE_TIME_TYPE(CLK_MHZ * 450 / 1000), -- 450ns min time in the ACTIVE state
		HOLD => STATE_TIME_TYPE(CLK_MHZ * 450 / 1000), -- 450ns min time in the HOLD state
		WAIT_FOR_LOAD_LOW => 0);
	signal state_time, state_time_load_value : STATE_TIME_TYPE :=0;
	signal state_time_load : STD_LOGIC := '0';
	signal state_locked : STD_LOGIC;
	signal state : STATE_TYPE := IDLE;
	signal data_reg : STD_LOGIC_VECTOR (3 downto 0) := (others=>'0');
	signal RS_reg : STD_LOGIC := '0';
begin
with state_time select
	state_locked <= state_time_load when 0,'1' when others;
-- Lock state transitions while states are active
TICK_PROC: process (clk) is
begin
		if rising_edge(clk) then
			if state_time_load = '1' then
				state_time <= state_time_load_value;
			elsif state_time > 0 then
				state_time <= state_time - 1;
			end if;
		end if;
end process TICK_PROC;

-- State machine

STATE_PROC: process (clk, state_time, load, state) is variable next_state : STATE_TYPE;
begin
	if rising_edge(clk) then
		state_time_load <= '0';
		if state_locked = '0' then
			next_state := state;
			case state is
				when IDLE =>
					if load = '1' then
						data_reg <= data_in;
						RS_reg <= RS_in;
						next_state := SETUP;
					else
						next_state := IDLE;
					end if;
				when SETUP =>
					next_state := ACTIVE;
				when ACTIVE =>
					next_state := HOLD;
				when HOLD | WAIT_FOR_LOAD_LOW =>
					if load = '1' then
						next_state := WAIT_FOR_LOAD_LOW;
					else
						next_state := IDLE;
					end if;
				end case;
			-- end if;
			state <= next_state;
			state_time_load_value <= state_timings(next_state);
			if state /= next_state then
				state_time_load <= '1';
			end if;
		end if;
	end if;
end process STATE_PROC;

with state select
	LCD_E <= '1' when ACTIVE, '0' when others;

with state select
	ack_busy <= '0' when IDLE, '1' when others;
LCD_RS <= RS_reg;
LCD_RW <= '0';
LCD_data <= data_reg;

end Behavioral;
