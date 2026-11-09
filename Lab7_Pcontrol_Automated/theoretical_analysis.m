%% Theoretical step response

close all; clc; clear;

% Constant Kp Values
KP_8 = 0.008;
KP_4 = 0.004;
KP_1 = 0.001;

plants = struct("Unweighted", [], "Weighted", []);


% Km and Tm for each plant
plants.Unweighted.Km = 11661;
plants.Unweighted.Tm =  0.017;

plants.Weighted.Km = 37162;
plants.Weighted.Tm =  0.027;

names = fieldnames(plants);



for k=1:length(names)
    km_temp = plants.(names{k}).Km;
    tm_temp = plants.(names{k}).Tm;
    KP_critical = (1/(4*km_temp*tm_temp));
    plants.(names{k}).kp = {KP_8, KP_4, KP_1, KP_critical};
end

varNames = ["Loaded", "Km", "Tm", "Kp", "Pole 1", "Pole 2", "roots(1)", "roots(2)"];
varTypes = ["logical", "double", "double", "double", "double", "double", "double", "double"];

T = table('Size', [0, 8], 'VariableTypes', varTypes,'VariableNames',varNames);
row = 0;
for j = 1:length(names)
    Km = plants.(names{j}).Km;
    Tm = plants.(names{j}).Tm;
    Kp = plants.(names{j}).kp;
    for i = 1:length(Kp)
        wn_squared = (Km*Kp{i}/Tm);

        [Pole1, Pole2, R] = getPole(Km, Kp{i}, Tm);


        sys = tf(wn_squared , [1, 1/Tm, wn_squared]);
        name = names{j} + " Plant";
        figure('Name', name);
        step(sys)
        title(name);
        subtitle("Kp = " + Kp{i})
        xlim([0, 0.5]);
        ylim([0, 1.6]);
        grid on;

        row = row + 1;
        T(row, :) = {name == "Weighted Plant", Km, Tm, Kp{i}, Pole1, Pole2, R(1), R(2)};
        
    end
end

disp (T);

%% Helper Funcs

function [pole1, pole2, r] = getPole(km, kp, tm)
    real = -1/(2*tm);
    imag = 1/(2*tm)*sqrt(1-(4*km*tm*kp));
    pole1 = real + imag;
    pole2 = real - imag;
    r = roots([1, 1/tm, (km*kp)/tm]);
end
