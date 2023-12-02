
clc; clear; close all;

data = readmatrix("FT21.TXT");

time = data(:, 1);
pressure = data(:, 7);
temperature = data(:, 12);

figure;
plot(time / 1000, pressure * 0.1);

xlabel("Time")
ylabel("Pressure (kPa)")
grid on
title("Pressure of Propellant Tank During Leak Test")

figure;
plot(time / 1000, temperature);
grid on
xlabel("Time")
ylabel("Temperature (deg C)")
title("Temperature of Propellant Tank During Leak Test")