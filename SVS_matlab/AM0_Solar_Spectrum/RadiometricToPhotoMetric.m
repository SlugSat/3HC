clc;
clear;
close all;
filename='GW_CS8PM1_EM__blue__spectrum.csv';
LED_W_B = csvread(filename,1,0,[1 0 66 1]);

filename='GW_CS8PM1_EM_yellow_spectrum.csv';
LED_W_Y = csvread(filename,1,0,[1 0 135 1]);

LED_W= [LED_W_B(end-1,:);LED_W_Y];
filename='PhotopicCurve.csv';
Solar_Reference = csvread(filename,1,0,[1 0 470 1]);

colorSpectrum = Solar_Reference(Solar_Reference(:,1)>379 & Solar_Reference(:,1)<781,:);

%totalY=trapz(wavelengthY,irradianceY);
%totalB=trapz(wavelengthB,irradianceB);
%percentageOfVisibleLight= (colorIrradiance/total)*100;

%fprintf('Total Irradiance %.2f\n',total);
%fprintf('Visible Light Irradiance %.2f\n', colorIrradiance);
%fprintf('Visible Light Irradiance percentage of total %.2f%%\n', percentageOfVisibleLight);

figure (1);
%tiledlayout(2,1)

%nexttile
plot(LED_W_Y(:,1),LED_W_Y(:,2));
hold on
plot(LED_W_B(:,1),LED_W_B(:,2));
hold off
%area(color_wavelength,color_irradiance);
%hold off
%grid on;
%axis([0 4 0 2300]);
%title('Linear Plot of Spectral Irradiance @ AM0');
%xlabel('Wavelength (\mum)'); 
%ylabel('Spectral Irradiance (W/m^2 -\mum )');
figure(2);
[l]=combine(LED_W_B,LED_W_Y);
plot(l(:,1),l(:,2));
%nexttile
%loglog(wavelength,irradiance);
%hold on 
%area(color_wavelength,color_irradiance);
%hold off
%grid on;
%title('log-log Plot of Spectral Irradiance @ AM0');
%xlabel('Wavelength (\mum)'); 
%ylabel('Spectral Irradiance (W/m^2 -\mum )');