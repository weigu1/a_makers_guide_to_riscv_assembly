### 00_template.S
### weigu.lu

#define UART0_BAUDRATE 115200
#define UART1_BAUDRATE 1000000

#include "../lib/rp2350_riscv_image_def_block.S"
#include "../lib/rp2350_definitions.S"
#include "../lib/srs_delays.S"
#include "../lib/srs_gpios.S"
#include "../lib/srs_resets.S"
#include "../lib/srs_clocks.S"
//#include "../lib/srs_uarts.S"

        ### Global constants
        .equ PIN_LED,  25           # Onboard LED
        .equ DELAY_MS, 100          # 100 = 5 Hz blink

        .section .text              # Marks the start of the code section in memory
        .global _start              # Makes the symbol '_start' visible to the linker
        .balign 4
### Entry Point: _start
_start:         la sp, __stack_top  # Load the stack pointer with the top of the stack
                call main           # Call the main function
                wfi                 # Wait for interrupt (save power if CPU idle)
                j _start            # Safeguard if main returns (should never happen)

### Main Function
        .section .text              # Code section
        .global main                # Make 'main' visible to the linker
        .balign 4
main:
                jal init_clocks     # Initialize 5 clocks
                #jal init_uart1
                li a0, PIN_LED      # Setup PIN_LED as output
                jal init_gpio_out
        .balign 4
mainloop:       li a0, PIN_LED      # Toggle LED (atomic XOR)
                jal gpio_toggle
                li a0, DELAY_MS     # Kill some time
                jal delay_ms
                j mainloop          # Jump mainloop forever
