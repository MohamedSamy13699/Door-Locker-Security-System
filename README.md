# Door-Locker-Security-System

To design and implement a Door Locker Security System based on your requirements, we'll outline a brief plan to tackle each component of your system using two ATmega32 Microcontrollers. Here’s a condensed guide to each requirement and how to approach it:

System Overview

Microcontrollers: Two ATmega32 MCUs at 8MHz. One serves as the Human Machine Interface (HMI_ECU) connected to a 2x16 LCD and a 4x4 keypad for user interaction. The other acts as the Control Unit (CONTROL_ECU) interfaced with EEPROM for data storage, a buzzer for alerts, and a DC motor for door control.

Layered Architecture: The project is divided into distinct layers with HMI_ECU handling user inputs/outputs and CONTROL_ECU managing data processing and decision-making.

System Sequence

Create System Password: The user sets and confirms a password via HMI_ECU, which communicates with CONTROL_ECU to verify and store the password in EEPROM.

Main Options: Displayed continuously on the LCD.

Open Door: Password validation triggers motor actions to unlock then relock the door.

Change Password: Allows the user to set a new password following successful current password entry.

Password Retry Logic: Implements a three-strike rule for incorrect password attempts before triggering a lockout state with buzzer alert.

Driver and Hardware Integration

GPIO Driver: Standard driver for interfacing all GPIO pins across both ECUs.

LCD Driver: 2x16 LCD connected to HMI_ECU, using either 4-bit or 8-bit mode for displaying messages and prompts.

Keypad Driver: 4x4 Keypad connected to HMI_ECU for input.

DC Motor Driver: Controls door lock mechanism via CONTROL_ECU, utilizing Timer0 PWM for full-speed operation.

EEPROM Driver: External EEPROM interfaced with CONTROL_ECU via I2C for password storage.

I2C Driver: Modified with a configuration structure for flexible communication settings.

UART Driver: Facilitates communication between HMI_ECU and CONTROL_ECU, customized with a configuration structure.

Timer Driver: TIMER1 used in both ECUs for managing time-based actions, supporting normal and compare modes.

Buzzer Driver: Integrated into CONTROL_ECU for alerting on security breaches or incorrect password entries multiple times.


State Machine:
<img src= "https://github.com/MohamedSamy13699/Door-Locker-Security-System/blob/main/Door%20Locker%20Security%20System/FLOW%20CHART/flow%20chart.png">
proteus:
<img src= "https://github.com/MohamedSamy13699/Door-Locker-Security-System/blob/main/Door%20Locker%20Security%20System/Proteus/schmatic.PNG">
