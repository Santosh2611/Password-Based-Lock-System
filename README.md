A mini-embedded C project based on an automatic door lock system that takes a four-digit password as input and loads the output. NXP's LPC2148 Microcontroller is used to actuate the load, a phone keypad to get input from the user and a DC motor to unlock the door. An LCD module is also connected to display the required information.

Once the correct code or password is entered, the door is opened, and the concerned person is allowed access to the secured area. Again, if another person arrives, it will ask to enter the password. If the password is wrong, then the door will remain closed, thereby denying access to the person.

---

# Password-Based Lock System Using ARM7 (LPC2148)

This project is an implementation of a password-based lock system using ARM7 (LPC2148), which integrates features like a numeric keypad, LCD, and control of a DC motor based on the correct password input.

## Hardware Components
- LPC2148 Microcontroller
- 4x3 Numeric Keypad
- 16x2 Alphanumeric LCD Display
- DC Motor

## Code Overview
The code starts by setting the default password and defining the necessary functions for motor control, LCD interfacing, and keypad input handling. The main function then initializes the hardware and enters an infinite loop for the lock system functionality.

### Main Function
1. Displays "Enter Password" message on the first line of the LCD.
2. Accepts user input from the keypad and masks the input with asterisks on the display.
3. Compare the input with the default password and take action accordingly:
   - If the password is correct, it displays a success message, operates the motor in both directions and then stops.
   - If the password is incorrect, it displays a failure message.

### Keypad and LCD Functions
The project includes functions to handle keypad input and interact with the LCD for showing messages and masking the password input.

### DC Motor Control
The code defines functions to control the DC motor to rotate in forward, reverse, and stop states.

## Usage
To use this project, you need to interface the LPC2148 microcontroller with the specified hardware components mentioned above. Additionally, ensure that the necessary connections are made and the libraries for the microcontroller are appropriately included.
