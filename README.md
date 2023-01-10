# FEMTA PME & FTE Arduinos

This repository contains the main sketches to operate the Propellant Management Experiment and FEMTA Thruster Experiments, as well as relevant libraries and datasheets for the sensors used in the hardware.

# Quick Start
See the [wiki](https://github.com/FEMTA-Suborbital-Experiment/FEMTA-arduino/wiki) for an onboarding tutorial to help you get started with developing for the FEMTA Avionics team.

# Navigation

The folder structure of the arduino Github is easy to follow. The following list below describes the contents of each folder.

- datasheets: Relevant sensors used in the PME and FTE have datasheets, which are contained in this folder. This is a convenience for those who will need to interface sensors with the Arduino
- libraries: Arduino contains libraries used to communicate with sensors. In particular, the Wire library is a necessary module with capabilities to interface with I2C devices
- logs: The Feather M0 must produce data from its sensor readings such that one can take the data and produce a plot. The subteam expects to use .csv or delimited .txt files
- src: This folder houses the .ino sketches. The subteam expects to have two main files, one for the PME and one for the FTE. In addition, sketches in this folder will point to the "libraries" folder
