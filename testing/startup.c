#include <stdint.h>

/* 1. Import the memory boundary symbols defined in your .ld file */
extern uint32_t __StackTop;
extern uint32_t __etext;
extern uint32_t __data_start__;
extern uint32_t __data_end__;
extern uint32_t __bss_start__;
extern uint32_t __bss_end__;

/* Forward declaration of your application entry point */
int main(void);

/* Default handler for unexpected hardware faults */
void Default_Handler(void) {
  while (1)
    ;
}

/* 2. The very first code executed by the CPU after reset */
void Reset_Handler(void) {
  /* Copy initialized global/static variables from Flash to SRAM */
  uint32_t *src = &__etext;
  uint32_t *dst = &__data_start__;
  while (dst < &__data_end__) {
    *dst++ = *src++;
  }

  /* Zero-out uninitialized global/static variables in SRAM */
  dst = &__bss_start__;
  while (dst < &__bss_end__) {
    *dst++ = 0;
  }

  /* Enter the C application */
  main();

  /* Trap execution if main() ever returns */
  while (1)
    ;
}

/* 3. The Hardware Vector Table placed at 0x00000000 by your .ld script */
__attribute__((section(".vectors"), used)) const uint32_t vector_table[16] = {
    (uint32_t)&__StackTop,     /* 0x00: Initial Stack Pointer */
    (uint32_t)Reset_Handler,   /* 0x04: Reset Handler */
    (uint32_t)Default_Handler, /* 0x08: NMI Handler */
    (uint32_t)Default_Handler, /* 0x0C: Hard Fault Handler */
    (uint32_t)Default_Handler, /* 0x10: MPU Fault Handler */
    (uint32_t)Default_Handler, /* 0x14: Bus Fault Handler */
    (uint32_t)Default_Handler, /* 0x18: Usage Fault Handler */
    0,
    0,
    0,
    0,                         /* 0x1C - 0x28: Reserved by Arm */
    (uint32_t)Default_Handler, /* 0x2C: SVCall Handler */
    (uint32_t)Default_Handler, /* 0x30: Debug Monitor Handler */
    0,                         /* 0x34: Reserved by Arm */
    (uint32_t)Default_Handler, /* 0x38: PendSV Handler */
    (uint32_t)Default_Handler  /* 0x3C: SysTick Handler */
};
