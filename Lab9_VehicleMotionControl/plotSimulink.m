data = out.simout.Data;
time = out.tout(:, 1);

car1 = data(:, 1);


% Find index where value is between 63% and 64% of final value
tau1_idx = find(car1 > 0.63 & car1<0.64)

disp(time(1151))

car2 = data(:, 2);


figure('Name', "Car 1 Acceleration VS Time")
plot(time, car1);
grid on;

figure('Name', "Car 2 Acceleration VS Time")
plot(time, car2);
grid on;





%% Tau is approx 1.15 s for CAR 1 


