
%% ------------------------------------------------- INFO -------------------------------------------------
close all; clc; clear;

%{

P Control (Plant 1)   = 0
PI Control (Plant 1)  = 1
PID Control (Plant 1) = 2

P Control (Plant 2)   = 3
PI Control (Plant 2)  = 4
PID Control (Plant 2) = 5

%}

%% ------------------------------------------------- Motor Transfer Function -------------------------------------------------

% Km and Tm 
Km = [11661, 37162];
Tm = [0.017, 0.027];

% Systems
s = tf('s');
plant1_p = (0.004*Km(1)/((Tm(1)*s^2)+ s +(0.004*Km(2))));
plant2_p = (0.004*Km(2)/((Tm(2)*s^2)+ s +(0.004*Km(2))));

% Bode plots
figure('Name', 'Plant1 Bode Plot');
[mag, ~, wout] = bode(plant1_p);
mag = squeeze(mag);
mag_dB = 20 * log10(mag);

semilogx(wout, mag_dB, 'b-', 'LineWidth', 1.5);
grid on;
title('Plant 1 Bode Plot');
xlabel('Frequency (rad/s)');
ylabel('Magnitude (dB)');


figure('Name', 'Plant2 Bode Plot');
[mag, ~, wout] = bode(plant2_p);
mag = squeeze(mag);
mag_dB = 20 * log10(mag);

semilogx(wout, mag_dB, 'b-', 'LineWidth', 1.5);
grid on;
title('Plant 2 Bode Plot');
xlabel('Frequency (rad/s)');
ylabel('Magnitude (dB)');


% K Values
Kp = [ 0.005,  0.0045,  0.006,    0.0045,  0.00405, 0.0054    ];
Ki = [ 0.000,  0.0674,  0.15,     0.000,   0.0261,  0.058     ];
Kd = [ 0.000,  0.000,   0.00006,  0.000,   0.000,   0.0001255 ];


%% ------------------------------------------------- Folder Loading -------------------------------------------------

files = dir(fullfile("data", "*.txt")); % Load only files that are .txt
fprintf("\nFound %d files in folder\n\n", length(files));

for k = 1:length(files)
    disp(files(k).name);
end

%% ------------------------------------------------- Data Sorting -------------------------------------------------
s = struct();

for i = 1:length(files)
    name = files(i).name;
    name = erase(name, ".txt");
    data = load(fullfile(files(i).folder, files(i).name));
    if size(data, 2) > 1
        data = data(:, 3); % Get sineusoidal samples
    else 
        data = data(:, 1);
    end
    s.(name).data = data;

    % Creat time vector based on data length
    time = transpose(0:length(s.(name).data)-1); % 
    s.(name).time = time; % Time vector
end

fns = fieldnames(s);

%% ---------------------------------------------- EXERCISE 1: Finding Bandwidth -------------------------------------------------

varNames = ["Name", "Freq_hz", "w", "Kp", "C_rms", "C_amp", "Mag", "gainDb"];
varTypes = ["string", "double", "double", "double", "double", "double", "double", "double"];

T = table('Size', [0, 8], 'VariableTypes', varTypes,'VariableNames',varNames);

fns_ex1 = fns(startsWith(fns, 'Exercise1_'));

% Constants for all data
campIn = 250;
kp = 0.004;

row = 0;
for j = 1:length(fns_ex1)
    % Get name for table
    name = fns_ex1{j};

    % Data & Time 
    data = s.(fns_ex1{j}).data;
    time = s.(fns_ex1{j}).time; % time in ms

    % Get frequencies (Hz and rad/s)
    freqHz = str2double(extractAfter(erase(fns_ex1{j}, "Hz"), '_'));
    w = freqHz * 2 * pi;

    % Filter steady-state and find Crms and Camp
    data_ss = data(time >= 200); % get steady input only
    data_minusOffset = data_ss - mean(data_ss);
    
    crms = sqrt(mean(data_minusOffset.^2));
    campOut = sqrt(2) * crms;
    mag = campOut/campIn;
    gainDB = 20*log10(mag);

    % Add to table
    row = row + 1;
    T(row, :) = {name, freqHz, w, kp, crms, campOut, mag, gainDB};
end

% Sort by Frequency (Hz)
T = sortrows(T, {'Freq_hz'});
disp(T);
figure('Name', 'Plant 1: Bode Plot')
semilogx(T.w, T.gainDb, 'o-', 'LineWidth', 1.5);
hold on;
semilogx(wout, mag_dB, '-', 'LineWidth', 1.5);
grid on;
xlim([20 140]);
legend('Experimental', 'Theoretical', 'Location', 'southwest');

figure('Name', 'ZOOMED Plant 1: Bode Plot')
semilogx(T.w, T.gainDb, 'o-', 'LineWidth', 1.5);
hold on;
semilogx(wout, mag_dB, '-', 'LineWidth', 1.5);
grid on;
xlim([20 140]);
ylim([-12 8]);

xlabel('Frequency (rad/s)');
ylabel('Magnitude (dB)');
title('Plant 1 Closed-Loop Bode Plot: Theoretical vs Experimental');
subtitle('Kp = 0.004')
yline(-3,'--k','-3 dB');
legend('Experimental', 'Theoretical', '-3 dB Line', 'Location', 'southwest');

%% ---------------------------------------------- EXERCISE 2: Plots -------------------------------------------------

% Get only controller tests from struct
fns_ex2 = fns(startsWith(fns, 'ControllerTest'));

for n = 1:length(fns_ex2)
    type = "P";
    if Ki(n) ~= 0, type = type + "I"; end
    if Kd(n) ~= 0, type = type + "D"; end

    if n >= 4
        name = "Plant 2: " + type;
    else 
        name = "Plant 1: " + type;
    end

    % Data & Time 
    data = s.(fns_ex2{n}).data;
    time = s.(fns_ex2{n}).time; % time in ms

    data_sim = s.(fns_ex2{n}).data;

    figure('Name', name);
    plot(time, data, 'LineWidth', 1.5);
    hold on;
    plot(time, data_sim, 'LineWidth', 1.5);
    ylim([0 500]);
    xlim([0 1500]);
    grid on;
    xlabel('\omega (rad/s)');
    ylabel('Counts');
    title(name + " Controller");
    subtitle("Kp = " + Kp(n) + "  Ki = " + Ki(n) + "  Kp = " + Kd(n))
    %legend('Experimental Data', 'Simulation Data', 'Location', 'southwest');
end


type = "Combined";

name = "Plant 1: " + type;
figure('Name', name);

for m = 4:6
    data = s.(fns_ex2{m}).data;
    time = s.(fns_ex2{m}).time; % time in ms
    plot(time, data, 'LineWidth', 1.0);
    hold on;
    xlabel('\omega (rad/s)');
    ylabel('Counts');
end
xlim([0 1500]);
ylim([0 500]);
grid on;
legend('P','PI', 'PID')
    
name = "Plant 2: " + type;
figure('Name', name);

grid on;
for k = 1:3
    data = s.(fns_ex2{k}).data;
    time = s.(fns_ex2{k}).time; % time in ms
    plot(time, data, 'LineWidth', 1.0);
    hold on;
    
    xlabel('\omega (rad/s)');
    ylabel('Counts');
end
xlim([0 500]);
ylim([0 500]);
grid on;
legend('P','PI', 'PID')