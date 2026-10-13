close all;
clear;
clc;

%colors = orderedcolors("glow");

Fs = 1000; % Sample Rate (Hz)

ch1 = load('ch1.txt');
ch2 = load('ch2.txt');
ch3 = load('ch3.txt');

%tenS = size(ch1, 1);
twenS = size(ch1, 1); % gets rows of data

% Create time vector for number of lines in .txt file
t0 = 0;
%tf10 = (tenS-1)/Fs;
tf20 = (twenS-1)/Fs;

%t10 = transpose(t0:1/Fs:tf10);
t20 = transpose(t0:1/Fs:tf20);

% Get Columns of Interest
raw_1 = ch1(:,1);
filt_1 = ch1(:,2); 

raw_2 = ch2(:,1);
filt_2 = ch2(:,2); 

raw_3 = ch3(:,1);
filt_3 = ch3(:,2); 

% Plot Raw vs Filtered Data
figure('Name', 'Channel 1');
plot(t20, raw_1);
hold on;
plot(t20, filt_1);
xlabel('Time (s)');
ylabel('Output (V)');
legend('Raw Data', 'Filtered Data');
ylim([-500, 3500]);
grid on;
theme(gcf, "dark")

figure('Name', 'Channel 2');
plot(t20, raw_2);
hold on;
plot(t20, filt_2);
xlabel('Time (s)');
ylabel('Output (V)');
legend('Raw Data', 'Filtered Data');
ylim([-500, 3500]);
grid on;
theme(gcf, "dark")

figure('Name', 'Channel 3');
plot(t20, raw_3);
hold on;
plot(t20, filt_3);
xlabel('Time (s)');
ylabel('Output (V)');
legend('Raw Data', 'Filtered Data');
ylim([-500, 3500]);
grid on;
theme(gcf, "dark")

figure('Name', 'RMS Comparison');
raw1_rms = getRms(raw_1, 200);
raw2_rms = getRms(raw_2, 200);
raw3_rms = getRms(raw_3, 200);
plot(t, raw1_rms, '-', LineWidth= 0.5);
hold on;
plot(t, raw2_rms, '-', LineWidth = 0.5);
plot(t, raw3_rms, '-', LineWidth = 0.5); 
legend('Channel 1', 'Channel 2', 'Channel 3');
xlabel('Time (s)');
ylabel('Output (Vrms)');
grid on;