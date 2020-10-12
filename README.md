STM32f4_Project with GL-SK board. Buttons, external flash and LCD display as peripheral.

Code lock project. Device manipulates the lock (opens and closes). There is no lock connected, but it is simulated with green and red LEDs on STM32F4.

Compile with make SEMIHOSTING=1 target flash gdb. At the moment printf()-function is used at some places, gdb session required.

# Video with presentation of my project in russian. English version comming https://drive.google.com/file/d/1XRJgsv0b7AchGrI3LCIWJ8YY7n2drJce/view?usp=sharing

Code partly from https://github.com/kpi-keoa/glsk_stm32_toolchain
