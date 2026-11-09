%% LAB 6 BATCH PROCESS

close all; clear; clc;

%%----------------------------- SETTINGS ----------------------------------

FS = 1000; % Sample Rate (Hz)
plantNames = ["Unloaded", "Loaded"];
folderNames = ["noload_output", "loaded_output"];

varNames = ["Loaded", "ampV", "freqHz", "w", "crms", "camp", "gainDB"];
varTypes = ["logical", "double", "double", "double", "double", "double", "double"];

T = table('Size', [0, 7], 'VariableTypes', varTypes,'VariableNames',varNames);

%% Question 1 

%%-----Add Data to Table for Each File --------

row = 0;
for p = 1:numel(folderNames)
    files = dir(fullfile(folderNames(p), "*.txt")); % Load only files that are .txt
    for k = 1:numel(files)
        
        % LOAD INTO MATRIX
        path = fullfile(files(k).folder, files(k).name);
        M = readmatrix(path);

        % Create time vector for number of lines in .txt file
        t = (0:size(M,1)-1)' / FS;

        % Column of interest
        filt = M(:, 2);
        
        %figure();
        %plot(t, filt);

        % Get Count AMP
        filt_ss = filt(t >= 1.5); % get steady input only
        filt_minusOffset = filt_ss - mean(filt_ss);
        crms = sqrt(mean(filt_minusOffset.^2));
        camp = sqrt(2) * crms;

        % Get amplitude & frequency from file name
        [~, name] = fileparts(files(k).name);
        ampV = str2double(extractBefore(name, '_')) / 10 ;
        freqHz = str2double(extractAfter(name, '_')) / 10;   
        w = 2*pi*freqHz;
        gainDB = 20*log10(camp/ampV);
    
        row = row + 1;
        T(row, :) = {plantNames(p) == "Loaded", ampV, freqHz, w, crms, camp, gainDB};
    end
end

% Seperate Data 
T = sortrows(T, {'Loaded',  'ampV', 'freqHz'});
Tu = T(~T.Loaded, :);     % unloaded rows
Tl = T( T.Loaded, :);     % loaded rows

% Plot
plotScatter(Tu, "Unloaded - Raw Data");
plotScatter(Tl, "Loaded - Raw Data");

% Display table of info
disp(Tu);
disp(Tl);

%% Question 2

% Data with asymptotes
plotScatter(Tu, "Unloaded - Asymptotes");
resU = findKm(Tu, "Unloaded", 0.5, [0 5], [30 65], [1 3], [1 3]);

plotScatter(Tl, "Loaded - Asymptotes");
resL = findKm(Tl, "Loaded", 0.5, [0 5], [15 35], [1 3], [1 3]);

results = [resU; resL];

% Display info from amplitude
disp(results)

Tu_1V = Tu(Tu.ampV==1, :);    
Tl_1V = Tl(Tl.ampV==1, :);

plot2PlantScatter(Tu_1V, Tl_1V, "1V Combined- Raw Data")

%% Helper Functions
function plotScatter(Tp, plant)
    figure('Name', plant, 'Color', 'w');
    hold on; grid on; box on;
    set(gca, 'XScale', 'log', 'FontSize', 12, 'LineWidth', 1);
    grid minor   
    colors = [0 0.447 0.741; 0.850 0.325 0.098];
    shapes = ["o", "s"];

    amps = unique(Tp.ampV);
    for a = 1:numel(amps)
        sel = Tp.ampV == amps(a);
        plot(Tp.w(sel), Tp.gainDB(sel), shapes(a), ...
            'MarkerSize', 6, ...
            'LineWidth', 1.5, ...
            'MarkerEdgeColor', colors(a,:), ...
            'MarkerFaceColor', colors(a,:), ...
            'DisplayName', sprintf('%.1f V input', amps(a)));
    end

    xlabel('\omega (rad/s)');
    ylabel('20 log_{10}(C_{amp}/V_{in}) (dB)');
    title(plant);
    legend('Location', 'southwest');
end

function plot2PlantScatter(Tp1, Tp2, name)
    figure('Name', name, 'Color', 'w');
    hold on; grid on; box on;
    set(gca, 'XScale', 'log', 'FontSize', 12, 'LineWidth', 1);
    grid minor   
    colors = [0 0.447 0.741; 0.850 0.325 0.098];
    shapes = ["o", "s"];

    plot(Tp1.w, Tp1.gainDB, shapes(1), ...
        'MarkerSize', 6, ...
        'LineWidth', 1.5, ...
        'MarkerEdgeColor', colors(1,:), ...
        'MarkerFaceColor', colors(1,:), ...
        'DisplayName', "Unloaded 1 V input");

    plot(Tp2.w, Tp2.gainDB, shapes(2), ...
        'MarkerSize', 6, ...
        'LineWidth', 1.5, ...
        'MarkerEdgeColor', colors(2,:), ...
        'MarkerFaceColor', colors(2,:), ...
        'DisplayName', 'Loaded 1 V input');

    xlabel('\omega (rad/s)');
    ylabel('20 log_{10}(C_{amp}/V_{in}) (dB)');
    title("Combined Data");
    legend('Location', 'southwest');
end

function results = findKm(Tp, plant, fitAmp, wLowIdx, wHighIdx, xLowPlot, xHighPlot)
    % Generates table of slope and intercept for each asymptote
    % plots asymptotes on top of current figure

    F = Tp(Tp.ampV == fitAmp, :);
    x = log10(F.w);

    % Get the input ranges
    lowSel  = F.w >= (2*pi*wLowIdx(1)) & F.w <= (2*pi*wLowIdx(2));
    highSel = F.w >= (2*pi*wHighIdx(1)) & F.w <= (2*pi*wHighIdx(2));

    % Line fitting toolbox
    pLow  = polyfit(x(lowSel), F.gainDB(lowSel),  1);
    pHigh = polyfit(x(highSel), F.gainDB(highSel), 1);

    %frequency and KM
    xc  = (pHigh(2) - pLow(2)) / (pLow(1) - pHigh(1));
    wc  = 10^xc;
    Km  = 10^(polyval(pLow, 0)/20);       % low asymptote at w = 1 rad/s

    % Draw each line over its own range
    xxLow  = linspace(xLowPlot(1), xLowPlot(2),  200);
    xxHigh = linspace(xHighPlot(1), xHighPlot(2), 200);
    plot(10.^xxLow,  polyval(pLow,  xxLow),  'b--', 'LineWidth', 1.5, 'DisplayName', 'Low asymptote');
    plot(10.^xxHigh, polyval(pHigh, xxHigh), 'k--', 'LineWidth', 1.5, 'DisplayName', 'High asymptote');

    results = table(plant, pLow(1), pLow(2), pHigh(1), pHigh(2), wc, Km, 'VariableNames', ["plant","lowFreqSlope", "lowFreqIntercept","highFreqSlope", "highFreqIntercept","wc","Km"]);
end

    
