------------------------------------------------------------------------------
-- user_logic.vhd - entity/architecture pair
------------------------------------------------------------------------------
--
-- ***************************************************************************
-- ** Copyright (c) 1995-2008 Xilinx, Inc.  All rights reserved.            **
-- **                                                                       **
-- ** Xilinx, Inc.                                                          **
-- ** XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS"         **
-- ** AS A COURTESY TO YOU, SOLELY FOR USE IN DEVELOPING PROGRAMS AND       **
-- ** SOLUTIONS FOR XILINX DEVICES.  BY PROVIDING THIS DESIGN, CODE,        **
-- ** OR INFORMATION AS ONE POSSIBLE IMPLEMENTATION OF THIS FEATURE,        **
-- ** APPLICATION OR STANDARD, XILINX IS MAKING NO REPRESENTATION           **
-- ** THAT THIS IMPLEMENTATION IS FREE FROM ANY CLAIMS OF INFRINGEMENT,     **
-- ** AND YOU ARE RESPONSIBLE FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE      **
-- ** FOR YOUR IMPLEMENTATION.  XILINX EXPRESSLY DISCLAIMS ANY              **
-- ** WARRANTY WHATSOEVER WITH RESPECT TO THE ADEQUACY OF THE               **
-- ** IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO ANY WARRANTIES OR        **
-- ** REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE FROM CLAIMS OF       **
-- ** INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS       **
-- ** FOR A PARTICULAR PURPOSE.                                             **
-- **                                                                       **
-- ***************************************************************************
--
------------------------------------------------------------------------------
-- Filename:          user_logic.vhd
-- Version:           1.00.a
-- Description:       User logic.
-- Date:              Tue Feb 15 16:48:32 2011 (by Create and Import Peripheral Wizard)
-- VHDL Standard:     VHDL'93
------------------------------------------------------------------------------
-- Naming Conventions:
--   active low signals:                    "*_n"
--   clock signals:                         "clk", "clk_div#", "clk_#x"
--   reset signals:                         "rst", "rst_n"
--   generics:                              "C_*"
--   user defined types:                    "*_TYPE"
--   state machine next state:              "*_ns"
--   state machine current state:           "*_cs"
--   combinatorial signals:                 "*_com"
--   pipelined or register delay signals:   "*_d#"
--   counter signals:                       "*cnt*"
--   clock enable signals:                  "*_ce"
--   internal version of output port:       "*_i"
--   device pins:                           "*_pin"
--   ports:                                 "- Names begin with Uppercase"
--   processes:                             "*_PROCESS"
--   component instantiations:              "<ENTITY_>I_<#|FUNC>"
------------------------------------------------------------------------------

-- DO NOT EDIT BELOW THIS LINE --------------------
library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

library proc_common_v2_00_a;
use proc_common_v2_00_a.proc_common_pkg.all;

-- DO NOT EDIT ABOVE THIS LINE --------------------

--USER libraries added here

