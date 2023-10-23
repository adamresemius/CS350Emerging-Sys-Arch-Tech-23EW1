/*
 * Copyright (c) 2015-2020, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ======== gpiointerrupt.c ========
 */
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
/* Driver Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/I2C.h>
#include <ti/drivers/UART2.h>
#include <ti/drivers/Timer.h>

/* Driver configuration */
#include "ti_drivers_config.h"

// Constants
#define DISPLAY(x) UART2_write(uart, &output, x, NULL);
#define TIMER_PERIOD 1000 // Timer period in milliseconds
#define Timer_PERIOD_MS Timer_PERIOD_US // Assuming milliseconds

// Custom I2C Transaction Structure
typedef struct {
    uint8_t targetAddress;
    uint8_t *writeBuf;
    size_t writeCount;
    uint8_t *readBuf;
    size_t readCount;
} CustomI2C_Transaction;

// Sensor information
static const struct {
    uint8_t address;
    uint8_t resultReg;
    char *id;
} sensors[3] = {
    { 0x48, 0x0000, "11X" },
    { 0x49, 0x0000, "116" },
    { 0x41, 0x0001, "006" }
};

uint8_t txBuffer[1];
uint8_t rxBuffer[2];
CustomI2C_Transaction i2cTransaction;  // Custom I2C transaction structure
I2C_Handle i2c;
char output[64];
UART2_Handle uart;
bool buttonUpPressed = false;
bool buttonDownPressed = false;
int setPointTemperature = 25;
bool heatIsOn = false;

// Function to read temperature
int16_t readTemp(void) {
    int16_t temperature = 0;
    i2cTransaction.readCount = 2;
    i2cTransaction.writeBuf = txBuffer;
    i2cTransaction.writeCount = 1;
    i2cTransaction.readBuf = rxBuffer;
    i2cTransaction.readCount = 0;

    if (I2C_transfer(i2c, &i2cTransaction)) {
        temperature = (rxBuffer[0] << 8) | rxBuffer[1];
        temperature *= 0.0078125;

        if (rxBuffer[0] & 0x80) {
            temperature |= 0xF000;
        }
    } else {
        // Handle temperature sensor reading error
    }
    return temperature;
}

// Function to initialize UART
void initUART(void) {
    UART2_Params uartParams;
    UART2_Params_init(&uartParams);
    uartParams.baudRate = 115200;
    uartParams.readReturnMode = UART2_ReadReturnMode_FULL;
    uart = UART2_open(CONFIG_UART2_0, &uartParams);
    if (uart == NULL) {
        // Handle UART initialization failure
        while (1);
    }
}


// Function to send data to UART
void sendToUART(const char *message) {
    UART2_write(uart, message, strlen(message), UART2_Mode_BLOCKING);
}

// Function to initialize I2C
void initI2C(void) {
    int8_t i, found;
    I2C_Params i2cParams;
    I2C_Transaction i2cTransaction;  // Use I2C_Transaction

    DISPLAY(snprintf(output, 64, "Initializing I2C Driver - "));
    I2C_init();
    I2C_Params_init(&i2cParams);
    i2cParams.bitRate = I2C_400kHz;
    i2c = I2C_open(CONFIG_I2C_0, &i2cParams);
    if (i2c == NULL) {
        DISPLAY(snprintf(output, 64, "Failed\n\r"));
        while (1);
    }
    DISPLAY(snprintf(output, 32, "Passed\n\r"));

    i2cTransaction.writeBuf = txBuffer;
    i2cTransaction.writeCount = 1;
    i2cTransaction.readBuf = rxBuffer;
    i2cTransaction.readCount = 0;
    found = false;

    for (i = 0; i < 3; ++i) {
        i2cTransaction.targetAddress = sensors[i].address;
        txBuffer[0] = sensors[i].resultReg;
        DISPLAY(snprintf(output, 64, "Is this %s? ", sensors[i].id));
        if (I2C_transfer(i2c, &i2cTransaction)) {
            DISPLAY(snprintf(output, 64, "Found\n\r"));
            found = true;
            break;
        }
        DISPLAY(snprintf(output, 64, "No\n\r"));
    }

    if (found) {
        DISPLAY(snprintf(output, 64, "Detected TMP%s I2C address: %x\n\r", sensors[i].id, i2cTransaction.targetAddress));
    } else {
        DISPLAY(snprintf(output, 64, "Temperature sensor not found...\n\r"));
    }
}



// Function to initialize timers
void initTimers(void) {
    Timer_Params timerParams;
    Timer_Handle timer0;

    // Initialize the Timer module
    Timer_init();

    // Configure timer parameters
    Timer_Params_init(&timerParams);
    timerParams.period = TIMER_PERIOD; // Define the timer period in milliseconds
    timerParams.periodUnits = Timer_PERIOD_MS; // Specify the time units

    // Open the timer instance
    timer0 = Timer_open(CONFIG_TIMER_0, &timerParams);
    if (timer0 == NULL) {
        // Handle timer initialization failure
        while (1);
    }

    // Start the timer
    if (Timer_start(timer0) == Timer_STATUS_ERROR) {
        // Handle timer start failure
        while (1);
    }
}

// Main thread
void *mainThread(void *arg0) {
    GPIO_init();
    initI2C();
    initUART();
    initTimers();
    uint32_t seconds = 0;

    while (1) {
        int16_t temperature = readTemp();

        // Button-related code and setPointTemperature update
        if (buttonUpPressed) {
            setPointTemperature++;
            buttonUpPressed = false;
        } else if (buttonDownPressed) {
            setPointTemperature--;
            buttonDownPressed = false;
        }

        // LED control - Implement your LED control logic here.

        // UART data transmission
        if (temperature > setPointTemperature) {
            heatIsOn = false;
        } else if (temperature < setPointTemperature) {
            heatIsOn = true;
        }
        GPIO_write(CONFIG_GPIO_LED_0, heatIsOn ? CONFIG_GPIO_LED_ON : CONFIG_GPIO_LED_OFF);

        snprintf(output, 64, "<%02d,%02d,%d,%04ld>", temperature, setPointTemperature, heatIsOn, seconds);
        sendToUART(output);

        seconds++;
        usleep(1000000);
    }
    return (NULL);
}
