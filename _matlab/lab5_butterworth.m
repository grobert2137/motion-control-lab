%% Butterworth Filter
close all;
clc; clear;

format long;
N = 2; % Filter order 
Fs = 1000; % Hz
Fc = 1; % Hz

Wn = Fc /(Fs/2);
[b, a] = butter(N, Wn, "high")
[h, w] = freqz(b, a, 6000); % output frequency respose vector and angular frequency vector

figure("Name", "Filter frequency response");

% Manual bode plot magnitude
subplot(2, 1, 1);
semilogx(w, 20*log10(abs(h))); % plot bode plot
str = sprintf('Highpass filter, generalized frequency response. Fc=%4.2f Hz, Order N=%d', Fc, N);
title(str);
ylabel('Relative Magnitude of Output (dB)');
xlabel('Frequency (Radians per Sample)');
grid on;

subplot(2, 1, 2);
semilogx(Fs*w/(2*pi), 20*log10(abs(h)));
str = sprintf('Specific for Fsample =%d Hz', Fs);
title(str);
xlabel('Frequency (Hz)');
grid on;

% Calculate gain at Wc
wc = 2*pi*Fc/Fs;
gain_c = freqz(b, a, [wc wc]);
gain_c_db = 20*log10(abs(gain_c(1)));