------------------------------------------------------------------------------
-- Entity section
------------------------------------------------------------------------------
-- Definition of Generics:
--   C_SLV_DWIDTH                 -- Slave interface data bus width
--   C_NUM_REG                    -- Number of software accessible registers
--   C_RDFIFO_DEPTH               -- Read FIFO depth
--   C_WRFIFO_DEPTH               -- Write FIFO depth
--
-- Definition of Ports:
--   Bus2IP_Clk                   -- Bus to IP clock
--   Bus2IP_Reset                 -- Bus to IP reset
--   Bus2IP_Data                  -- Bus to IP data bus
--   Bus2IP_BE                    -- Bus to IP byte enables
--   Bus2IP_RdCE                  -- Bus to IP read chip enable
--   Bus2IP_WrCE                  -- Bus to IP write chip enable
--   IP2Bus_Data                  -- IP to Bus data bus
--   IP2Bus_RdAck                 -- IP to Bus read transfer acknowledgement
--   IP2Bus_WrAck                 -- IP to Bus write transfer acknowledgement
--   IP2Bus_Error                 -- IP to Bus error response
--   IP2RFIFO_WrReq               -- IP to RFIFO : IP write request
--   IP2RFIFO_Data                -- IP to RFIFO : IP write data bus
--   IP2RFIFO_WrMark              -- IP to RFIFO : mark beginning of packet being written
--   IP2RFIFO_WrRelease           -- IP to RFIFO : return RFIFO to normal FIFO operation
--   IP2RFIFO_WrRestore           -- IP to RFIFO : restore the RFIFO to the last packet mark
--   RFIFO2IP_WrAck               -- RFIFO to IP : RFIFO write acknowledge
--   RFIFO2IP_AlmostFull          -- RFIFO to IP : RFIFO almost full
--   RFIFO2IP_Full                -- RFIFO to IP : RFIFO full
--   RFIFO2IP_Vacancy             -- RFIFO to IP : RFIFO vacancy
--   IP2WFIFO_RdReq               -- IP to WFIFO : IP read request
--   IP2WFIFO_RdMark              -- IP to WFIFO : mark beginning of packet being read
--   IP2WFIFO_RdRelease           -- IP to WFIFO : return WFIFO to normal FIFO operation
--   IP2WFIFO_RdRestore           -- IP to WFIFO : restore the WFIFO to the last packet mark
--   WFIFO2IP_Data                -- WFIFO to IP : WFIFO read data
--   WFIFO2IP_RdAck               -- WFIFO to IP : WFIFO read acknowledge
--   WFIFO2IP_AlmostEmpty         -- WFIFO to IP : WFIFO almost empty
--   WFIFO2IP_Empty               -- WFIFO to IP : WFIFO empty
--   WFIFO2IP_Occupancy           -- WFIFO to IP : WFIFO occupancy
------------------------------------------------------------------------------

entity user_logic is
  generic
  (
    -- ADD USER GENERICS BELOW THIS LINE ---------------
    --USER generics added here
    -- ADD USER GENERICS ABOVE THIS LINE ---------------

    -- DO NOT EDIT BELOW THIS LINE ---------------------
    -- Bus protocol parameters, do not add to or delete
    C_SLV_DWIDTH                   : integer              := 32;
    C_NUM_REG                      : integer              := 1;
    C_RDFIFO_DEPTH                 : integer              := 512;
    C_WRFIFO_DEPTH                 : integer              := 512
    -- DO NOT EDIT ABOVE THIS LINE ---------------------
  );
  port
  (
    -- ADD USER PORTS BELOW THIS LINE ------------------
    --USER ports added here
	LCD_data	: out STD_LOGIC_VECTOR(3 downto 0);
	LCD_RS		: out STD_LOGIC;
	LCD_RW		: out STD_LOGIC;
	LCD_E		: out STD_LOGIC;
	
	
    -- ADD USER PORTS ABOVE THIS LINE ------------------

    -- DO NOT EDIT BELOW THIS LINE ---------------------
    -- Bus protocol ports, do not add to or delete
    Bus2IP_Clk                     : in  std_logic;
    Bus2IP_Reset                   : in  std_logic;
    Bus2IP_Data                    : in  std_logic_vector(0 to C_SLV_DWIDTH-1);
    Bus2IP_BE                      : in  std_logic_vector(0 to C_SLV_DWIDTH/8-1);
    Bus2IP_RdCE                    : in  std_logic_vector(0 to C_NUM_REG-1);
    Bus2IP_WrCE                    : in  std_logic_vector(0 to C_NUM_REG-1);
    IP2Bus_Data                    : out std_logic_vector(0 to C_SLV_DWIDTH-1);
    IP2Bus_RdAck                   : out std_logic;
    IP2Bus_WrAck                   : out std_logic;
    IP2Bus_Error                   : out std_logic;
    IP2RFIFO_WrReq                 : out std_logic;
    IP2RFIFO_Data                  : out std_logic_vector(0 to C_SLV_DWIDTH-1);
    IP2RFIFO_WrMark                : out std_logic;
    IP2RFIFO_WrRelease             : out std_logic;
    IP2RFIFO_WrRestore             : out std_logic;
    RFIFO2IP_WrAck                 : in  std_logic;
    RFIFO2IP_AlmostFull            : in  std_logic;
    RFIFO2IP_Full                  : in  std_logic;
    RFIFO2IP_Vacancy               : in  std_logic_vector(0 to log2(C_RDFIFO_DEPTH));
    IP2WFIFO_RdReq                 : out std_logic;
    IP2WFIFO_RdMark                : out std_logic;
    IP2WFIFO_RdRelease             : out std_logic;
    IP2WFIFO_RdRestore             : out std_logic;
    WFIFO2IP_Data                  : in  std_logic_vector(0 to C_SLV_DWIDTH-1);
    WFIFO2IP_RdAck                 : in  std_logic;
    WFIFO2IP_AlmostEmpty           : in  std_logic;
    WFIFO2IP_Empty                 : in  std_logic;
    WFIFO2IP_Occupancy             : in  std_logic_vector(0 to log2(C_WRFIFO_DEPTH))
    -- DO NOT EDIT ABOVE THIS LINE ---------------------
  );

  attribute SIGIS : string;
  attribute SIGIS of Bus2IP_Clk    : signal is "CLK";
  attribute SIGIS of Bus2IP_Reset  : signal is "RST";

