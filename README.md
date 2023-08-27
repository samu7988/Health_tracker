<a href="https://aimeos.org/">
    <img src="https://github.com/samu7988/Health_tracker/blob/main/Pictures/logo.jpeg" alt="Health Tracker logo" title="Health Tracker" align="right" height="60" />
</a>

# Health Tracker


:star: [Health Tracker](https://github.com/samu7988/Health_tracker/tree/main/Health_tracker) is a low power embedded system designed to measure heartbeat and detect free-fall.

> **Goal**
> 
> In a world where safety meets innovation,  Health Tracker Project takes center stage. Imagine a scenario where a person's well-being is monitored beyond the ordinary – not just heartbeats, but even the unexpected. Free fall events, those moments when someone needs help the most, are now under the vigilant watch of our technology..


![Health-Tracker](https://github.com/samu7988/Health_tracker/blob/main/Pictures/main.png)

# Health Tracker - Pulse Rate & Free Fall Monitoring

![Project Logo](https://link-to-your-logo.com)

## Table of Contents

- [Project Proposal](#project-proposal)
  - [Goal](#goal)
  - [Student Names](#student-names)
  - [Project Overview](#project-overview)
- [High-Level Requirements](#high-level-requirements)
- [High-Level Design](#high-level-design)
- [Functional Hardware Block Diagram](#functional-hardware-block-diagram)
- [Functional Software Block Diagram](#functional-software-block-diagram)
- [Dataflow Diagram](#dataflow-diagram)
- [Division of Labor](#division-of-labor)
- [Subsystem Summary](#subsystem-summary)
- [Test Plan](#test-plan)
- [Proposed Schedule](#proposed-schedule)
- [Update 1](#update-1)
- [Update 2](#update-2)
- [Final Report](#final-report)
- [Distribution of Work](#distribution-of-work)
- [Learnings](#learnings)
- [Energy Profiler Output](#energy-profiler-output)

## Project Proposal

### Goal

The goal of this project is to emulate a health tracker capable of monitoring pulse rate (heartbeats) and detecting free falls. The captured data is then sent to a smartphone application via Bluetooth Low Energy protocol. The pulse rates are captured using a pulse rate sensor from Sparkfun, while free falls are detected using an accelerometer.

### Student Names

- Sayali Mule (sayali.mule@colorado.edu)

### Project Overview

Team 16 has chosen to proceed with option 1. The project involves creating a system with a server (Blue Gecko board) and a client (mobile phone application) to monitor pulse rates and detect free falls. The server communicates the collected data to the client via BLE protocol.

## High-Level Requirements

1. The system consists of a server (Blue Gecko) and a client (mobile phone).
2. The Blue Gecko board advertises GATT services.
3. GATT service includes characteristics for free fall detection (accelerometer) and heart rate sensing (pulse rate sensor).
4. Values are sent to the cellphone client every 10 seconds.
5. Encrypted link established between server and client via bonding.
6. Server sends values to the client every 10 seconds before entering sleep mode.
7. Server operates in the lowest possible energy mode.
8. Communication between server and client uses BLE.
9. Client displays received free fall and pulse rate values.
10. Server's LCD displays connection status and current indications.
11. Client's application displays free fall and pulse rate values.

## High-Level Design

The project consists of a server (Blue Gecko board) and a client (Bluetooth-based mobile app). The server measures free fall and pulse rate values, which are transmitted to the client via BLE.

### Functional Hardware Block Diagram

![Hardware Diagram](https://link-to-hardware-diagram.com)

### Functional Software Block Diagram

![Software Diagram](https://link-to-software-diagram.com)

### Dataflow Diagram

![Dataflow Diagram](https://link-to-dataflow-diagram.com)

## Division of Labor

The project's hardware components (sensors, boards) were purchased off the shelf. Hardware development constituted about 5% of the effort. The majority of the work was focused on software design and implementation, particularly firmware for sensor reading and BLE communication.

## Subsystem Summary

The project comprises a Blue Gecko server and a mobile client application. The system involves both hardware and software components. The server mostly operates in a low-power mode and sends values to the client every 10 seconds. Both server and client display GATT service values.

## Test Plan

[Link to Test Plan](https://link-to-test-plan-document.com)

## Proposed Schedule

Refer to the proposed schedule in the provided document.

## Update 1

### Progress Summary

Summarize the work done in this update.

### Firmware

- Interfaced the heart rate sensor (pulse sensor).
- Implemented pulse sensor functionality to read BPM values.
- Interfaced the accelerometer (ADXL345) for free fall detection.
- Developed accelerometer APIs for reading and configuring.

### Challenges Encountered

- List any challenges faced during this update and how they were resolved.

### Proposed Schedule

Refer to the proposed schedule in the provided document.

## Update 2

### Progress Summary

Summarize the work done in this update.

### Firmware

- Verified functionality of pulse sensor and accelerometer.
- Implemented accelerometer interrupt for free fall detection.
- Developed circular buffer for handling sensor data.
- Configured GATT characteristics for pulse sensor and accelerometer.

### Challenges Encountered

- List any challenges faced during this update and how they were resolved.

### Proposed Schedule

Refer to the proposed schedule in the provided document.

## Final Report

### Progress Summary

Summarize the final update and status of the project.

### Firmware

- Verified system functionality and low-power modes.
-


## Links

* [Web site](https://aimeos.org/integrations/typo3-shop-extension/)
* [Documentation](https://aimeos.org/docs/TYPO3)
* [Forum](https://aimeos.org/help/typo3-extension-f16/)
* [Issue tracker](https://github.com/aimeos/aimeos-typo3/issues)
* [Source code](https://github.com/aimeos/aimeos-typo3)

