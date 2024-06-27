## Porting µC/OS-II to the NEXYS DDR4 Development Board using Enhanced OpenMIPS Architecture

### Objective of the Experiment

The primary objective of this experiment is to port an operating system to the NEXYS DDR4 development board, facilitating subsequent program development. The operating system to be ported is µC/OS-II, and the specific porting process follows the guidelines provided in Lei Silei's book "Building Your Own CPU."

### Experiment Content

Following the approach outlined in "Building Your Own CPU," this experiment involves adding a Wishbone bus, GPIO, UART, Flash controller, and SDRAM controller. The process also includes cross-compiling the system's binary files under Ubuntu, making adjustments such as baud rate settings, and finally porting the µC/OS-II system onto the development board. The general workflow is as follows:

1. **Implement the Bus**
   - Add the Wishbone bus, GPIO, UART controller, Flash controller, and create a small SOPC.
   - Refer to the foundational concepts in the "Building Your Own CPU" book.

2. **System Porting to µC/OS-II**
   - Establish a cross-compilation environment using Ubuntu.
   - Rewrite and compile the µC/OS-II system.
   - Refer to Chapter 15 of "Building Your Own CPU."

3. **Verification and Testing on the Board**
   - Use a serial port assistant to observe and verify the loading results.
  
### Experiment Procedure

#### 3.1 Experiment Environment

##### 3.1.1 Software Environment
- **Operating System:** Windows 10 (64-bit)
- **Development Environment:** Vivado v2019.2 (64-bit), Visual Studio Code
- **Simulation Environment:** Vivado v2019.2 (64-bit)
- **Testing Environment:** Vivado v2019.2 (64-bit)
- **Serial Port Assistant:** sscom5.13.1.exe

##### 3.1.2 Hardware Environment
- **Local Machine:** Lenovo XiaoXinPro 16ACH 2021
- **Server:** Windows Subsystem for Linux (WSL)
- **Development Board:** NEXYS 4 DDR Atrix-7

#### 3.2 Principle of the Experiment

The overall block diagram of the practical OpenMIPS system design is shown below:
![image](https://github.com/WinstonLiyt/mips2-c-os-II/assets/104308117/9610931a-3839-494d-b141-6709fc62d8fb)

In this experiment, the previous 89 versions of the experimental OpenMIPS will be improved to create a practical version of OpenMIPS for porting the µC/OS-II system. The design approach and improvements are illustrated in the diagram below.
![image](https://github.com/WinstonLiyt/mips2-c-os-II/assets/104308117/42f59f55-35a2-444b-b752-20087789fe29)
![image](https://github.com/WinstonLiyt/mips2-c-os-II/assets/104308117/731a69e7-ae4a-46bd-a917-4007d9eec522)

**For more details:**
1. **Embedded Real-Time Operating System (RTOS) Characteristics:**
   - An RTOS executes predefined tasks with specific requirements.
   - It reduces size and cost while ensuring timely responses and high reliability.
   - RTOS like μC/OS-II are used in various applications, including mobile phones, routers, UPS, and aircraft, with proven reliability certified by the FAA.

2. **Porting Process:**
   - The μC/OS-II system is ported to the NEXYS 4 DDR development board.
   - The process includes setting up a cross-compilation environment on Ubuntu to modify and compile the μC/OS-II system.

3. **System Integration:**
   - The experiment builds upon a base OpenMIPS CPU implementation, enhancing it with components like Wishbone bus, GPIO, UART, Flash controller, and SDRAM controller.
   - These components are integrated into a small System on Programmable Chip (SOPC) system to support the μC/OS-II operating system.

4. **Verification:**
   - The system's functionality is verified using a serial port tool to observe the results.
   - The successful execution of the μC/OS-II system on the development board demonstrates the porting success.
