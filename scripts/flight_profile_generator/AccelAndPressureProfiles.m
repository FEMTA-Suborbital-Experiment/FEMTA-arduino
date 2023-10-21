%% Program Information
% this program makes the acceleration and altitude profiles over flight
% mission time from pre-liftoff to apogee. 
%% Setup
clear; clc; close all;

%x = rand ----- this makes a random number from 0 - 1
accelLength = 2460;
accel = zeros(accelLength, 2);

g = 9.8;
%% Acceleration Flight Profile

% liftoff at 7 seconds - index 70
% meco at 146 seconds - index 1460
% peak at 246 seconds - index 2460
% land at 430 seconds - index 4300

for i = 1:accelLength
    accel(i,1) = i;
    if i < 70 % before liftoff
        accel(i, 2) = g + rand;
    elseif (i >= 70) && (i <= 75) % eary liftoff (vibration and quick increase in acceleration)
        accel(i,2) = g + (i-70)*(1.5*g - 1*g)/(5) + rand * (0.25*g);
    elseif i < 1460 % liftoff
        accel(i, 2) = 1.5*g + (i - 75) * ((2.7*g-1.5*g) /(1460-75)) + rand * (1.6 * g + (i - 75) * (0.6 * g - 1.6 * g)/(1460-75));
    elseif (i >= 1460) && (i < 1470) % early meco (expecting a little vibration still from the engine cutoff process, and acceleration is quickly reaching zero)
        accel(i,2) = 2.7*g - 2.7*g/10*(i - 1460) + rand * (0.5 * g);
    else % meco
        accel(i, 2) = 0 + rand;
    end
end

%% Altitude/Pressure Profile
%column 1 is time (seconds) and column 2 is altitude (1,000 feet). These
%values were visually estimated from the provided altitude graph by Blue
%Origin
profile = [0,3.65
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
% liftoff at 7 seconds - index 4
% meco at 146 seconds
% peak at 246 seconds - index 23
% land at 430 seconds - index 40 
skip1 = 60;
skip2=414;
time = linspace(0,600,6000);

time1=time(time<= 7);
time2=time(time>7 & time<=skip1);
time3=time(time>skip1 & time<=skip2);
time4=time(time>skip2 & time<=430);
time5=time(time>430);

p1=polyfit(profile(4:40,1),profile(4:40,2),5);
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

alt = alt.*1000; % convert from 1k ft to 1 ft
alt = alt./3.281; % convert from ft to m
[~, ~, pressure, ~] = atmoscoesa(alt);

%% Output

Profile = zeros(2460,4); % first collumn is time (s), second column is acceleration (m/s), third column is pressure (Pa), fourth is altitude (km)
Profile(:,1) = accel(:,1)./10;
Profile(:,2) = accel(:,2);
Profile(:,3) = pressure(1:2460);
Profile(:,4) = alt(1:2460);

figure;
yyaxis left
plot(Profile(:,1), Profile(:,2));
xlabel("Time (s)");
grid on;
ylabel("Acceleration (m/s)");
yyaxis right
plot(Profile(:,1), Profile(:,3));
ylabel("Pressure (Pa)");
title("Suborbital Flight Profile");
xline(7,'--',{'Liftoff'});
xline(146,'--',{'Meco'});

% writematrix(Profile,"FlightSimProfiles.xlsx");