%% Butterworth Filter
format long;

Fs = 1000; % Hz
Fc = 1; % Hz

Wn = Fc /(Fs/2);
[b, a] = butter(2, Wn, "high")

freqz(b,a, [], Fs);

