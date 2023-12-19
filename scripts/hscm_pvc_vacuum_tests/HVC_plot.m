% VIP 37920 Fall 2023
% Pirani and HSCM Test - Plot Code
% Authors: Gabriel Oliveira and Jacob Valdez

% Comments on numbers for each trial:
% 0: Atmospheric Run
% 1: Small Low Vacuum Chamber
% 2: Big High Vacuum Chamber

% Change plot data to kPa for better interpretation
% Time Format Description      h:m:s
% First measurement of data: 10:00:01.33464813232421875
% First measurement of data1: 11:47:05.8331270217895507812

clc; clf; clear;

%% Initialization

%For High Vacuum Chamber Test
data = readmatrix('102123.txt'); % Pirani (mTorr) and HSCM (psi)
lvm_1 = lvm_import('data.lvm',0); % HVC Data for Comparison (towards vacuum)
lvm_2 = lvm_import('data_1.lvm',0); % HVC Data for Comparison (towards atm)

data_1 = lvm_1.Segment1.data(:, [1, 7, 8]);
data_2 = lvm_2.Segment1.data(:, [1, 7, 8]);

% For creating data

time = data(1:end,1);
pressure_pvc = data(1:end,2).*0.13332237; % in Pa
pressure_hscm = data(1:end,6).*6894.76; % in Pa

time_1 = data_1(1:end,1);
pressure_ba1 = data_1(1:end,2); % in Pa
pressure_io1 = data_1(1:end,3); % in Pa

time_2 = data_2(1:end,1);
pressure_ba2 = data_2(1:end,2); % in Pa
pressure_io2 = data_2(1:end,3); % in Pa

% For time difference fixing
tlong1 = 1.33464813232421875 + time_1(end); %total in seconds
th1 = tlong1/3600; % in hours
tm1 = (th1 - 1)*60; % in minutes
ts1 = (tm1 - 39)*60; % in seconds
ts1_f = 1.33464813232421875 + ts1;
% Therefore summing the times, the last reading for data was on:
% 11:39:46.4544 am
% First measurement of data1: 11:47:05.8331270217895507812

t2_initial = (47*60) + 05.8331270217895507812; % time in seconds
t1_final = (39*60) + ts1_f;

t2_shift = t2_initial - t1_final; % in seconds

i = 1;

while i <= length(time_2)
    time_comp2(i) = time_2(i) + t2_shift + time_1(end);
    i = i + 1;
end

i = 1;

kba1 = find(pressure_ba1 < 103);
kio1 = find(pressure_io1 < 150);

nba1 = kba1(i);
nio1 = kio1(i);

iba1 = 1;
io1 = 1;

while i <= length(time_1)
    if i <= nba1
        ba_1(iba1) = pressure_ba1(i);
        timeba1(iba1) = time_1(i);
        iba1 = iba1 + 1;
    end
    if i >= nio1
        io_1(io1) = pressure_io1(i);
        timeio1(io1) = time_1(i);
        io1 = io1 + 1;
    end
    i = i + 1;
end

i = 1;
iba2 = 1;

kba2 = find(pressure_ba2 >= 103.689241);
nba2 = kba2(25);

while i <= length(time_comp2)
    if i >= nba2
        ba_2(iba2) = pressure_ba2(i);
        timeba2(iba2) = time_comp2(i);
        iba2 = iba2 + 1;
    end
    i = i + 1;
end

%% Plotting & Display

figure(1)
subplot(2,1,1)
semilogy(time,pressure_pvc,'b-','DisplayName','Pirani (PVC)')
title("Pressure vs. Time in High Vacuum Chamber")
xlabel("Time (s)")
ylabel("Pressure (Pa)")
grid on
hold on
semilogy(time,pressure_hscm,'r-','DisplayName','HSCM')
legend('location','best')

subplot(2,1,2)
semilogy(timeba1,ba_1,'c-','DisplayName','Baratron')
title("Pressure vs. Time in High Vacuum Chamber")
xlabel("Time (s)")
ylabel("Pressure (Pa)")
grid on
hold on
semilogy(timeio1,io_1,'k-','DisplayName','Ion Gage')
semilogy(timeba2,ba_2,'c-','HandleVisibility','off')
legend('location','best')

figure(2)
semilogy(time,pressure_pvc,'b-','DisplayName','Pirani (PVC)')
title("Pressure vs. Time in High Vacuum Chamber")
xlabel("Time (s)")
ylabel("Pressure (Pa)")
grid on
hold on
semilogy(time,pressure_hscm,'r-','DisplayName','HSCM')
semilogy(timeba1,ba_1,'c-','DisplayName','Baratron')
semilogy(timeio1,io_1,'k-','DisplayName','Ion Gage')
semilogy(timeba2,ba_2,'c-','HandleVisibility','off')
legend('location','best')

% fprintf('The Lowest Pressure value for Pirani was %.3f Pa \n', min(pressure_pvc))
% fprintf('The Lowest Pressure value for HSCM was %.3f Pa \n', min(pressure_1))
% fprintf('The Lowest Pressure value for Raspberry Pi was %.3f Pa \n', min(pressure_comp1))
