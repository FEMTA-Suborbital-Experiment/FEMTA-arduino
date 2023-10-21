%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% AUTHOR: JACOB VALDEZ
% FEMTA Suborbital Experiment
% Buffer Size versus Total Write Time Optimum
% Plot a graph of buffer size versus write time to identify a conservative
% buffer size from a best-fit, as well as buffer size versus percentage of
% the time spent writing to the SD card.
%
% When interpreting the results from the graph, the percent downtime is a
% part of the experiment spent writing to the SD card. We want to minimize
% the time spent writing to the buffer, while maximizing the number of data
% points preserved from the buffer size.
% 
% From the results, a buffer size of 50 is an optimum which maximizes data
% points while minimizing the amount of time spent writing to the SD card.
% We achieved a percentage of about 1% for downlinking for the entire
% experiment
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

clear; clc; close all;

%% INITIALIZATIONS

bufferTime = readmatrix("bufferTimeData.txt");

buffers = bufferTime(:,1);
writeTimes = bufferTime(:,2); % Write time per buffer size
poll_frequency = 1/10; % Hz

%% CALCULATIONS

% New Shepard Payload User Guide Page 13
experiment_time = 346 - 181; % seconds

% Total number of data points for entire experiment duration
total_data = experiment_time ./ poll_frequency; 

% Total number of writes 
num_of_writes = total_data ./ buffers;

% Total write time per buffer
total_write_time = num_of_writes .* writeTimes .* 1e-6;


%% PLOTTING

bufferRange = linspace(0, buffers(end), 2^12);
p = polyfit(buffers, writeTimes, 2);

y = polyval(p, buffers);

Rsq = 1 - sum((writeTimes - y).^2)/sum((writeTimes - mean(writeTimes)).^2);

y = polyval(p, bufferRange);

% Plot buffer size versus write time per buffer size
figure;
scatter(bufferTime(:, 1), bufferTime(:, 2) / 1000);
title("Write Time as a Function of Buffer Size");
xlabel("Buffer Size");
ylabel("Write Time (ms)")
hold on
plot(bufferRange, y / 1000);
grid on
legend("Data", strcat("Quadratic Fit: R^2 = ", num2str(Rsq)));

% Plot buffer size versus total time spent writing to the SD card
figure;
semilogy(buffers, total_write_time / experiment_time * 100, 'o--');
xlabel("Buffer Size")
ylabel("Percent Downtime")
grid on
title("Percentage downtime as function of Buffer Size")