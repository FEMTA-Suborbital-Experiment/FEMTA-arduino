#!/bin/bash
# TODO: Replace with Makefile, CMake, whatever people are using nowadays
#
# This test file links the StateLogic class with the atmospheric profile
# directly in order to test functionality on a local system. This test
# should eventually be replaced with a better-integrated unit test
# (using AUnit or some other framework) and/or an integration test
# with the rest of the PME mainloop.
#
# This file must be run in this directory.

# cd to this directory
parent_path=$( cd "$(dirname "${BASH_SOURCE[0]}")" ; pwd -P )
cd "$parent_path"

g++ -I. -I../../main/pme/src ../../main/pme/src/StateLogic.cpp ../../main/pme/src/AtmSphericProf.cpp test_statelogic.cpp
