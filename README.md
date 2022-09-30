# FEMTA PME & FTE Arduinos

Due to new developments during the Summer of 2022, as well as plans to move away from using Exp in Spring 2021, the software and electronics team has completely refactored their approach to the avionics of the Propellant Management Experiment and the FEMTA Thruster Experiment. Rather than using a Pi-based flight computer with an in-house scripting language to collect data on the ground, the avionics subteam is developing a system to collect and to transition between different states of the Blue Origin New Shepard  launch. In comparison to using a system like Exp to run experiments, the avionics subteam proposes a simpler Arduino system that can accomplish testing on the ground and data collection for the FEMTA Suborbital Experiment. We aim to accomplish three tasks moving forward for this semester:

- Software development of Arduino-based system on the Adafruit Feather M0 Logger to collect and govern transitions from one flight stage to the next
- Electronics design and manufacturing of a new printed circuit board to contain the required circuitry for the Adafruit Feather
- Repeated verification and testing of both software and electronics elements in multiple conditions, to collect meaningful data for analysis on the hardware

Two experiments are conducted on the FEMTA Payload on board the New Shepard Rocket: the Propellant Management Experiment and the FEMTA Thruster Experiment. Two Adaloggers will thus be used to control each experiment.

## Propellant Management Experiment

The Propellant Management Experiment (PME) is a system that provides the FEMTA thruster with a means of refueling and controlling the propellant while in the vacuum. For the PME, the Arduino in this experiment is tasked with the three following steps:

- Govern transitions between states of flight (launch, ascent, descent, main-engine cutoff)
- Collect pressure, temperature, and flow-rate data from sensors on the PME hardware 
- Collect ambient pressure data on its surroundings, while observing an internal time variable, to help determine transitions between states

The last step motivates the first step. In order to properly collect data during during the main-engine cutoff stage, the software must be able to deduce the flight state of the New Shepard rocket. For instance, during launch, we must send a signal to open the vent valve in the collection chamber, while keeping the flow solenoids closed. If the vent valve remained closed, the lack of a pressure differential does not produce a flow through the tubing connecting the propellant tanks to the collection chamber. Therefore, the flow sensors do not receive any meaningful data to indicate that propellant has flowed through its sensors. Knowing the importance of a decision-making algorithm to determine state transitions is critical to the experiment, as it satisfies the second step, data collection.

## FEMTA Thruster Experiment

The FEMTA Thruster Experiment (FTE) will characterize the thrust of the FEMTA thruster in a vacuum. This will be accomplished along with the Propellant Management Experiment. The Arduino in this experiment will be tasked with the three following steps:

- Auto calibrate the thrust stand on the FEMTA thruster
- Delegate power from the Arduino into the FEMTA thruster
- Collect data from the thrust stand

# Navigation

The folder structure of the arduino Github is easy to follow. The following list below describes the contents of each folder.

- datasheets: Relevant sensors used in the PME and FTE have datasheets, which are contained in this folder. This is a convenience for those who will need to interface sensors with the Arduino
- libraries: Arduino contains libraries used to communicate with sensors. In particular, the Wire library is a necessary module with capabilities to interface with I2C devices
- logs: The Feather M0 must produce data from its sensor readings such that one can take the data and produce a plot. The subteam expects to use .csv or delimited .txt files
- src: This folder houses the .ino sketches. The subteam expects to have two main files, one for the PME and one for the FTE. In addition, sketches in this folder will point to the "libraries" folder
