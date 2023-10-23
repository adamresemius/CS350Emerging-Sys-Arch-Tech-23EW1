# CS350Emerging-Sys-Arch-Tech-23EW1
# Smart Thermostat Prototype

## Project Overview

Welcome to the Smart Thermostat Prototype repository, designed for the LaunchpadXL-3220S model launchpad. This project serves as a versatile IoT application for reading room temperature, controlling a heating system, and simulating data transmission to a server. It makes use of various hardware components, including the TMP006 temperature sensor, LEDs, buttons, UART communication, and a timer.

## Q/A Course Overview

### Summarize the project and what problem it was solving.

**Q:** Can you provide a brief overview of the project and the problem it aimed to address?

**A:** The project I developed is a smart thermostat that regulates heating based on user-defined temperature settings and real-time temperature readings. Its primary purpose was to offer an efficient and automated solution for maintaining comfortable indoor temperatures.

### What did you do particularly well?

**Q:** What aspects of the project did you excel at?

**A:** I excelled at understanding how various software drivers interacted with one another and how they could effectively communicate information from sensors. This understanding allowed me to seamlessly integrate hardware components into the project.

### Where could you improve?

**Q:** Are there areas within the project where you recognize room for improvement?

**A:** I identified a couple of areas where I could enhance my skills. First, I aim to become more proficient in troubleshooting errors that can arise during embedded software engineering, ensuring a smoother development process. Additionally, I plan to utilize resources and documentation more effectively to gain a deeper understanding of the intricacies of each driver I work with.

### What tools and/or resources are you adding to your support network?

**Q:** Have you incorporated any new tools or resources into your development process, and if so, what are they?

**A:** As part of my ongoing learning process, I have added various tools and resources to my support network. This includes online forums, technical documentation, and collaborative platforms where I can seek advice and share knowledge with others in the embedded systems and IoT development community.

### What skills from this project will be particularly transferable to other projects and/or course work?

**Q:** Which skills that you gained during this project do you believe will be highly valuable and transferable to future projects and coursework?

**A:** The skills I acquired during this project are particularly transferable to a broad range of projects and coursework. They include a deep understanding of embedded systems and IoT development, which is increasingly relevant in various fields. These skills will empower me to tackle complex tasks in future projects with confidence.

### How did you make this project maintainable, readable, and adaptable?

**Q:** In what ways did you ensure that your project was maintainable, readable, and adaptable for future development?

**A:** To make this project maintainable, I structured the code in a modular and organized manner, following best practices for software development. This approach enhances code readability, making it easier for others to understand and modify in the future. Additionally, I designed the project to be adaptable, allowing for straightforward customization to meet different requirements or incorporate additional features. This adaptability ensures that the project remains relevant and useful in changing contexts.


## Table of Contents

- [Hardware Components](#hardware-components)
- [Code Functionality](#code-functionality)
- [Architecture Decision](#architecture-decision)
- [Usage](#usage)
- [References](#references)

## Hardware Components

The Smart Thermostat Prototype utilizes the following hardware components:

- **TMP006 Temperature Sensor:** This project interfaces with the TMP006 temperature sensor using the I2C protocol. The sensor supports multiple sensors, each identified by a unique I2C address.

- **LEDs:** LEDs are used to provide visual feedback on the heating system's status. They light up when the heating system is active.

- **Buttons:** The thermostat features two buttons, integral for temperature control. Users can increase or decrease the set temperature using these buttons.

- **UART Communication:** UART is utilized to simulate data transmission to a server. This feature is essential for testing and verifying the communication process with an external server.

- **Timer Module:** The project relies on a timer module to control timing aspects within the thermostat, ensuring that actions occur at predetermined intervals.

## Code Functionality

The codebase of this project includes several key functionalities:

- **Temperature Sensor (I2C):** The code initializes the I2C peripheral and reads temperature data from the TMP006 sensor, providing an accurate and real-time room temperature reading.

- **LED Control (GPIO):** GPIO is configured to manage the LEDs, which serve as a visual indicator of the heating system's status.

- **Button Control (GPIO Interrupt):** GPIO interrupts detect button presses for temperature control. These buttons are used to increase or decrease the set temperature.

- **UART Data Transmission:** The UART peripheral is initialized to simulate data transmission to a server. This feature enables the project to output data, including room temperature, set temperature, heating status, and time, facilitating testing and validation of the server communication process.

- **Task Scheduler:** A task scheduler is implemented to coordinate and execute various actions within the thermostat, ensuring that all defined functionalities are performed seamlessly.

## Architecture Decision

Selecting the right microcontroller architecture is pivotal for this project. Key factors considered include peripheral support, Wi-Fi connectivity, Flash and RAM capacity, cost-effectiveness, and performance. After careful evaluation, Texas Instruments (TI) architecture was chosen as the best fit.

## Usage

To utilize this Smart Thermostat Prototype:

1. **Hardware Setup:** Ensure you have the required hardware components and the LaunchpadXL-3220S model launchpad.

2. **Code Installation:** Upload the provided code to your microcontroller, following specified configurations and pin assignments.

3. **Operation:** Power on the prototype, observe its functionality, and monitor the LEDs, buttons, and UART communication. Customize the code and hardware as needed.

## References

- [Design News - Freescale, Microchip, TI Roll Out Microcontrollers](https://www.designnews.com/freescale-microchip-ti-roll-out-microcontrollers-0)
- [Embedded Systems Design - Philadelphia University](https://www.philadelphia.edu.jo/academics/kaubaidy/uploads/ES-Slids-lec3.pdf)
- [SimpleLink™ CC3220 Wi-Fi® LaunchPad™ Development Kit Hardware](https://www.farnell.com/datasheets/2247297.pdf)
