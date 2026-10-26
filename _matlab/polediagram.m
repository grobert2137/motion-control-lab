% Tf

s = tf('s');
sys = (10*(s+1)/((s+10)*(s+10^3)*(s+10^4)));

figure(1) 
pzmap(sys);

figure(2)
bode(sys);

figure(3);
h = pzplot(sys);grid on;

% % 3. Find and customize the markers
% % This finds all line elements on the plot (poles and zeros)
% lines = findobj(gca, 'Type', 'Line');
% 
% % 4. Adjust the sizes
% set(lines, 'MarkerSize', 12, 'LineWidth', 2.5);


%% Test 3D Plot of S-Domain
% 1. Define log-spaced or wide ranges due to the large pole values
sigma = linspace(-15000, 1000, 500); 
omega = linspace(-15000, 15000, 500);

% 2. Create the grid
[Sigma, Omega] = meshgrid(sigma, omega);
s = Sigma + 1i * Omega;

% 3. Element-wise Transfer Function evaluation
H = (10 .* (s + 1)) ./ ((s + 10) .* (s + 10^3) .* (s + 10^4));

% 4. Convert magnitude to Decibels (dB) so the peaks are visible
Z = 20 * log10(abs(H));

% 5. Plot
figure;
surf(Sigma, Omega, Z);
shading interp;
colorbar;
xlabel('\sigma (Real)');
ylabel('\omega (Imaginary)');
zlabel('|H(s)| (dB)');
title('3D S-Domain Log-Magnitude Plot');
grid on;