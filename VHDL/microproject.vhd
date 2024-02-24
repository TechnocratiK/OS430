library IEEE;
use IEEE.std_logic_1164.all;
Use Ieee.std_logic_unsigned.all;

entity microproject is 
port( userVal: in std_logic_vector(7 downto 0);
		reset: in std_logic;
		clock: in std_logic;
		--clock2: in std_logic;
		LED: out std_logic_vector(6 downto 0);
		clock_en : out std_logic;
		dec_pt : out std_logic;
		--BCDintegerout: out std_logic_vector(7 downto 0);
		--userValLSDout, userValMSDout: out std_logic_vector(3 downto 0);
		--countBCDout: out integer range 0 to 100;
		MOSI:     in std_logic;
		SOMI:     in std_logic;
		STE:      in std_logic;
		CLK:      in std_logic;
		MOSI_out: out std_logic;
		SOMI_out: out std_logic;
		STE_out:  out std_logic;
		CLK_out:  out std_logic;
	   chooseLED, chooseLED2: out std_logic);
end microproject; 

architecture Timing of microproject is 

type state_type is(start, one, two, three);
signal state: state_type;
signal BCDinteger: std_logic_vector(7 downto 0); 
signal userValLSD, userValMSD: std_logic_vector(3 downto 0);
signal LEDswitch: std_logic := '0';
signal oneSec: integer range 0 to 3 := 0;
signal LEDout: std_logic_vector(3 downto 0);
signal countBCD: integer range 0 to 10 := 0;
signal count: integer range 0 to 4001 := 0;
-- use userValMSD and userValLSD to drive LCD display. 

begin

--userValLSDout <= userValLSD;
--userValMSDout <= userValMSD;
clock_en <= '1';
MOSI_out <= MOSI;
SOMI_out <= SOMI;
CLK_out <= CLK;
STE_out <= STE;
userValMSD <= BCDinteger(3) & BCDinteger(2) & BCDinteger(1) & BCDinteger(0);
userValLSD <= BCDinteger(7) & BCDinteger(6) & BCDinteger(5) & BCDinteger(4);
dec_pt <= '1';

chooseLED <= LEDswitch;
--countBCDout <= countBCD;
--BCDintegerout <= BCDinteger;

chooseLED2 <= not LEDswitch;

with LEDswitch select LEDout <=
	userValLSD when '1',
	userValMSD when others;

with LEDout select LED <=
        "1000000" when "0000", --0
        "1111001" when "0001", --1
        "0100100" when "0010", --2
        "0110000" when "0011", --3
        "0011001" when "0100", --4
        "0010010" when "0101", --5
        "0000010" when "0110", --6
        "1111000" when "0111", --7
        "0000000" when "1000", --8
        "0011000" when "1001", --9
		  "1111111" when others;

--process(clock2, reset) 
--	--variable countBCD: integer := 0;
--	begin
--		if clock2'event and clock2 = '1' then
--			if reset = '1' then
--			  countBCD <= 0;
--			  BCDinteger(7 downto 0) <= "00000000";
--			else 
--			--oneSec <= 0;
--				if countBCD < 8 then
--					BCDinteger(countBCD) <= userVal; 
--					countBCD <= countBCD + 1; --will count bcd be incremented before or after storing userVal?				  
--				end if;
--			end if;
--		end if;
--end process;

process(clock)
  begin
		if clock 'event and clock = '1' then
		  if reset = '1' then
		    count <= 0;
		  elsif count < 4000 then
          count <= count + 1;
        else
			 BCDinteger(7 downto 0) <= userVal(7 downto 0); 
		    count <= 0;
		    LEDswitch <= not LEDswitch;
		  end if;
		end if; 
end process; 

--process(clock) 
--	begin 
--		if clock 'event and clock = '1' then
--			if reset = '0' then 
--				case state is 
--				
--					when start => 
--						oneSec <= 0;
--						LEDswitch <= LEDswitch xor '1';
--						state <= one; 
--				
--					when one => 
--						userValLSD <= BCDinteger(3 downto 0);
--						userValMSD <= BCDinteger(7 downto 4);
--						LEDswitch <= LEDswitch xor '1';
--						state <= two;
--						
--					when two => 
--						oneSec <= oneSec + 1;
--						if oneSec = 2 then -- figure out the value of oneSec that corresponds to one second having elapsed. 
--							if userValLSD = "1001" then
--								userValLSD <= "0000";
--								if userValMSD = "0101" then
--									userValMSD <= "0000";
--									LEDswitch <= LEDswitch xor '1';
--									state <= three;
--								else 
--									userValMSD <= userValMSD + "0001"; -- find out if this works
--									LEDswitch <= LEDswitch xor '1';
--									state <= three;
--								end if;
--							else
--								userValLSD <= userValLSD + "0001";
--								LEDswitch <= LEDswitch xor '1';
--								state <= three;
--							end if;
--						end if;
--						
--					when three => 
--						oneSec <= 0;
--						state <= two;
--						
--				end case;
--			end if;
--		end if;
--end process;
end Timing;