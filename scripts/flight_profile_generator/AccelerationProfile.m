%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% AUTHOR: JACOB VALDEZ
% FEMTA Suborbital Experiment
% New Shepard Accecleration Profile Generator
% Generate a complete acceleration profile based on Blue Origin's New
% Shepard rocket. The acceleration profile follows Figure 5b of Llanos et
% al. (2019). The purpose of this file is to provide an acceleration
% profile input for the flight simulation, which depends entirely on the
% acceleration and pressure profiles.
% 
% Llanos et al. (2019), https://doi.org/10.2478/gsr-2019-000.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

clear; clc; close all;

%set(groot,'defaultAxesTickLabelInterpreter','latex');  
%set(groot, 'defaulttextinterpreter', 'latex')
%set(groot,'defaultLegendInterpreter','latex');

%% PARAMETERS

% Specify noise amplititude based on the five stages of flight: preflight, ascent,
% zero-g, descent, and postflight
amplitude = [0.1 0.25 0.06 1 0.25];

%% INITIALIZATIONS

frequency = 0.1;
t = linspace(0,600,6000);
metric = 9.806625;
imperial = 32;

acceleration = zeros(size(t));

m1 = (1.644-1.452)/36;
b1 = 1.644 - m1*36;
m2 = (1.243-1.644)/(46-36);
b2 = 1.243 - m2*46;
m3 = (2.872-1.243)/(120-46);
b3 = 2.872 - m3*120;
m4 = (0.11-2.872)/(132-120);
b4 = 0.11 - m4*132;
m5 = 4.831/(356-340);
b5 = 4.831 - m5*356;
m6 = (1.653-4.831)/(380-356);
b6 = 1.653 - m6*380;
m7 = (1.124 - 1.653)/(439-280);
b7 = 1.124 - m7*439;
m8 = (8.372 - 1.124)/(447-339);
b8 = 8.372 - m8*447;
m9 = (1.172 - 8.372)/(466-347);
b9 = 1.172 - m8*466;

%% ACCELERATION GENERATION

% Generate a noiseless profile of the acceleration. A noise profile is
% applied based on the parameters of the user.
for i = 1:length(t)
    noise = 2*rand(1)-1;
    if (i < 70)
        acceleration(i) = 1 + noise*amplitude(1);
    elseif (i < 360)
        acceleration(i) = b1 + frequency*i*m1 + noise*amplitude(2);
    elseif (i < 460)
        acceleration(i) = b2 + frequency*i*m2 + noise*amplitude(2);
    elseif (i < 1200)
        acceleration(i) = b3 + frequency*i*m3 + noise*amplitude(2);
    elseif (i < 1320)
        acceleration(i) = b4 + frequency*i*m4 + noise*amplitude(2);
    elseif (i < 3300)
        acceleration(i) = 0 + noise*amplitude(3);
    elseif (i < 3560)
        acceleration(i) = b5 + frequency*i*m5 + noise*amplitude(3);
    elseif (i < 3800)
        acceleration(i) = b6 + frequency*i*m6 + noise*amplitude(4);
    elseif (i < 4390)
        acceleration(i) = b7 + frequency*i*m7 + noise*amplitude(4);
    elseif (i < 4470)
        acceleration(i) = b8 + frequency*i*m8 + noise*amplitude(4);
    elseif (i < 4660)
        acceleration(i) = 1 + noise*amplitude(5);
    elseif (i <= 6000)
        acceleration(i) = 1 + noise*amplitude(5);
    end
end

acceleration(acceleration < 0) = 0;

% Spike the acceleration at particular areas
for i = 1:length(t) 
    noise = 2*rand(1)-1;
    if (i >= 1545 && i < 1560)
        acceleration(i) = 3 + noise*amplitude(3);
    elseif (i >= 4800 && i < 5100)
        acceleration(i) = 2 + noise*amplitude(4);
    elseif (i >= 5985 && i < 5995)
        acceleration(i) = 2 + noise*amplitude(5);
    end
end

plot(t, acceleration)
grid on
title("Acceleration Profile")
xlabel("Time (s)")
ylabel("Acceleration (g)")
set(gca, 'FontSize', 45)
set(gca, "LineWidth", 4)

lineWidth = 2.5;

xline(24, "r--", "Liftoff; 1", 'FontSize', 30, 'LineWidth', lineWidth);
xline(3.60, "b--", "Liftoff; 1", 'FontSize', 30, 'LineWidth', lineWidth);
xline(130, "b--", "MECO; 2", 'FontSize', 30, 'LineWidth', lineWidth);
xline(150, "r--", "MECO; 2", 'FontSize', 30, 'LineWidth', lineWidth);
xline(180, "b--", "Start Experiment; 3", 'FontSize', 30, 'LineWidth', lineWidth);
xline(326, "b--", "End Experiment; 4", 'FontSize', 30, 'LineWidth', lineWidth);
xline(353, "b--", "Descent; 5", 'FontSize', 30, 'LineWidth', lineWidth);
xline(470, "b--", "Landed; 6", 'FontSize', 30, 'LineWidth', lineWidth);

%out_metric = [t', acceleration' * metric];
%out_imperial = [t', acceleration' * imperial];

%writematrix(out_metric, "acceleration_Full_metric.csv");
%writematrix(out_imperial, "acceleration_Full_imperial.csv");