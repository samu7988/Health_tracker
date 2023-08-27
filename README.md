<a href="https://healthtracker.org/">
    <img src="https://github.com/samu7988/Health_tracker/blob/main/Pictures/logo.jpeg" alt="Health Tracker logo" title="Health Tracker" align="right" height="60" />
</a>

# Health Tracker


:star: [Health Tracker](https://github.com/samu7988/Health_tracker/tree/main/Health_tracker) is a low power embedded system designed to measure heartbeat and detect free-fall.

> **Goal**
> 
> In a world where safety meets innovation,  Health Tracker Project takes center stage. Imagine a scenario where a person's well-being is monitored beyond the ordinary – not just heartbeats, but even the unexpected. Free fall events, those moments when someone needs help the most, are now under the vigilant watch of our technology.


![Health-Tracker](https://github.com/samu7988/Health_tracker/blob/main/Pictures/main.png)

# Health Tracker - Pulse Rate & Free Fall Monitoring

## Table of Contents

- [Project Proposal](#project-proposal)
  - [Goal](#goal)
  - [Student Names](#student-names)
- [High-Level Requirements](#high-level-requirements)
- [High-Level Design](#high-level-design)
- [GATT profile](#gatt-profile)
- [Functional Hardware Block Diagram](#functional-hardware-block-diagram)
- [Functional Software Block Diagram](#functional-software-block-diagram)
- [Dataflow Diagram](#dataflow-diagram)
- [Test Plan](#test-plan)
- [Firmware Changes](#firmware-changes)
- [Energy Profiler Output](#energy-profiler-output)
  - [When pulse sensor is enabled/disabled](#When-pulse-sensor-is-enabled/disabled)
  - [When pulse sensor free-fall is detected](#When-pulse-sensor-free-fall-is-detected)
- [Challenges Encountered](#challenges-encountered)
- [Links](#links)

## Project Proposal

### Goal

The goal of this project is to emulate a health tracker capable of monitoring pulse rate (heartbeats) and detecting free falls. The captured data is then sent to a smartphone application via Bluetooth Low Energy protocol. The pulse rates are captured using a pulse rate sensor from Sparkfun, while free falls are detected using an accelerometer.

### Student Names

- Sayali Mule (sayali.mule@colorado.edu)

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

### GATT profile

![GATT Profile](https://github.com/samu7988/Health_tracker/blob/main/Pictures/gatt.png)

### Functional Hardware Block Diagram

![Hardware Diagram](https://github.com/samu7988/Health_tracker/blob/main/Pictures/HW.png)

### Functional Software Block Diagram

![Software Diagram](https://github.com/samu7988/Health_tracker/blob/main/Pictures/software.png)

### Dataflow Diagram

![Dataflow Diagram](https://github.com/samu7988/Health_tracker/blob/main/Pictures/dataflow.png)

## Test Plan

[Link to Test Plan](https://link-to-test-plan-document.com)


### Firmware Changes

- Interfaced the heart rate sensor (pulse sensor).
- Implemented pulse sensor functionality to read BPM values.
- Interfaced the accelerometer (ADXL345) for free-fall detection.
- Developed accelerometer APIs for reading and configuring.
- Verified functionality of pulse sensor and accelerometer.
- Implemented accelerometer interrupt for free-fall detection.
- Developed circular buffer for handling sensor data.
- Configured GATT characteristics for pulse sensor and accelerometer.

### Energy Profiler Output

#### When pulse sensor is enabled/disabled
![Energy Profiler Output Enable Disable](https://github.com/samu7988/Health_tracker/blob/main/Pictures/enabledisable.png))

#### When pulse sensor free-fall is detected
![Free fall detection](https://github.com/samu7988/Health_tracker/blob/main/Pictures/pulse.png)

### Challenges Encountered

- List any challenges faced during this update and how they were resolved.

## Links

* [Web site](https://aimeos.org/integrations/typo3-shop-extension/)
* [Documentation](https://aimeos.org/docs/TYPO3)
* [Forum](https://aimeos.org/help/typo3-extension-f16/)
* [Issue tracker](https://github.com/aimeos/aimeos-typo3/issues)
* [Source code](https://github.com/aimeos/aimeos-typo3)

