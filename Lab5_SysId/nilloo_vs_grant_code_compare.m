close all;
clear;
clc;

colors = orderedcolors("glow");

Fs = 1000; % Sample Rate (Hz)

ch1 = load('ch1_grantcode_grantsln.txt');
ch2 = load('ch1_grantcode_niloosln.txt');
ch3 = load('ch1_niloocode_grantsln.txt');

tenS = size(ch1, 1);
twenS = size(ch3, 1); % gets rows of data

% Create time vector for number of lines in .txt file
t0 = 0;
tf10 = (tenS-1)/Fs;
tf20 = (twenS-1)/Fs;

t10 = transpose(t0:1/Fs:tf10);
t20 = transpose(t0:1/Fs:tf20);

% Get Columns of Interest
raw_1 = ch1(:,1);
filt_1 = ch1(:,2); 

raw_2 = ch2(:,1);
filt_2 = ch2(:,2); 

raw_3 = ch3(:,1);
filt_3 = ch3(:,2); 

% Plot Raw vs Filtered Data
figure('Name', 'Grant Code & Sln');
plot(t10, raw_1,'Color', colors(1,:));
hold on;
plot(t10, filt_1,'Color', colors(2,:));
xlabel('Time (s)');
ylabel('Output (V)');
legend('Raw Data', 'Filtered Data');
ylim([-500, 3500]);
grid on;
theme(gcf, "dark")

figure('Name', 'Grant Code, Niloo Sln');
plot(t10, raw_2,'Color', colors(1,:));
hold on;
plot(t10, filt_2, 'Color', colors(2,:));
xlabel('Time (s)');
ylabel('Output (V)');
legend('Raw Data', 'Filtered Data');
ylim([-500, 3500]);
grid on;
theme(gcf, "dark")

figure('Name', 'Niloo Code & Sln');
plot(t20, raw_3, 'Color', colors(1,:));
hold on;
plot(t20, filt_3, 'Color', colors(2,:));
xlabel('Time (s)');
ylabel('Output (V)');
legend('Raw Data', 'Filtered Data');
ylim([-500, 3500]);
grid on;
theme(gcf, "dark")

% figure('Name', 'RMS Comparison');
% raw1_rms = getRms(raw_1, 200);
% raw2_rms = getRms(raw_2, 200);
% raw3_rms = getRms(raw_3, 200);
% plot(t, raw1_rms, '-', 'Color', colors(1,:), LineWidth= 0.5);
% hold on;
% plot(t, raw2_rms, '-', 'Color', colors(2,:), LineWidth = 0.5);
% plot(t, raw3_rms, '-', 'Color', colors(5,:), LineWidth = 0.5); 
% legend('Channel 1', 'Channel 2', 'Channel 3');
% xlabel('Time (s)');
% ylabel('Output (Vrms)');
% grid on;
% theme(gcf, "dark")