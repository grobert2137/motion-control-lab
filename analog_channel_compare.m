close all;
clear;
clc;

colors = orderedcolors("glow");

Fs = 1000; % Sample Rate (Hz)

ch1 = load('ch1.txt');
ch2 = load('ch2.txt');
ch3 = load('ch3.txt');

numLines = size(ch1, 1); % gets rows of data

% Create time vector for number of lines in .txt file
t0 = 0;
tf = (numLines-1)/Fs;
t = transpose(t0:1/Fs:tf);

% Get Columns of Interest
raw_1 = ch1(:,1);
filt_1 = ch1(:,2); 

raw_2 = ch2(:,1);
filt_2 = ch2(:,2); 

raw_3 = ch3(:,1);
filt_3 = ch3(:,2); 

% Plot Raw vs Filtered Data
figure('Name', 'Channel 1');
plot(t, raw_1,'Color', colors(1,:));
hold on;
plot(t, filt_1,'Color', colors(2,:));
xlabel('Time (s)');
ylabel('Output (V)');
legend('Raw Data', 'Filtered Data');
grid on;
theme(gcf, "dark")

figure('Name', 'Channel 2');
plot(t, raw_2,'Color', colors(1,:));
hold on;
plot(t, filt_2, 'Color', colors(2,:));
xlabel('Time (s)');
ylabel('Output (V)');
legend('Raw Data', 'Filtered Data');
grid on;
theme(gcf, "dark")

figure('Name', 'Channel 3');
plot(t, raw_3, 'Color', colors(1,:));
hold on;
plot(t, filt_3, 'Color', colors(2,:));
xlabel('Time (s)');
ylabel('Output (V)');
legend('Raw Data', 'Filtered Data');
grid on;
theme(gcf, "dark")

figure('Name', 'RMS Comparison');
raw1_rms = getRms(raw_1, 200);
raw2_rms = getRms(raw_2, 200);
raw3_rms = getRms(raw_3, 200);
plot(t, raw1_rms, '-', 'Color', colors(1,:), LineWidth= 0.5);
hold on;
plot(t, raw2_rms, '-', 'Color', colors(2,:), LineWidth = 0.5);
plot(t, raw3_rms, '-', 'Color', colors(5,:), LineWidth = 0.5); 
legend('Channel 1', 'Channel 2', 'Channel 3');
xlabel('Time (s)');
ylabel('Output (Vrms)');
grid on;
theme(gcf, "dark")