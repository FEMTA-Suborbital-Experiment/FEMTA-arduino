% VIP 37920 Fall 2023
% Flow Test - Plot Function
% Authors: Gabriel Oliveira and Jacob Valdez
%% Function Call
% flowmeter(n), being n the trial number
% Plots Data and prints Air In Line times and Flow Average

% Comments on numbers for each trial:
% 1,2 --> Verification Runs 1&2
% 3: Trial Run 1
% 4: Trial Run 2
% 5: Trial Run 3
% 6: Trial Run 4

%% Initialization

function flowmeter(n)

if n == 1
    data = readmatrix('flow-verify.csv');
elseif n == 2
    data = readmatrix('flow-verify2.csv');
elseif n == 3
    data = readmatrix('Flow_Slow_Trial1.txt');
elseif n == 4
    data = readmatrix('Flow_Fast_Trial2.txt');
elseif n == 5
    data = readmatrix('Flow_Faster_Trial3.txt');
else
    data = readmatrix('Flow_Atmos_Trial4.txt');
end

% For creating data

time = data(1:end,1);
flow = data(1:end,2); % in ml/min
temp = data(1:end,3);
AIL = data(1:end,4); % 1 for AIL and 0 for no AIL
HF = data(1:end,5);

i = 1;
index = 1;

nonzero_flows = nonzeros(flow);
avg_flow_temp = mean(nonzero_flows); % temporary variable for avg flow

while i <= length(time)
    mat_avg_flow(i) = avg_flow_temp;
    i = i + 1;
end

avg_flow = transpose(mat_avg_flow);

i = 1;

fprintf('Flow Test %d \n',n)
while i ~= length(time)
    if AIL(i) == 1
        fprintf("Air in Line Detected at Time: %.3f s \n", time(i))
        k(index) = i;
        index = index + 1;
    end
    i = i + 1;
end
fprintf('\n')

index = 1;

i = 1;

while i ~= length(time)
    if HF(i) == 1
        h(index) = i;
        index = index + 1;
    end
    i = i + 1;
end

index = 1;
%% Plotting & Display

figure(n)
if n == 1
    sgtitle('Verification Run 1');
elseif n == 2
    sgtitle('Verification Run 2');
elseif n == 3
    sgtitle('Trial 1 - Slow Pressing');
elseif n == 4
    sgtitle('Trial 2 - Fast Pressing');
elseif n == 5
    sgtitle('Trial 3 - Faster Pressing');
else
    sgtitle('Trial 4 - Atmospheric');
end
subplot(2,1,1)
plot(time,flow,'b-','DisplayName','Flowrate')
title("Flow vs. Time")
xlabel("Time (s)")
ylabel("Flowrate (ml/min)")
grid on
hold on
plot(time,avg_flow,'c-','DisplayName','Average Flowrate')
if length(find(AIL)) >= 1
    while index ~= length(k)
        plot(time(k(index)),flow(k(index)),'r*','HandleVisibility','off');
        index = index + 1;
    end
    plot(time(k(length(k))),flow(k(length(k))),'r*','DisplayName','Air In Line Detected')
end

index = 1;

if length(find(HF)) >= 1
    while index ~= length(h)
        plot(time(h(index)),flow(h(index)),'g*','HandleVisibility','off');
        index = index + 1;
    end
    plot(time(h(length(h))),flow(h(length(h))),'g*','DisplayName','High Flow in Line Detected')
end
legend('location','best')
hold off

subplot(2,1,2)
plot(time,temp,'r-')
ylim([20 30])
title("Temperature vs. Time")
xlabel("Time (s)")
ylabel("Temperature (°C)")
grid on
hold on

% Printing all flow averages

if n == 1
    fprintf('Verify 1: The flow average was %.3f ml/min \n', avg_flow_temp)
elseif n == 2
    fprintf('Verify 2: The flow average was %.3f ml/min \n', avg_flow_temp)
elseif n == 3
    fprintf('Trial 1: The flow average was %.3f ml/min \n', avg_flow_temp)
elseif n == 4
    fprintf('Trial 2: The flow average was %.3f ml/min \n', avg_flow_temp)
elseif n == 5
    fprintf('Trial 3: The flow average was %.3f ml/min \n', avg_flow_temp)
else
    fprintf('Trial 4: The flow average was %.3f ml/min \n', avg_flow_temp)
end

fprintf('\n')
end
