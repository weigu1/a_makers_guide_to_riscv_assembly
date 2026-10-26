### 00_template_2.S
### weigu.lu

#define UART0_BAUDRATE 115200
#define UART1_BAUDRATE 1000000

#include "../lib/rp2350_riscv_image_def_block.S"
#include "../lib/rp2350_definitions.S"
#include "../lib/srs_delays.S"
#include "../lib/srs_gpios.S"
#include "../lib/srs_resets.S"
#include "../lib/srs_clocks.S"
#include "../lib/srs_timers_pwm.S"
//#include "../lib/srs_uarts.S"

        ### Global constants
        .equ PIN_LED,  25           # Onboard LED
        .equ DELAY_MS, 250          # 100 = 5 Hz blink

        .section .text              # Marks the start of the code section in memory
        .global _start              # Makes the symbol '_start' visible to the linker
        .balign 4
### Entry Point: _start
_start:         la sp, __stack_top  # Load the stack pointer with the top of the stack
                # Copy .data from Flash to RAM
                la a0, __data_load_addr # Source: .data in Flash (LMA)
                la a1, __data_start # Destination: .data in RAM (VMA)
                la a2, __data_end   # End of .data in RAM
_copy_data_loop: beq a1, a2, _copy_data_done # If dest == end, we're done
                lw a3, 0(a0)        # Load word from Flash
                sw a3, 0(a1)        # Store word to RAM
                addi a0, a0, 4      # Increment source address pointer (word)
                addi a1, a1, 4      # Increment destination address pointer (word)
                j _copy_data_loop   # Stay in loop
                # Zero-initialize .bss
_copy_data_done: la a0, __bss_start # Start of .bss
                la a1, __bss_end    # End of .bss
_zero_bss_loop: beq a0, a1, _zero_bss_done # If start == end, we're done
                sw zero, 0(a0)      # Store zero to RAM
                addi a0, a0, 4      # Increment address pointer
                j _zero_bss_loop    # Stay in loop
_zero_bss_done: call main          # Call the main function
                wfi                 # Wait for interrupt (save power if CPU idle)
                j _start            # safeguard if main returns (should never happen)

### Main Function
        .section .text              # Code section
        .global main                # Make 'main' visible to the linker
        .balign 4
main:
                jal init_clocks     # Initialize 5 clocks
                jal init_timer0     # Init TIMER0 1us
                #jal init_uart1
                li a0, PIN_LED      # Setup PIN_LED as output
                jal init_gpio_out
        .balign 4
mainloop:       li a0, PIN_LED      # Toggle LED (atomic XOR)
                jal gpio_toggle
                li a0, DELAY_MS     # Kill some time
                jal delay_ms_t0
                j mainloop          # Jump mainloop forever
