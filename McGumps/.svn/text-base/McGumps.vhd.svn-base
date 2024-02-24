--
-- McGumps.vhd
--

-- IEEE library for std_logic type
library ieee;
use ieee.std_logic_1164.all;

entity McGumps is
  port (
    -- MSP430 ports
    msp430Port1Pin7               : in std_logic;
    msp430Port1Pin6               : in std_logic;
    msp430Port1Pin5               : in std_logic;
    msp430Port1Pin4               : in std_logic;
    msp430Port1Pin3               : in std_logic;
    msp430Port1Pin2               : in std_logic;
    msp430Port1Pin1               : in std_logic;
    msp430Port1Pin0               : in std_logic;

	 msp430Port2Pin7               : in std_logic;
	 msp430Port2Pin6               : in std_logic;
	 msp430Port2Pin5               : in std_logic;
	 msp430Port2Pin4               : in std_logic;
	 msp430Port2Pin3               : in std_logic;
	 msp430Port2Pin2               : in std_logic;
	 msp430Port2Pin1               : in std_logic;
	 msp430Port2Pin0               : in std_logic;

	 msp430Port4Pin7               : in std_logic;
	 msp430Port4Pin6               : in std_logic;
	 msp430Port4Pin5               : in std_logic;
	 msp430Port4Pin4               : in std_logic;
	 msp430Port4Pin3               : in std_logic;
	 msp430Port4Pin2               : in std_logic;
	 msp430Port4Pin1               : in std_logic;
	 msp430Port4Pin0               : in std_logic;
	 
	 msp430Port5Pin7               : in std_logic;
	 msp430Port5Pin6               : in std_logic;
	 msp430Port5Pin5               : in std_logic;
	 msp430Port5Pin4               : in std_logic;
	 msp430Port5Pin3               : in std_logic;
	 msp430Port5Pin2               : out std_logic;
	 msp430Port5Pin1               : in std_logic;
	 msp430Port5Pin0               : in std_logic;
	 
	 -- Header pins
	 --h1Pin3                        : out std_logic;
	 h1Pin4                        : out std_logic;
	 h1Pin5                        : out std_logic;
	 h1Pin6                        : out std_logic;
	 h1Pin7                        : out std_logic;
	 h1Pin8                        : out std_logic;
	 h1Pin9                        : out std_logic;
	 h1Pin10                       : out std_logic;
	 h1Pin12                       : out std_logic;
	 h1Pin14                       : out std_logic;
	 h1Pin16                       : out std_logic;
	 h1Pin18                       : out std_logic;
	 h1Pin24                       : out std_logic;
	 h1Pin26                       : out std_logic;
	 h1Pin28                       : out std_logic;
	 h1Pin30                       : in std_logic;
	 h1Pin32                       : out std_logic;
	 h1Pin34                       : out std_logic;
	 h1Pin36                       : out std_logic;
	 
	 -- Seven-Segment Display pins
	 ssdSegments                   : out std_logic_vector(7 downto 0);
	 ssdLeftEnable                 : out std_logic;
	 ssdRightEnable                : out std_logic;
	 
	 -- Clock
	 clockEnable                   : out std_logic;
	 clk                           : in std_logic);
end McGumps;

architecture McGumps of McGumps is
  signal reset                    : std_logic;
  signal serial_byte              : std_logic_vector(7 downto 0);
  signal ssd_nibble               : std_logic_vector(3 downto 0);
  signal ssd_select               : std_logic;

--component SerialShiftRegister
--  port (
--    clk                           : in std_logic;
--	 reset                         : in std_logic;
--	 serial_in                     : in std_logic;
--	 serial_clk                    : in std_logic;
--	 data_out                      : out std_logic_vector(7 downto 0));
--end component;
--
--component SSDSwitch
--  port (
--    clk                           : in std_logic;
--	 reset                         : in std_logic;
--	 ssd_select                    : out std_logic);
--end component;
--
--component SSDMultiplexer
--  port (
--    ssd_select                    : in std_logic;
--    ssd_values                    : in std_logic_vector(7 downto 0);
--    ssd_nibble                    : out std_logic_vector(3 downto 0);
--	 ssd_left_enable               : out std_logic;
--	 ssd_right_enable              : out std_logic);
--end component;
--
--component SSDDecoder
--  port (
--    ssd_value                     : in std_logic_vector(3 downto 0);
--    ssd_segments                  : out std_logic_vector(6 downto 0)); 
--end component;

component SPIPassthrough
  port (
    msp_somi                      : out std_logic;
	 msp_simo                      : in std_logic;
	 msp_uclk                      : in std_logic;
	 msp_ste                       : in std_logic;
	 
	 slave_somi                    : in std_logic;
	 slave_simo                    : out std_logic;
	 slave_uclk                    : out std_logic;
	 slave_ste                     : out std_logic);
end component;


begin
  clockEnable <= '1';
  reset <= msp430Port1Pin2;
  ssdSegments(7) <= '1';

--  shiftReg : SerialShiftRegister
--    port map (
--	   clk => clk,
--		reset => reset,
--		serial_in => msp430Port1Pin1,
--		serial_clk => msp430Port1Pin0,
--		data_out => serial_byte);
--
--  switch : SSDSwitch
--    port map (
--	   clk => clk,
--		reset => reset,
--		ssd_select => ssd_select);
--		
--  mux : SSDMultiplexer
--    port map (
--	   ssd_select => ssd_select,
--		ssd_values => serial_byte,
--		ssd_nibble => ssd_nibble,
--		ssd_left_enable => ssdLeftEnable,
--		ssd_right_enable => ssdRightEnable);
--	
--  decoder : SSDDecoder
--    port map (
--	   ssd_value => ssd_nibble,
--		ssd_segments => ssdSegments(6 downto 0));
		
  spiPT : SPIPassthrough
    port map (
	   msp_somi => msp430Port5Pin2,
	   msp_simo => msp430Port5Pin1,
	   msp_uclk => msp430Port5Pin3,
	   msp_ste => msp430Port5Pin0,
	 
	   slave_somi => h1Pin30,
	   slave_simo => h1Pin34,
	   slave_uclk => h1Pin24,
	   slave_ste => h1Pin26);
end McGumps;