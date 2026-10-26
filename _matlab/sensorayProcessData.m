%function sensorayProcessData(filePath)

Fs = 1000; % Sample Rate (Hz)
filePath = '50_10_grant.txt';

data = load(filePath);
numLines = size(data, 1); % gets rows of data

% Create time vector for number of lines in .txt file
t0 = 0;
tf = (numLines-1)/Fs;
t = transpose(t0:1/Fs:tf);

% Get Columns of Interest
raw = data(:,1);
filt = data(:,2); 

% Plot Raw vs Filtered Data
figure('Name', 'Sensoray Data');
subplot(1,1,1);
plot(t, raw);
hold on;
%plot(t, filt);
xlabel('Time (s)');
ylabel('Output (V)');
legend('Raw Data', 'Filtered Data');
grid on;

% Seems to be steady after 0.5s, so filter those first 0.5 seconds out
filt_steadystate = filt(t >= 0.5);

% Compute CAMP
filt_mean = mean(filt_steadystate);
filt_minusOffset = filt_steadystate - filt_mean;
crms = sqrt(mean(filt_minusOffset.^2));
camp = sqrt(2) * crms;

% Gain
A_in = 1; % 1V 
gain = camp / A_in;

% Question 4 Ans
ans = 20*log10(camp/A_in);
