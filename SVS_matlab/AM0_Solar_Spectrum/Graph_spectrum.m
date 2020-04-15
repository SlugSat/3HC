%---------------------------------------------------------------------------------------------------------------------
%Created by: Juan Ayala
%Date Created:4/15/20
%Uses ASTM's data of Spectral irradiance at AM0 to plot data and calculate
%the visible spectrums irradiance compared to the total irradiance
%--------------------------------------------------------------------------------------------------------------------
clc;
clear;
close all;

filename='ASTM_SolarIrradiance_AM0.csv';
data = csvread(filename,1,0,[1 0 1697 1]);
wavelength = data(:,1);
irradiance = data(:,2);

colorSpectrum = data(wavelength(:,1)>0.3 & wavelength(:,1)<0.9,:);
color_wavelength = colorSpectrum(:,1);
color_irradiance = colorSpectrum(:,2);

total=trapz(wavelength,irradiance);
colorIrradiance=trapz(color_wavelength,color_irradiance);
percentageOfVisibleLight= (colorIrradiance/total)*100;

fprintf('Total Irradiance %.2f\n',total);
fprintf('Visible Light Irradiance %.2f\n', colorIrradiance);
fprintf('Visible Light Irradiance percentage of total %.2f%%\n', percentageOfVisibleLight);

figure (1);
tiledlayout(2,1)

nexttile
plot(wavelength,irradiance);
hold on
area(color_wavelength,color_irradiance);
hold off
grid on;
axis([0 4 0 2300]);
title('Linear Plot of Spectral Irradiance @ AM0');
xlabel('Wavelength (\mum)'); 
ylabel('Spectral Irradiance (W/m^2 -\mum )');

nexttile
loglog(wavelength,irradiance);
hold on
area(color_wavelength,color_irradiance);
hold off
grid on;
title('log-log Plot of Spectral Irradiance @ AM0');
xlabel('Wavelength (\mum)'); 
ylabel('Spectral Irradiance (W/m^2 -\mum )');

