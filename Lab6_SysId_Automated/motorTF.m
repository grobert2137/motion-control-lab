%% Motor Transfer Function
clc; clear; close all;

% Tf
Km = 11661;
Tm = 0.017;

s = tf('s');
sys = (Km/(s*(Tm*s+1)));

figure('Name', 'Unloaded: Bode Plot')
bode(sys);
grid on;

figure('Name', 'Unloaded: Pole & Zero Plot') 
h = pzplot(sys); 
grid on;


