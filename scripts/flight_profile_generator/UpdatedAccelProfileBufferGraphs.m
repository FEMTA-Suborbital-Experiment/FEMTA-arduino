%% Setup

%x = rand ----- this makes a random number from 0 - 1
BUFF_SIZE = 15;
accelLength = 2460;
accel = zeros(accelLength, 2);
buff = zeros(BUFF_SIZE, 2); % left column is new buffer and right is older
buff(:,:) = 9.8;
average = zeros(1, 2);
stdDev = zeros(1, 2);

averages1 = zeros(1,accelLength); % holds newer buffer means per each loop
averages2 = zeros(1,accelLength); % holds older buffer means per each loop
newBuffStdDev = zeros(1, accelLength);
oldBuffStdDev = zeros(1,accelLength);
j = 0;
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

%% Buffer Method

for i = 1:accelLength-BUFF_SIZE
    temp = accel(i,2); % Simulating getting new data
    for j = BUFF_SIZE:-1:2
      buff(j,1) = buff(j - 1, 1); % Slide all the buffer points to the right for the newer buffer.
      buff(j,2) = buff(j - 1, 2); % Same as above but with the older buffer
    end
    buff(1,1) = temp; % Set first point of new buffer to newly aquired accelerometer data
    temp = buff(BUFF_SIZE,1);
    buff(1,2) = temp; %Set first value of older buffer to last value of newer buffer (sliding)

    average(1) = sum(buff(:,1)) / BUFF_SIZE;
    average(2) = sum(buff(:,2)) / BUFF_SIZE;

    for j = 1:BUFF_SIZE
      stdDev(1) = stdDev(1) + ((average(1) - buff(j,1)) ^2) / BUFF_SIZE;
      stdDev(2) = stdDev(2) + ((average(2) - buff(j,2)) ^2) / BUFF_SIZE;
    end
     
    stdDev(1) = sqrt(stdDev(1));
    stdDev(2) = sqrt(stdDev(2));

    averages1(i) = average(1);
    averages2(i) = average(2);
    newBuffStdDev(i) = stdDev(1);
    oldBuffStdDev(i) = stdDev(2);
    
    average(:,:) = 0;
    stdDev(:,:) = 0;
end

%% Output

fprintf("\n\nDone!\n\n");

figure;
plot(accel(:,1) ./ 10, accel(:,2), 'k')
xlabel('Time (s)')
ylabel('Acceleration Magnitude (m/s^2)')
title('Suborbital Flight Acceleration Profile')
xline(70, '--g', {'Liftoff Start'}, 'HandleVisibility','off')
xline(200, '--g', {'MECO Start'}, 'HandleVisibility','off')
grid on


% figure;
% hold on
% plot(accel(:,1) ./ 10, newBuffStdDev)
% plot(accel(:,1)./10, oldBuffStdDev)
% plot(accel(:,1) ./ 10, newBuffStdDev - oldBuffStdDev)
% legend('Std Dev of Newer Buffer', 'Std Dev of Older Buffer', 'Difference between StdDev of Newer Buffer and StdDev of Older Buffer')
% xlabel('Time (s)')
% ylabel('Acceleration Magnitude (m/s^2)')
% title('Suborbital Flight Acceleration Profile with Buffer Method Standard Deviations (Std Dev)')
% xline(7, '--g', {'Liftoff Start'}, 'HandleVisibility','off')
% xline(146, '--g', {'MECO Start'}, 'HandleVisibility','off')
% grid on
% hold off

% figure;
% plot(accel(1:accelLength-BUFF_SIZE,1) ./ 10, averages1(1:accelLength-BUFF_SIZE))
% hold on 
% plot (accel(1:accelLength-BUFF_SIZE,1) ./ 10, averages2(1:accelLength-BUFF_SIZE))
% plot (accel(1:accelLength-BUFF_SIZE,1) ./ 10, averages1(1:accelLength-BUFF_SIZE) - averages2(1:accelLength-BUFF_SIZE))
% legend('Mean of Newer Buffer', 'Mean of Older Bufffer', 'Difference of Means')
% xlabel('Time (s)')
% ylabel('Acceleration Magnitude (m/s^2)')
% title('Suborbital Flight Acceleration Profile with Buffer Method Averages')
% xline(7, '--g', {'Liftoff Start'}, 'HandleVisibility','off')
% xline(146, '--g', {'MECO Start'}, 'HandleVisibility','off')
% grid on
% hold off


% figure;
% plot (accel(1:1800,1) ./ 10, newBuffStdDev(1:1800) - oldBuffStdDev(1:1800))
% xlabel('Time (s)')
% ylabel('Acceleration Magnitude (m/s^2)')
% title(['Suborbital Flight Acceleration Profile:' ...
%     ' Difference of Standard Deviations'])
% xline(7, '--g', {'Liftoff Start'}, 'HandleVisibility','off')
% xline(146, '--g', {'MECO Start'}, 'HandleVisibility','off')
% grid on
% hold off

% figure;
% plot (accel(1:1800,1) ./ 10, averages1(1:1800) - averages2(1:1800))
% xlabel('Time (s)')
% ylabel('Acceleration Magnitude (m/s^2)')
% title(['Suborbital Flight Acceleration Profile:' ...
%     ' Difference of Buffer Means'])
% xline(7, '--g', {'Liftoff Start'}, 'HandleVisibility','off')
% xline(146, '--g', {'MECO Start'}, 'HandleVisibility','off')
% grid on
% hold off

writematrix(accel, 'acceleration_liftoff_MECO.csv')