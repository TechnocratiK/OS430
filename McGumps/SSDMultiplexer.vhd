--
-- SSDMultiplexer.vhd
--

-- IEEE library for std_logic type
library ieee;
use ieee.std_logic_1164.all;

entity SSDMultiplexer is
  port (
    ssd_select                    : in std_logic;
    ssd_values                    : in std_logic_vector(7 downto 0);
    ssd_nibble                    : out std_logic_vector(3 downto 0);
	 ssd_left_enable               : out std_logic;
	 ssd_right_enable              : out std_logic);
end SSDMultiplexer;

architecture SSDMultiplexer of SSDMultiplexer is
begin
  ssd_left_enable <= ssd_select;
  ssd_right_enable <= not ssd_select;
  
  with ssd_select select
    ssd_nibble <=
	   ssd_values(3 downto 0) when '0',
		ssd_values(7 downto 4) when '1';
end SSDMultiplexer;