%% Lab 6 Batch Process

close all;
clear;
clc;

FS = 1000; % Sample Rate (Hz)

folderPath = 'noload_output';

files = dir(folderPath);

for i=1:length(files)
    filename = 
    temp = load(filename);

    
    twenS = size(temp, 1);

    % Create time vector for number of lines in .txt file
    t0 = 0;
    tf20 = (twnS-1)/FS;

    t20 = transpose(t0:1/FS:tf20);

    % Get Columns of Interest
    raw = temp(:,1);
    filt = temp(:,2); 

    % Plot Raw vs Filtered Data
    figure('Name', filename);
    plot(t20, raw);
    hold on;
    plot(t20, filt);
    xlabel('Time (s)');
    ylabel('Output (Counts)');
    legend('Raw Data', 'Filtered Data');
    ylim([-500, 3500]);
    grid on;

    % Get Count AMP
    filt_steadystate = filt(t >= 0.5);
    filt_mean = mean(filt_steadystate);
    filt_minusOfFSet = filt_steadystate - filt_mean;
    crms = sqrt(mean(filt_minusOfFSet.^2));
    camp = sqrt(2) * crms;

    % Gain
    A_in = 1; % 1V 
    gain = camp / A_in;
    dB = 20*log10(camp/A_in);

end
