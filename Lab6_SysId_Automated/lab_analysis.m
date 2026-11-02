%% Lab 6 Batch Process

close all;
clear;
clc;

%%---------------------------- Settings ----------------------------------
FS = 1000; % Sample Rate (Hz)

folderPath = {'noload_output'; 'loaded_output'};

% Create struct of data
data = struct();
data.free = struct('crms', [], 'camp', [], 'freq', [], 'amp', [], 'gain', []);
data.loaded = struct('crms', [], 'camp', [], 'freq', [], 'amp', [], 'gain', []);

names= fieldnames(data);

for j=1:2

    files = dir(fullfile(folderPath{j}, "*.txt")); % Load only files that are .txt

    for i=1:length(files)
        
        tempName = files(i).name;
        tempPath = fullfile(pwd, folderPath{j}, tempName);
    
        % Get amplitude & frequency from file name
        [~, name] = fileparts(tempName);
        amp_str = extractBefore(name, '_');
        freqHz_str = extractAfter(name, '_');
        
        % Convert strings to doubles
        amp = str2double(amp_str);
        freqHz = str2double(freqHz_str);
    
        % Place amp and freq in struct
        data.(names{j}).amp(i) = amp;
        data.(names{j}).freq(i) = 2*pi*freqHz;
    
        % Get matrix of data
        M = readmatrix(tempPath);
    
        % Get count RMS
        % Create time vector for number of lines in .txt file
        t0 = 0;
        tf = size(M(:, 1)-1, 1)/FS - (1/FS);
        t = transpose(t0:1/FS:tf);
    
        % Get Column of Interest
        filt = M(:,2); 
    
        % Get Count AMP
        filt_ss = filt(t >= 0.5); % get steady input only
        filt_minusOffset = filt_ss - mean(filt_ss);
        crms = sqrt(mean(filt_minusOffset.^2));
        camp = sqrt(2) * crms;
    
        data.(names{j}).crms(i) = crms;
        data.(names{j}).camp(i) = camp;
    
        data.(names{j}).gain(i) = 20*log10(camp/amp);
    end
end


%% Unloaded Plotting

figure('Name', 'Unloaded');


scatter(data.free.freq(1:16), data.free.gain(1:16), 'ro');
hold on;
scatter(data.free.freq(17:32), data.free.gain(17:32), 'gx');


gx = gca;
gx.XScale = 'log';

grid on;

% get asymptotes

w = data.free.freq(17:32)';
magdB = data.free.gain(17:32)';
x = log10(w);

lowIdx = w < 1200;
highIdx = w > 1200 & w <5000;

pLow = polyfit(x(lowIdx), magdB(lowIdx), 1); %linear fit
pHigh = polyfit(x(highIdx), magdB(highIdx), 1); %linear fit

xc = (pHigh(2) - pLow(2)) / (pLow(1) - pHigh(1));
wc = 10^xc

xx = linspace(min(x), max(x), 200);

semilogx(10.^xx, polyval(pLow,  xx), 'b--', 'LineWidth', 1.5, 'DisplayName', 'Low asymptote'); 
semilogx(10.^xx, polyval(pHigh, xx), 'k--', 'LineWidth', 1.5, 'DisplayName', 'High asymptote');
xline(wc, ':', sprintf('\\omega_c = %.0f rad/s', wc), 'HandleVisibility', 'off');
xlabel('\omega (rad/s)'); ylabel('Gain (dB)'); legend show

% Get Km
z = (1/20)*(pLow(1)*1 + pLow(2));
Km_unloaded = 10^z;
fprintf("Km Unloaded is %.1f\n", Km_unloaded)

%% Loaded Plotting
figure('Name', 'Loaded');



scatter(data.loaded.freq(1:16), data.loaded.gain(1:16), 'ro');
hold on;
scatter(data.loaded.freq(17:32), data.loaded.gain(17:32), 'gx');

gx = gca;
gx.XScale = 'log';

grid on;

% get asymptotes

w = data.loaded.freq(17:32)';
magdB = data.loaded.gain(17:32)';
x = log10(w);

lowIdx = w < 1200;
highIdx = w > 1200 & w <5000;

pLow = polyfit(x(lowIdx), magdB(lowIdx), 1); %linear fit
pHigh = polyfit(x(highIdx), magdB(highIdx), 1); %linear fit

xc = (pHigh(2) - pLow(2)) / (pLow(1) - pHigh(1));
wc = 10^xc

xx = linspace(min(x), max(x), 200);

semilogx(10.^xx, polyval(pLow,  xx), 'b--', 'LineWidth', 1.5, 'DisplayName', 'Low asymptote'); 
semilogx(10.^xx, polyval(pHigh, xx), 'k--', 'LineWidth', 1.5, 'DisplayName', 'High asymptote');
xline(wc, ':', sprintf('\\omega_c = %.0f rad/s', wc), 'HandleVisibility', 'off');
xlabel('\omega (rad/s)'); ylabel('Gain (dB)'); legend show

% Get Km
z = (1/20)*(pLow(1)*1 + pLow(2));
Km_unloaded = 10^z;
fprintf("Km loaded is %.1f\n", Km_unloaded)