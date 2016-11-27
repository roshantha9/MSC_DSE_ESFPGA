----------------------------------------------------------------------------------
-- Company: 	   UoY, Electronics Dept.
-- Engineer: 	   106033467
-- 
-- Create Date:    15:14:57 02/08/2011 
-- Design Name:    debouncer - single unit
-- Module Name:    deb_single - Behavioral 
-- Description:    Debouncer Logic for a single Bouncy Input. uses a counter based
--				   Design.
--
-- Dependencies: 
--
-- Revision: 
-- Revision 0.01 - File Created
-- Additional Comments: 
--
----------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

----------------------------------------------------------------------------------
-- Entity Declaration
----------------------------------------------------------------------------------
entity deb_single is
	-- (33000000/2000)-1 = 16499
	-- clock_freq/
	generic ( SAMPLE_COUNT_MAX 		: natural := 1;
			  PULSE_COUNT_MAX  		: natural := 100000;
			  SAMPLE_GEN_WORD_SIZE 	: natural := 24;
			  DEB_COUNT_WORD_SIZE  	: natural := 8 );
	port(
			CLK 		: in STD_LOGIC;
			RESET 		: in STD_LOGIC;
			D_IN  		: in STD_LOGIC;
			Q_OUT 		: out STD_LOGIC
		 );
end deb_single;

architecture Behavioral of deb_single is

signal sample_now 	: std_logic;
signal sample_count : std_logic_vector(SAMPLE_GEN_WORD_SIZE-1 downto 0);
signal deb_count 	: std_logic_vector(32 downto 0);
signal deb_out 		: STD_LOGIC;

begin

----------------------------------------------------------------------------------
-- deb_single logic - using a counter
----------------------------------------------------------------------------------
	
					 
	deb: process is
	begin
		wait until rising_edge(CLK);		
			if(D_IN='0') then
				deb_count <= (others =>'0');
			else
				if(deb_count = conv_std_logic_vector(600000, 32))then
					deb_out <= D_IN;
				else
					deb_out <= '0';				
				end if;
				deb_count <= deb_count + 1;			
			end if;		
	end process;
	
	Q_OUT <= deb_out;
	
end Behavioral;

