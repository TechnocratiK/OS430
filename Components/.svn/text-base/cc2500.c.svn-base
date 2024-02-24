
#include "cc2500.h"
#include "cc2500_registers.h"
#include "common.h"


char packetbuffer[256];
char cc2500_status_byte = 0x00;


/**
 * Initialize the CC2500
 *
 * Initialize UART, then reset and configure all registers
 */
void cc2500_init(void) {
  
  uart_switch(CC2500_UART);
  uart_init();
  
  cc2500_reset();
  timer_wait(10);
  cc2500_read_reg(0x05);
  cc2500_setradioregisters();
}


/**
 * Perform reset on CC2500 to put it in known state for future use
 */
void cc2500_reset(void){
  P5SEL &= 0xF0; //Selecting control of the pins
  P5DIR = 0xB; //1011 out

  P5OUT = 0x09; //clock high and STE high

  P5OUT &=~0x01; // STE low
  timer_wait(10);

  P5OUT |= 0x01; //STE high

  timer_wait(10);
  P5OUT &= ~0x01; // STE low

  while(P5IN & 0x04) continue; //Wait until SOMI goes low

  P5SEL |= 0x0E; //give control to UART for 1110
  uart_spi_byte(0x30,SPI_SINGLE_TRANS); //Send reset strobe

  //while(P5IN & 0x04) continue; //Wait until SOMI goes low
  timer_wait(10);
  
}

/**
 * Set up CC2500 using register configuration values provided by TAs
 */
void cc2500_setradioregisters(void){
  
  cc2500_write_reg(CC2500REG_MCSM0 , 0x18);
  cc2500_write_reg(CC2500REG_MCSM1 , 0x3C);

  cc2500_write_reg(CC2500REG_FSCTRL1 , 0x07);
  cc2500_write_reg(CC2500REG_FSCTRL0 , 0x00);
  cc2500_write_reg(CC2500REG_FREQ2 , 0x5D);
  cc2500_write_reg(CC2500REG_FREQ1 , 0x44);
  cc2500_write_reg(CC2500REG_FREQ0 , 0xEC);
  cc2500_write_reg(CC2500REG_MDMCFG4 , 0x2D);
  cc2500_write_reg(CC2500REG_MDMCFG3 , 0x3B);
  cc2500_write_reg(CC2500REG_MDMCFG2 , 0x73);
  cc2500_write_reg(CC2500REG_MDMCFG1 , 0x23);
  cc2500_write_reg(CC2500REG_MDMCFG0 , 0x3B);
  cc2500_write_reg(CC2500REG_CHANNR , 0x01);
  cc2500_write_reg(CC2500REG_DEVIATN , 0x01);
  cc2500_write_reg(CC2500REG_FREND1 , 0xB6);
  cc2500_write_reg(CC2500REG_FREND0 , 0x10);
  cc2500_write_reg(CC2500REG_MCSM0 , 0x18);
  cc2500_write_reg(CC2500REG_FOCCFG , 0x1D);
  cc2500_write_reg(CC2500REG_BSCFG , 0x1C);
  cc2500_write_reg(CC2500REG_AGCCTRL2 , 0xC7);
  cc2500_write_reg(CC2500REG_AGCCTRL1 , 0x00);
  cc2500_write_reg(CC2500REG_AGCCTRL0 , 0xB0);
  cc2500_write_reg(CC2500REG_FSCAL3 , 0xEA);
  cc2500_write_reg(CC2500REG_FSCAL2 , 0x0A);
  cc2500_write_reg(CC2500REG_FSCAL1 , 0x00);
  cc2500_write_reg(CC2500REG_FSCAL0 , 0x11);
  cc2500_write_reg(CC2500REG_FSTEST , 0x59);
  cc2500_write_reg(CC2500REG_TEST2 , 0x88);
  cc2500_write_reg(CC2500REG_TEST1 , 0x31);
  cc2500_write_reg(CC2500REG_TEST0 , 0x0B);
  cc2500_write_reg(CC2500REG_FIFOTHR , 0x07);
  cc2500_write_reg(CC2500REG_IOCFG2 , 0x29);
  cc2500_write_reg(CC2500REG_IOCFG0 , 0x06);
  cc2500_write_reg(CC2500REG_PKTCTRL1 , 0x04);
  cc2500_write_reg(CC2500REG_PKTCTRL0 , 0x05);
  cc2500_write_reg(CC2500REG_ADDR , 0xFF);
  cc2500_write_reg(CC2500REG_PKTLEN , 0xFF);

//  cc2500_write_reg(CC2500REG_MCSM1 , 0x30);
  
}


/**
 * Retreive status byte returned by last transaction
 */
char cc2500_status(void) {
  return cc2500_status_byte;
}


/**
 * Read a CC2500 register by sending an address with the read bit set, then sending a dummy byte and returning the values
 */
char cc2500_read_reg(char i) {
  uart_switch(CC2500_UART);
  cc2500_status_byte = uart_spi_byte(CC2500_READ | i, SPI_START_TRANS);
  return uart_spi_byte(0x00, SPI_END_TRANS);
}


/**
 * Write to a  CC2500 register byte sending an address with the write bit set and sending a second byte value
 */
void cc2500_write_reg(char i, char val) {
  uart_switch(CC2500_UART);
  cc2500_status_byte = uart_spi_byte(CC2500_WRITE | i, SPI_START_TRANS);
  uart_spi_byte(val, SPI_END_TRANS);
}


/**
 * Scan the register array to retreive the string describing the given address
 */
char* cc2500_reg_get_name(int addr) {
  int reg_size = sizeof(cc2500_reg);
  int i;
  
  for(i = 0; i < sizeof(registers)/reg_size; i++) {
    if(registers[i].address == addr) {
      return registers[i].name;
    }
  }
  return (char*)0x0;
}

/**
 * Scan the register array to retreive the address of the given address by name
 */
int cc2500_reg_get_address(char* name) {
  int reg_size = sizeof(cc2500_reg);
  int i;
  
  for(i = 0; i < sizeof(registers)/reg_size; i++) {
    if(strcmp(registers[i].name, name) == 0) {
      return registers[i].address;
    }
  }
  return 0xFF;
}




/**
 * Incomplete receive function, working code in main.c
 */
char* cc2500_receive_packet(int* len) {
  
  uart_switch(CC2500_UART);
  uart_spi_byte(CC2500REG_SRX, SPI_SINGLE_TRANS); //Put in receive mode
  
  
  
  
}


/**
 * Load TXBUF with data and transmit via Wireless
 */
void cc2500_transmit_packet(char* buffer, int length) {
  int i;
  char dat;
  
  if(length > 64) return;
  
  dat = cc2500_read_reg(CC2500REG_TXBYTES);
  if(dat < 64) {
    
    cc2500_write_reg(CC2500REG_DATABUF, length);
    
    for( i = 0; i < length; i++ ){
      cc2500_write_reg(CC2500REG_DATABUF, buffer[i]);
    }
    
    uart_switch(CC2500_UART);
    uart_spi_byte(CC2500REG_STX, SPI_SINGLE_TRANS); //Put in transmit mode
    
    while(cc2500_read_reg(CC2500REG_MARCSTATE) != 1) {
      
      
    }
    
  }
  else {
    hyperterm_putchar('E');
    hyperterm_putchar('R');
    hyperterm_putchar('R');
  }
}