end entity user_logic;

------------------------------------------------------------------------------
-- Architecture section
------------------------------------------------------------------------------


------------------------------------------------------------------------------
-- Architecture section
------------------------------------------------------------------------------

architecture IMP of user_logic is
-- Signals and shared variables
	type STATE_TYPE is ( READY, LOADING, WAITING );
	signal state : STATE_TYPE := WAITING;
	signal data_store : STD_LOGIC_VECTOR(7 downto 0) := (others=>'0');
	signal RS_store, load : STD_LOGIC := '0';
	signal ack_busy : STD_LOGIC;
	
-- External components
	component Driver_8Bit
		generic (
				CLK_MHZ : NATURAL
				);
		port (
				clk : in STD_LOGIC;
				data_in : in STD_LOGIC_VECTOR (7 downto 0);
				RS_in : in STD_LOGIC;
				load : in STD_LOGIC;
				ack_busy : out STD_LOGIC;
				LCD_data : out STD_LOGIC_VECTOR (3 downto 0);
				LCD_RS : out STD_LOGIC;
				LCD_RW : out STD_LOGIC;
				LCD_E : out STD_LOGIC
			);
	end component;
	
begin
	
-- Driver module instantiation
	driver : Driver_8Bit
		generic map (
			CLK_MHZ => 125
		)
		port map (
			clk => Bus2IP_Clk,
			data_in => data_store,
			RS_in => RS_store,
			load => load,
			ack_busy => ack_busy,
			LCD_data => LCD_data,
			LCD_RS => LCD_RS,
			LCD_RW => LCD_RW,
			LCD_E => LCD_E
		);

STATE_PROC: process (Bus2IP_Clk) is
begin
	if rising_edge(Bus2IP_Clk) then
		IP2WFIFO_RdReq <= '0';
		
		case state is
			
			when READY =>
				if WFIFO2IP_RdAck = '1' then
				-- Valid data to read from fifo
					data_store <= WFIFO2IP_Data(24 to 31);
					RS_store <= WFIFO2IP_Data(23);
					state <= LOADING;
				else
				-- No valid data present yet
					IP2WFIFO_RdReq <= '1';
				end if;
			
			when LOADING =>
				if ack_busy = '1' then
				-- Load request acknowledged
					state <= WAITING;
				end if;
			
			when WAITING =>
				if ack_busy = '0' then
				-- Driver free
				state <= READY;
				end if;
			end case;
		end if;
end process;

with state select 
	load <= '1' when LOADING,
	'0' when others;
	
------------------------------------------
-- Not using direct transfer of data
------------------------------------------
	IP2Bus_Data <= (others => '0');
	IP2Bus_WrAck <= '0';
	IP2Bus_RdAck <= '0';
	IP2Bus_Error <= '0';
	
end IMP;

