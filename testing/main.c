#include <stdint.h>

/* Helper macro for 32-bit Memory-Mapped I/O */
#define REG32(addr) (*(volatile uint32_t *)(addr))

/* Hardware Register Definitions (exact offsets come from the A33G52x User's
 * Manual) */
#define WDT_BASE 0x40004000U
#define WDT_CON REG32(WDT_BASE + 0x08U) /* Watchdog Control Register */

#define PMU_BASE 0x40000000U
#define PMU_PCER1                                                              \
  REG32(PMU_BASE + 0x28U) /* Peripheral Clock Control Register - svd 758 */

#define PCD_BASE 0x40000230U /* Port D Pin Control Base (PCD) */
#define GPIO_MODER                                                             \
  REG32(PCD_BASE + 0x04U) /* PCD -> CR: Pin Control Register (2 bit per pin)*/

#define GPIOx_BASE 0x40000330U /* Base address of target GPIO-d Ports*/
#define GPIOx_OUTDR                                                            \
  REG32(GPIOx_BASE + 0x00U) /* Output Data Register (1 bit per pin) */

#define LED_PIN 0U /* Pin number on the port (0..15) */

static void delay(volatile uint32_t count) {
  while (count--) {
    __asm__ volatile(
        "nop"); /* Prevents compiler from optimizing away empty loop */
  }
}

int main(void) {
  /* 1. Disable the Watchdog Timer using the datasheet's write-key + disable bit
   */
  WDT_CON &= ~(1U << 5); /* disable watchdog interrupt reset, would happen after
                           some runtime when watchdog reaches 0 */

  /* 2. Enable the peripheral clock for the target GPIO port */
  PMU_PCER1 |= (1U << 8);

  /* 3. Set LED_PIN mode to Push-Pull Output (0b01) */
  GPIO_MODER &= ~(3U << (LED_PIN * 2U)); /* Clear both mode bits first */
  GPIO_MODER |= (1U << (LED_PIN * 2U));  /* Set bit pattern 01 */

  /* 4. Infinite superloop toggling the pin */
  while (1) {
    GPIOx_OUTDR ^= (1U << LED_PIN);
    delay(500000U);
  }

  return 0;
}
