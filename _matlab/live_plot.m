

PORT = 250000; % UDP_PORT
windowsSec = 5; % seconds of plot shown
fs = 1000; % Sensoray sample rate
nFields = 5;
bytesPerSample = nFields * 8; % 8 bits 

% Open UDP port 
u = udpport("byte", "IPV4", "LocalHost", "127.0.0.1", "LocalPort", port);
cleanupObj = onCleanup(@() delete(u));
flush(u, "input");


% Figure
fig = figure("Name", "Sensoray Motor Plot");
tl = tiledlayout(fig, 2, 1, "TitleSpacing", "compact");

ax1 = nexttile(tl);
raw = plot(ax1, nan, nan, "DisplayName", "raw counts");
hold on;
d(ax1, "on");
filt = plot(ax1, nan, nan, "DisplayName", "filtered counts");

ax2 = nexttile(tl);
cmd = plot(ax2, nan, nan);
ylabel(ax2, "Command (V)");
xlabel(ax2, "Time (s)");
ylim(ax2, [-10 10]);
linkaxes([ax1 ax2], "x");

buffer = zeros(0, nFields);
lastIdx = NaN;
dropped = 0;

while ishandle(fig)
    nSmp = floor(u.NumBytesAvailable / bytesPerSmp);
 
    if nSmp > 0
        raw = read(u, nSmp * nFields, "double");
        new = reshape(raw, nFields, []).';          % one row per sample
 
        idx = new(:, 1);
 
        % C++ program restarted? (index went backwards) -> start fresh
        if ~isnan(lastIdx) && idx(1) <= lastIdx
            buf = zeros(0, nFields);
            lastIdx = NaN;
            dropped = 0;
        end
 
        % dropped-sample check using the sample index
        if isnan(lastIdx), lastIdx = idx(1) - 1; end
        dropped = dropped + sum(diff([lastIdx; idx]) - 1);
        lastIdx = idx(end);
 
        % append and keep a rolling window
        buf = [buf; new];                           %#ok<AGROW>
        if size(buf, 1) > maxSamples
            buf = buf(end - maxSamples + 1 : end, :);
        end
 
        t = buf(:, 2);
        set(hRaw,  "XData", t, "YData", buf(:, 4));
        set(hFilt, "XData", t, "YData", buf(:, 5));
        set(hCmd,  "XData", t, "YData", buf(:, 3));
        xlim(ax1, [max(0, t(end) - windowSec), max(windowSec, t(end))]);
        title(ax1, sprintf("Dropped samples: %d", dropped));
 
        drawnow limitrate
    else
        pause(0.01);   % nothing new; don't spin the CPU
    end
end