--
-- SPIPassthrough.vhd
--

-- IEEE library for std_logic type
library ieee;
use ieee.std_logic_1164.all;

entity SPIPassthrough is
  port (
    msp_somi                      : out std_logic;
	 msp_simo                      : in std_logic;
	 msp_uclk                      : in std_logic;
	 msp_ste                       : in std_logic;
	 
	 slave_somi                    : in std_logic;
	 slave_simo                    : out std_logic;
	 slave_uclk                    : out std_logic;
	 slave_ste                     : out std_logic);
end SPIPassthrough;

architecture SPIPassthrough of SPIPassthrough is
begin
  msp_somi <= slave_somi;
  slave_simo <= msp_simo;
  slave_uclk <= msp_uclk;
  slave_ste <= msp_ste;
end SPIPassthrough;