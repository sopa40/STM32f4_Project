# STM32f4_Project with GL-SK board. Buttons, external flash and LCD display as peripheral.

# Code lock project. Device manipulates the lock (opens and closes). There is no lock connected, but it is simulated with green and red LEDs on STM32F4.

# Compile with make SEMIHOSTING=1 target flash gdb. At the moment some printf()-functions are used, gdb session required 

# Code partly from https://github.com/kpi-keoa/glsk_stm32_toolchain
