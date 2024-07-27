# RSA Encryption and Decryption on MKL25Z4

This repository contains C code that implements RSA encryption and decryption functionalities on the MKL25Z4 microcontroller, utilizing its hardware features such as GPIO ports and peripherals. The system interacts with the user through a 16x2 LCD and a keypad.

## Project Overview

This project demonstrates a simple numeric encryption algorithm based on RSA, which involves:
- Generating keys from two prime numbers.
- Encrypting and decrypting messages using these keys.
- Interfacing with a keypad and LCD to receive inputs and display outputs.

## Features

- **Encryption and Decryption Modes:** The code supports two operation modes, which can be selected via the keypad.
- **Numeric Input:** Users can enter an 8-digit numeric message via the keypad.
- **Display Output:** After processing, the encrypted or decrypted message is displayed on the LCD.

## Main Functionalities

- **Key Generation:** Calculates encryption and decryption keys using provided prime numbers.
- **Modular Arithmetic:** Implements modular exponentiation to manage large integers without overflow.
- **User Interaction:** Guides the user with prompts on the LCD and takes input from the keypad.

## Usage

1. **Initialize the System:** Set up the LCD and keypad interfaces.
2. **Select Mode:** Choose between encryption ('A') and decryption ('D') modes.
3. **Input Message:** Enter an 8-digit number.
4. **View Result:** The processed message is displayed on the LCD.

## Building and Running
1. Compile the code using an appropriate ARM Cortex-M0 compiler that supports the MKL25Z4 microcontroller.
2. Load the binary onto the microcontroller using a debugger or flash programmer.
3. Interact with the system via the keypad and observe results on the LCD.


## Code Structure

```c
#include <MKL25Z4.H>
#include "utils.h"  // Utility functions for LCD and keypad

void main(void) {
    // Initialization
    LCD_init();
    keypad_init();

    // Key generation and mode selection
    int mode = get_number_from_keypad();
    process_mode(mode);

    // Message input and processing
    long message = get_number_from_keypad();
    process_message(message);

    // Display result
    display_result();
}

// Example of a utility function
void LCD_init(void) {


    // Commands to initialize LCD
}

void process_mode(int mode) {
    // Encryption or decryption
}

