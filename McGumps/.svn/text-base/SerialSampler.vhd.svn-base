--
-- SerialSampler.vhd
--

-- IEEE library for std_logic type
library ieee;
use ieee.std_logic_1164.all;

entity SerialSampler is
  port (
    clk                           : in std_logic;
	 reset                         : in std_logic;
	 serial_in                     : in std_logic;
	 serial_clk                    : in std_logic;
	 bit_out                       : out std_logic;
	 bit_good                      : out std_logic;
end SerialSampler;

architecture SerialSampler of SerialSampler is
  signal samples                  : integer range 0 to 100000;
  signal sampled                  : integer range 0 to 100000;
  signal serial_clk_old           : std_logic;
  signal bits                     : integer range 0 to 7;
  signal data                     : std_logic_vector(7 downto 0);
  
begin
  process (clk, reset)
  begin
	 if clk = '1' and clk'event then
	   serial_clk_old <= serial_clk;
		
	   if reset = '1' then
		  samples <= 0;
		  sampled <= 0;
		end if;
		
	   if serial_clk = '1' then
		  if samples < 100000 then
		  else
		    if samples / 2 < sampled then
			 
      else if serial_clk_old = '1' then
		  samples <= 0;
		  sampled <= 0;
	   end if;
	 end if;
  end process;
end SerialShiftRegister;