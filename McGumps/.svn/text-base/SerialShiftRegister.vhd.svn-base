--
-- SerialShiftRegister.vhd
--

-- IEEE library for std_logic type
library ieee;
use ieee.std_logic_1164.all;

entity SerialShiftRegister is
  port (
    clk                           : in std_logic;
	 reset                         : in std_logic;
	 serial_in                     : in std_logic;
	 serial_clk                    : in std_logic;
	 data_out                      : out std_logic_vector(7 downto 0));
end SerialShiftRegister;

architecture SerialShiftRegister of SerialShiftRegister is
  signal state                    : integer range 0 to 5;
  signal samples                  : std_logic_vector(1 downto 0);
  signal bits                     : integer range 0 to 7;
  signal data                     : std_logic_vector(7 downto 0);
  
begin
  process (clk)
  begin
	 if clk = '1' and clk'event then
	   if reset = '1' then
		  state <= 0;
		  bits <= 0;
		  samples <= "00";
		  data <= "00000000";
		end if;
		
      case state is
	     when 0 => -- clock was low
	       samples <= "00";
		    if serial_clk = '1' then
	         state <= 1;
			 else
			   state <= 0;
			 end if;
				
	     when 1 => -- clock is high; sample
		    samples <= serial_in & samples(1 downto 1);
			 --state <= 2;
			 if serial_in = '1' then
			   state <= 3;
			 elsif serial_in = '0' then
			   state <= 4;
			 end if;
		
		  when 2 => -- analyze the sample
			 if samples = "11" then
			   state <= 3;
			 elsif samples = "00" then
			   state <= 4;
			 else
			   state <= 1;
			 end if;
				
		  when 3 => -- a '1' was detected
		    case bits is
			   when 0 =>
  			     data <= "10000000";
				when 7 =>
			     data_out <= '1' & data(7 downto 1);
			     bits <= 0;
			   when others =>
			     data <= '1' & data(7 downto 1);
				  bits <= bits + 1;
			 end case;
			 state <= 5;
				
		  when 4 => -- a '0' was detected
		    case bits is
			   when 0 =>
  			     data <= "10000000";
				when 7 =>
			     data_out <= '0' & data(7 downto 1);
			     bits <= 0;
			   when others =>
			     data <= '0' & data(7 downto 1);
				  bits <= bits + 1;
			 end case;
			 state <= 5;
			
		  when 5 => -- waiting for the clock to go low
		    if serial_clk = '0' then
			   state <= 0;
			 else
			   state <= 5;
			 end if;
		end case;
	 end if;
	 
  end process;
end SerialShiftRegister;