clc;
clear;
close all;

spectrum = guass_estimate(385,10);

figure (1);
plot(spectrum(:,1),spectrum(:,2));
