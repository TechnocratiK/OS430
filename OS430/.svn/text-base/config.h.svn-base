#ifndef _CONFIG_H
#define _CONFIG_H

/******************************************************************************
 * cc2500
 *****************************************************************************/
#define CC2500_PACKET_SIZE 64
#define CC2500_DESTINATION_ADDRESS 0xff
#define CC2500_RECEIVE_TIMEOUT 80
#define CC2500_CHANNEL 0x08

/******************************************************************************
 * Temperature
 *****************************************************************************/
#define TEMPERATURE_ZERO_OFFSET 2871
#define TEMPERATURE_SLOPE_NUMERATOR 17
#define TEMPERATURE_SLOPE_DENOMINATOR 44
#define TEMPERATURE_DISPLAY_TIME 5000

/******************************************************************************
 * Terminal
 *****************************************************************************/
#define TERMINAL_BAUD_RATE 19200

/******************************************************************************
 * Scheduler
 *****************************************************************************/
#define SCHEDULER_INIT_STACK_SIZE 0x80
#define SCHEDULER_QUANTUM_TIME 1000
#define SCHEDULER_MAXIMUM_PRIORITY 2
#define SCHEDULER_MINIMUM_PRIORITY 4
#define SCHEDULER_QUEUE_SIZE 0x20

/******************************************************************************
 * Memory
 *****************************************************************************/
#define MEMORY_TOTAL_SIZE 0x0400
#define MEMORY_GRANULARITY 1

#define MEMORY_HEAP_START (_memorySpace)
#define MEMORY_HEAP_SIZE (((MEMORY_TOTAL_SIZE) / ((8 << MEMORY_GRANULARITY) + 1)) * (8 << MEMORY_GRANULARITY))
#define MEMORY_HEAP_END (MEMORY_HEAP_START + MEMORY_HEAP_SIZE)

#define MEMORY_MAP_START (((MEMORY_HEAP_END + 1) / 2) * 2)
#define MEMORY_MAP_SIZE ((MEMORY_TOTAL_SIZE) / ((8 << MEMORY_GRANULARITY) + 1))
#define MEMORY_MAP_END ((((MEMORY_MAP_START + MEMORY_MAP_SIZE) + 1) / 2) * 2)

/******************************************************************************
  *Flash Memory
******************************************************************************/
#define MESSAGE_MAX_AGE 4
#define FLASH_BLOCK_SIZE 50
#define FLASH_BASE_ADDRESS 0x1000

#endif                                  // _CONFIG_H
