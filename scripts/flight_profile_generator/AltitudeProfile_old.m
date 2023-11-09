%column 1 is time (seconds) and column 2 is altitude (1,000 feet)
clear; clc; close all;

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
%liftoff at 7 seconds - index 4
%meco at 146 seconds - index 17
%peak at 246 seconds - index 23
%land at 430 seconds - index 40 
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

plot(profile(:,1),profile(:,2), 'o')
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

alt = alt.*1000;
alt = alt./3.281;
[~, ~, pressure, ~] = atmoscoesa(alt);

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

%writematrix(time, 'time.csv');
%writematrix(alt, 'altitude.csv');
%writematrix(pressure, 'pressure.csv');

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