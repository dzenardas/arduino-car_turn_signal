# Car Turn Signal
This project simulates a car turn signal using Arduino Uno R3 and other components in Tinkercad (see documentation.pdf for the brief note). Two buttons turn the left or right signal on and off. The LED blinks and the buzzer beeps at the same time. The third button changes the blinking speed and the buzzer tone, and this setting is saved in EEPROM, so it is still there after power off.

# Features
- Left and right button turn the signal on/off (LED + buzzer blink together)
- Pressing the other button changes the side
- Set button changes speed (300 ms / 600 ms) and tone (2000 Hz / 1000 Hz)
- Speed and tone are saved in EEPROM and loaded when Arduino starts
- Serial monitor shows events (buttons, saved settings)
- No delay() in the code, only interrupts, timer flags and state machine

# How to use
1. Open this project in Tinkercad (or upload the code to Arduino Uno);
2. Start the simulation and open the Serial Monitor;
3. Press the left or right button to start the signal, press it again to stop;
4. Press the set button to change the speed and the tone;
5. Press restart button built-in Arduino Uno R3 board - the last setting is still the same.

# Files
- car_turn_signal.ino - full project (Arduino) code
- documentation.pdf - brief note (design, timer config, ISR roles, EEPROM layout, wiring/schematic, timing budget, test results, known issues)
