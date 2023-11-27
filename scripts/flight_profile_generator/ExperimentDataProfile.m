%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% AUTHOR: JACOB VALDEZ
% FEMTA Suborbital Experiment
% New Shepard Experiment Data Profile Generator
% Generate a complete flight profile of the experiment profiles: propellant
% tank pressure and temperature, and propellant flow. This simulated
% profile will be used as input to test the passing of data between
% different FEMTA Software systems.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

clear; clc; close all;

%set(groot,'defaultAxesTickLabelInterpreter','latex');  
%set(groot, 'defaulttextinterpreter', 'latex')
%set(groot,'defaultLegendInterpreter','latex');

%% INITIALIZATIONS

pa_to_torr = 1/133.322368;
torr_to_mtorr = 1e3;

altitude_profile = [0,3.65
    2,3.65
    6,3.65
    7,3.65
    10,3.8
    11,3.9
    12,4.1
    13,4.2
    15,4.5
    25,7
    40,11
    50,20
    75,37
    100,70
    125,130
    128,140.9
    146,196.7
    162,242.2
    177,277.8
    200,315
    225,340
    242,350
    246,351
    250,350
    263,344
    275,335
    300,300
    332,235.6
    340,208
    348,187.6
    364,132.9
    380.9,80.9
    381,80.9
    381.1,80.9
    400,25
    410,15
    415,10
    420,7
    425,5
    430,3.65
    431,3.65
    500,3.65
    600,3.65];

%% ALTITUDE GENERATION

skip1 = 60;
skip2=414;
time = linspace(0,600,6000);

time1=time(time<= 7);
time2=time(time>7 & time<=skip1);
time3=time(time>skip1 & time<=skip2);
time4=time(time>skip2 & time<=430);
time5=time(time>430);

p1=polyfit(altitude_profile(4:40,1),altitude_profile(4:40,2),5);
alt1=0.*time1+3.65;
alt3 = polyval(p1,time3);
alt5=0.*time5+3.65;

b = alt3(1);
a = skip1;
w = log(b-2.65) / (a-7);
alt2 = exp(w.*(time2 - 7)) + 2.65;

b = alt3(length(alt3));
a = skip2;
w = log(b-2.65) / (a-430);
alt4 = exp(w.*(time4 - 430)) + 2.65;

alt=[alt1,alt2,alt3,alt4,alt5];

%% PRESSURE PROFILE

oldalt = alt;
MECO_alt_i = find(abs(oldalt - 150) < 0.1);
zeroG_time_i = find(time > 181 & time < 346);

alt = alt.*1000;
alt = alt./3.281;
[~, ~, pressure, ~] = atmoscoesa(alt);

%% COLLECTION CHAMBER PROFILE

% Units: mbar

pa_to_mbar = 0.01;

cc_pressure = pressure * pa_to_mbar;
start = [zeroG_time_i(1), cc_pressure(zeroG_time_i(1))];
goal = [zeroG_time_i(end), 30 * pa_to_mbar];
x = zeroG_time_i;
cc_pressure(zeroG_time_i(1):zeroG_time_i(end)) = point_slope(start, goal, x);

%% PVC PROFILE

w = randn(size(pressure)) * 2000;

mtorr_pressure = pressure*pa_to_torr*torr_to_mtorr;
mtorr_max_i = find(mtorr_pressure - 10000 < 0.1);
mtorr_max = mtorr_pressure(mtorr_max_i(1));
mtorr_max2 = mtorr_pressure(mtorr_max_i(end));
mtorr_min_i = find(mtorr_pressure - 1e-1 < 0.1);
mtorr_pressure(1:mtorr_max_i(1)) = linspace(760000, mtorr_max, mtorr_max_i(1));
mtorr_pressure(mtorr_max_i(end):end-mtorr_max_i(1)) = linspace(mtorr_max2, 760000, (length(mtorr_pressure)-mtorr_max_i(1))-mtorr_max_i(end)+1);
mtorr_pressure(end-mtorr_max_i(1):end) = 760000;
mtorr_pressure(mtorr_min_i(1):mtorr_min_i(end)) = 1e-1;

pvc_pressure = mtorr_pressure + w;

%% WRITE TO FILE

writematrix(pvc_pressure, 'pvc_pressure.csv');
writematrix(cc_pressure, 'cc_pressure.csv');

%% PLOT PRESSURE AND ALTITUDE PROFILES

plot(altitude_profile(:,1),altitude_profile(:,2)*1000 / 3.281, 'o')
grid on
xlabel('time (seconds)')
ylabel('Geodetic Altitude (thousand feet)')
title('Altitude Profile')
hold on
plot(time,alt,'--')
legend('Data Points', 'Fit')
xline(7,'--k',{'Liftoff'},'HandleVisibility','off')
xline(146,'--k',{'Meco'},'HandleVisibility','off')
xline(246, '--k', {'Apogee'},'HandleVisibility','off')
xline(430, '--k', {'Land'},'HandleVisibility','off')
hold off

figure;
plot(time,pressure)
grid on
title("Pressure Profile")
xlabel('time (s)')
ylabel('Pressure (Pa)')
xline(7,'--k',{'Liftoff'},'HandleVisibility','off')
xline(146,'--k',{'Meco'},'HandleVisibility','off')
xline(246, '--k', {'Apogee'},'HandleVisibility','off')
xline(430, '--k', {'Land'},'HandleVisibility','off')

figure;
semilogy(time,pressure)
grid on
title("Pressure Profile")
xlabel('time (s)')
ylabel('Pressure (Pa)')
xline(7,'--k',{'Liftoff'},'HandleVisibility','off')
xline(146,'--k',{'Meco'},'HandleVisibility','off')
xline(246, '--k', {'Apogee'},'HandleVisibility','off')
xline(430, '--k', {'Land'},'HandleVisibility','off')

figure;
plot(time,cc_pressure / pa_to_psi)
hold on
plot(time,pvc_pressure / pa_to_torr / torr_to_mtorr)
legend("HSCM", "PVC")
grid on
title("HSCM and Pirani Pressure Profile")
xlabel('time (s)')
ylabel('Pressure (Pa)')
xline(7,'--k',{'Liftoff'},'HandleVisibility','off')
xline(146,'--k',{'Meco'},'HandleVisibility','off')
xline(246, '--k', {'Apogee'},'HandleVisibility','off')
xline(430, '--k', {'Land'},'HandleVisibility','off')

%% FUNCTIONS

function y = point_slope(start, goal, x)
    m = (goal(2) - start(2)) / (goal(1) - start(1));
    y = m*(x - start(1)) + goal(2);
end
