--
-- SSDSwitch.vhd
--

-- IEEE library for std_logic type
library ieee;
use ieee.std_logic_1164.all;

entity SSDSwitch is
  port (
    clk                           : in std_logic;
	 reset                         : in std_logic;
	 ssd_select                    : out std_logic);
end SSDSwitch;

architecture SSDSwitch of SSDSwitch is
  signal counter                  : integer range 0 to 40000;
  signal ssd_select_int           : std_logic;
begin
  ssd_select <= ssd_select_int;
  
  process (clk, reset)
  begin
	 if clk = '1' and clk'event then
	   if reset = '1' then
		  ssd_select_int <= '0';
		  counter <= 0;
		elsif counter = 40000 then
		  counter <= 0;
		  ssd_select_int <= not ssd_select_int;
		else
		  counter <= counter + 1;
		end if;
	 end if;
  end process;
end SSDSwitch;