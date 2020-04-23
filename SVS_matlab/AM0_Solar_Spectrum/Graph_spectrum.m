%---------------------------------------------------------------------------------------------------------------------
%Created by: Juan Ayala
%Date Created:4/15/20
%Uses ASTM's data of Spectral irradiance at AM0 to plot data and calculate
%the visible spectrums irradiance compared to the total irradiance
%--------------------------------------------------------------------------------------------------------------------
clc;
clear;
close all;

filename='GREEN_LED.csv';
LED_G = csvread(filename,1,0,[1 0 100 1]);

filename='GW_CS8PM1_EM__blue__spectrum.csv';
LED_W_B = csvread(filename,1,0,[1 0 66 1]);

filename='GW_CS8PM1_EM_yellow_spectrum.csv';
LED_W_Y = csvread(filename,1,0,[1 0 135 1]);

LED_W=addSpectrum(LED_W_B,LED_W_Y);

filename='ASTM_SolarIrradiance_AM0.csv';
Solar_reference = csvread(filename,1,0,[1 0 1697 1]);


colorSpectrum = Solar_reference(Solar_reference(:,1)>0.3 & Solar_reference(:,1)<0.9,:);

total=trapz(Solar_reference(:,1),Solar_reference(:,2));
colorIrradiance=trapz(colorSpectrum(:,1),colorSpectrum(:,2));
percentageOfVisibleLight= (colorIrradiance/total)*100;

fprintf('Total Irradiance %.2f\n',total);
fprintf('Visible Light Irradiance %.2f\n', colorIrradiance);
fprintf('Visible Light Irradiance percentage of total %.2f%%\n', percentageOfVisibleLight);

figure (1);
tiledlayout(2,1)

nexttile
plot(Solar_reference(:,1),Solar_reference(:,2));
hold on
area(colorSpectrum(:,1),colorSpectrum(:,2));
hold off
grid on;
axis([0 4 0 2300]);
title('Linear Plot of Spectral Irradiance @ AM0');
xlabel('Wavelength (\mum)'); 
ylabel('Spectral Irradiance (W/m^2 -\mum )');

nexttile
loglog(Solar_reference(:,1),Solar_reference(:,2));
hold on
area(colorSpectrum(:,1),colorSpectrum(:,2));
hold off
grid on;
title('log-log Plot of Spectral Irradiance @ AM0');
xlabel('Wavelength (\mum)'); 
ylabel('Spectral Irradiance (W/m^2 -\mum )');

figure (2);

plot(Solar_reference(:,1),Solar_reference(:,2));
hold on
plot(LED_W(:,1)/1000,LED_W(:,2)*2000);
hold on
plot(LED_G(:,1)/1000,LED_G(:,2)*2000);
hold off

grid on;
axis([0 1 0 2300]);
title('Plot of Spectral Irradiance @ AM0 and White LED');
xlabel('Wavelength (\mum)'); 
ylabel('Spectral Irradiance (W/m^2 -\mum )');
LED_G=[LED_G(:,1) LED_G(:,2)];
full_LED = addSpectrum(LED_W,LED_G);

figure(3);
plot(Solar_reference(:,1),Solar_reference(:,2));

hold on
axis([0 1 0 2300]);
section=full_LED(full_LED(:,1)>=400 & full_LED(:,1)<=500,:);
LED_sum=trapz(section(:,1)/1000,section(:,2)*2000);
area(section(:,1)/1000,section(:,2)*2000);
fprintf('400-500 %.2f\n',(LED_sum)/total);
hold on
section=full_LED(full_LED(:,1)>=500 & full_LED(:,1)<=600,:);
LED_sum=trapz(section(:,1)/1000,section(:,2)*2000);
area(section(:,1)/1000,section(:,2)*2000);
fprintf('500-600 %.2f\n',(LED_sum)/total);

hold on
section=full_LED(full_LED(:,1)>=600 & full_LED(:,1)<=700,:);
LED_sum=trapz(section(:,1)/1000,section(:,2)*2000);
area(section(:,1)/1000,section(:,2)*2000);
fprintf('600-700 %.2f\n',LED_sum/total);

hold on
section=full_LED(full_LED(:,1)>=700 & full_LED(:,1)<=800,:);
LED_sum=trapz(section(:,1)/1000,section(:,2)*2000);
area(section(:,1)/1000,section(:,2)*2000);
area(section(:,1)/1000,section(:,2)*2000);
fprintf('700-800 %.2f\n',LED_sum/total);

hold on
section=full_LED(full_LED(:,1)>=800 & full_LED(:,1)<=900,:);
LED_sum=trapz(section(:,1)/1000,section(:,2)*2000);
area(section(:,1)/1000,section(:,2)*2000);
fprintf('800-900 %.2f\n',LED_sum/total);

hold off

title('Plot of Spectral Irradiance @ AM0 and White and Green');
xlabel('Wavelength (\mum)'); 
ylabel('Spectral Irradiance (W/m^2 -\mum )');


