//Board = Arduino Nano w/ ATmega328
#define ARDUINO 103
#define __AVR_ATmega328P__
#define F_CPU 16000000L
#define __AVR__
#define __cplusplus
#define __attribute__(x)
#define __inline__
#define __asm__(x)
#define __extension__
#define __ATTR_PURE__
#define __ATTR_CONST__
#define __inline__
#define __asm__ 
#define __volatile__
#define __builtin_va_list
#define __builtin_va_start
#define __builtin_va_end
#define __DOXYGEN__
#define prog_void
#define PGM_VOID_P int
#define NOINLINE __attribute__((noinline))

typedef unsigned char byte;
extern "C" void __cxa_pure_virtual() {}

//already defined in arduno.h
void read_code();
void write_code();
void green();
void red();
void orange();
void orange_blink();
void off();
void red_blink();
void lights_on();
void lights_off();
void beep1();
void beep2();
void beep3();
void beep4();
void beep5();
void reset_ecode();
boolean recovery_code_ecode_compare();
void ecode_code_copy();
boolean code_ecode_compare();
boolean ecode_ecode1_compare();
void servos_open();
void servos_close();
void print_mode();
//already defined in arduno.h

#include "C:\Users\Alexander Hofmann\Documents\Arduino\arduino-1.3\hardware\arduino\variants\eightanaloginputs\pins_arduino.h" 
#include "C:\Users\Alexander Hofmann\Documents\Arduino\arduino-1.3\hardware\arduino\cores\arduino\arduino.h"
#include "C:\Users\Alexander Hofmann\Documents\Arduino\myprojects\tresor\tresor\tresor.ino"
