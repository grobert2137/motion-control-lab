%% Lab 5 Post-Lab
close all; clc; clear;

% Load Encoder Output File
ch1 = load('50_20_grant.txt');

twenS = size(ch1, 1); % gets rows of data

% Create time vector for number of lines in .txt file
Fs = 1000; % Sample Rate (Hz)
t0 = 0;
tf20 = (twenS-1)/Fs;

t20 = transpose(t0:1/Fs:tf20);

% Get Columns of Interest
raw_1 = ch1(:,1);
filt_1 = ch1(:,2); 

% Plot Raw vs Filtered Data
figure('Name', 'Raw Vs Filtered');
plot(t20, raw_1);
hold on;
plot(t20, filt_1);
title('Encoder Output to 5Hz 1V Amplitude Sine Wave Motor Input')
xlabel('Time (s)');
ylabel('Output (Counts)');
legend('Raw Data', 'Filtered Data');
ylim([-500, 3500]);
grid on;

% Seems to be steady after 0.5s, so filter those first 0.5 seconds out
filt_steadystate = filt_1(t20 >= 1);

% Compute Count Amplitude
filt_mean = mean(filt_steadystate);
filt_minusOffset = filt_steadystate - filt_mean;
count_rms = sqrt(mean(filt_minusOffset.^2))
count_amp = sqrt(2) * count_rms

% Ratio of input amplitude to output amplitude
A_in = 1; % 1V 
PPR = 1000; % Pulse per revolution of encoder
ratio = (count_amp) / A_in

% Question 4 Ans
dB = 20*log10(ratio)


%% Frequency Response of a Moving Average Filter
clc; close all;
figure(1) = fvtool([1/6 1/6 1/6 1/6 1/6 1/6],1);




%% In Class Derived TF

clc; close all; clear;

% Create TF 
s = tf("s");
sys = (10^9*(s+1))/((s+10)*(s+10^3)*(s+10^4));

[mag, phase, wout] = bode(sys);


% Convert to 3D Array to 1D Array
mag_db = 20 * log10(squeeze(mag));
phase_deg = squeeze(phase);
freq = squeeze(wout);

figure('Name', 'MATLAB Magnitude');
subplot(2,1,1);
semilogx(freq, mag_db, 'LineWidth', 1.5);
grid on;
title('MATLAB Bode Plot ');
subtitle("10^9*(s+1)/((s+10)*(s+10^3)*(s+10^4))");
ylabel('Magnitude (dB)');
ylim([-40, 60]);

subplot(2,1,2);
x_points = [1e-1, 1, 10, 1e3, 1e4, 1e5];
y_points = [20, 20, 40, 40, 20, -20];

semilogx(x_points, y_points, 'o-');

title('Asymptotic Plot From Class')
ylim([-40, 60]);
xlim([1e-2, 1e6]);
grid on;