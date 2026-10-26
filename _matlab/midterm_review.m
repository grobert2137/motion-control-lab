% TF Derrived from bode plot in class on 10/23
s = tf('s');
sys = (10^10)/((s+1000)*(s+1000));

grid on;

figure(1) 
pzmap(sys);

figure(2)
bode(sys);

figure(3);
h = pzplot(sys);grid on;