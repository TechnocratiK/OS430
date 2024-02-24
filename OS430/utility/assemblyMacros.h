#ifndef _UTILITY_ASSEMBLY_MACROS_H
#define _UTILITY_ASSEMBLY_MACROS_H

ROTATE_BYTE_LEFT MACRO v, n
x SET 0
  LOOP x < n
    rla.b   v
    adc.b   v
x SET (x + 1)
  ENDLOOP
ENDMACRO

ROTATE_WORD_LEFT MACRO v, n
x SET 0
  LOOP x < n
    rla.w   v
    adc.w   v
x SET (x + 1)
  ENDLOOP
ENDMACRO

ROTATE_BYTE_RIGHT MACRO v, n
x SET 0
  LOOP x < n
    bit.b   #0x01, v
    rrc.b   v
x SET (x + 1)
  ENDLOOP
ENDMACRO

ROTATE_WORD_RIGHT MACRO v, n
x SET 0
  LOOP x < n
    bit.w   #0x0001, v
    rrc.w   v
x SET (x + 1)
  ENDLOOP
ENDMACRO

ARITHMETIC_SHIFT_BYTE_LEFT MACRO v, n
x SET 0
  LOOP x < n
    rla.b   v
x SET (x + 1)  
  ENDLOOP
ENDMACRO

ARITHMETIC_SHIFT_WORD_LEFT MACRO v, n
x SET 0
  LOOP x < n
    rla.w   v
x SET (x + 1)  
  ENDLOOP
ENDMACRO

ARITHMETIC_SHIFT_BYTE_RIGHT MACRO v, n
x SET 0
  LOOP x < n
    rra.b   v
x SET (x + 1)
  ENDLOOP
ENDMACRO

ARITHMETIC_SHIFT_WORD_RIGHT MACRO v, n
x SET 0
  LOOP x < n
    rra.w   v
x SET (x + 1)
  ENDLOOP
ENDMACRO

LOGICAL_SHIFT_BYTE_LEFT MACRO v, n
  ARITHMETIC_SHIFT_BYTE_LEFT v, n
ENDMACRO

LOGICAL_SHIFT_WORD_LEFT MACRO v, n
  ARITHMETIC_SHIFT_WORD_LEFT v, n
ENDMACRO

LOGICAL_SHIFT_BYTE_RIGHT MACRO v, n
  ARITHMETIC_SHIFT_BYTE_RIGHT v, n
  bic.b   #((-1) << (8 - n)), v
ENDMACRO

LOGICAL_SHIFT_WORD_RIGHT MACRO v, n
  ARITHMETIC_SHIFT_WORD_RIGHT v, n
  bic.w   #((-1) << (16 - n)), v
ENDMACRO

#endif 
