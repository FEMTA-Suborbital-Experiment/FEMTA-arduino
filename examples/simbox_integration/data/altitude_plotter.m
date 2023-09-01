% Script to make an altitude plot of altitude.txt
% Jacob Valdez

altitude = readmatrix('altitude.txt');

plot(altitude);
xlabel("Index");
ylabel("Altitude [km]"); % Assumed to be in kilometers
title("Altitude Plot of altitude.txt");