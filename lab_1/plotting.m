clear; close all;

% create time vector
t = 0:0.1:5;

% create amplitudes
a1 = 1;
a2 = 0.5;

% create frequencies
f1 = 1;
f2 = 10; 

y1 = a1*cos(2*pi*t*f1);

y2 = a2*cos(2*pi*t*f2);

plot(t,y1);
grid on; hold on;
plot(t, y2);
legend on;
