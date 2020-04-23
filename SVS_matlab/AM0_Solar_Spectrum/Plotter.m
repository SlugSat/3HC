%******************** LED Gaussian Plotter **********************%
% %
% Zachary Klein - LED Solar Simulator - Summer 2010 %
% University of Illinois at Urbana-Champaign %
% %
% This m file plots the LED Gaussian Spectrum curves obtained %
% from the LED data sheets. The function RelativeGaussianGrabber %
% uses an edited image from the data sheet to obtain the %
% relative gaussian curve data in two columns, wavelength and %
% relative intensity. The function call takes the arguments %
% %
% RelativeGaussianGrabber('fileName', StartWL, EndWL) %
% where: %
% StartWL is the starting wavelength of the image %
% EndWL is the ending wavelength of the image %
% %
% In order for this function to work properly the image must be %
% edited so that there are no colored pixels above the gaussian %
% curve, including at the borders. There are several examples %
% of how this is performed in this directory, where fileName_RAW %
% is the snapshot taken from the data sheet using Adobe Reader's %
% snapshot tool, and fileName is the cleaned up image. %
% %
% Plotting this figure requires the the associated jpeg or png %
% image files as well as the m file "RelativeGaussianGrabber.m" %
% to be in the same directory. %
% %
%****************************************************************%
close all; clc;
hold on
% UV = RelativeGaussianGrabber('UV.png', 310, 510);
% plot(UV(:,1), UV(:,2), 'm')
% RoyalBlue = RelativeGaussianGrabber('450L.png', 400,700);
% plot(RoyalBlue(:,1), RoyalBlue(:,2), 'm', 'LineWidth', 2)
% Blue = RelativeGaussianGrabber('Blue.png', 400, 700);
% plot(Blue(:,1), Blue(:,2), 'b', 'LineWidth', 2)
% Cyan = RelativeGaussianGrabber('Cyan.png', 400, 700);
% plot(Cyan(:,1), Cyan(:,2), 'c', 'LineWidth', 2)
% Green = RelativeGaussianGrabber('Green.png', 400, 700);
% plot(Green(:,1), Green(:,2), 'g', 'LineWidth', 2)
% Red = RelativeGaussianGrabber('630L.png', 390,690);
% plot(Red(:,1), Red(:,2), 'r', 'LineWidth', 2)
White = RelativeGaussianGrabber('White.png', 350, 800);
plot(White(:,1), White(:,2), 'black')
% WarmWhite = RelativeGaussianGrabber('WarmWhite.png', 350, 800);
% plot(WarmWhite(:,1), WarmWhite(:,2))%, 'y', 'LineWidth', 3)
% Marubeni420 = RelativeGaussianGrabber('420.png', 369,469);
% plot(Marubeni420(:,1), Marubeni420(:,2))%, 'y', 'LineWidth', 3)
% Marubeni450 = RelativeGaussianGrabber('450M.png', 400,500);
% plot(Marubeni450(:,1), Marubeni450(:,2))%, 'y', 'LineWidth', 3)
% Marubeni490 = RelativeGaussianGrabber('450M.png', 440,540);
% plot(Marubeni490(:,1), Marubeni490(:,2))%, 'y', 'LineWidth', 3)
% LedEngin660 = RelativeGaussianGrabber('LedEngin660two.JPG', 400,700);
% plot(LedEngin660(:,1), LedEngin660(:,2))%, 'y', 'LineWidth', 3)
% Marubeni670 = RelativeGaussianGrabber('670.pnG', 620,720);
% plot(Marubeni670(:,1), Marubeni670(:,2))%, 'y', 'LineWidth', 3)
% Marubeni680 = RelativeGaussianGrabber('680.png', 628,728);
% plot(Marubeni680(:,1), Marubeni680(:,2))%, 'y', 'LineWidth', 3)
% Marubeni700 = RelativeGaussianGrabber('700.png', 650,750);
% plot(Marubeni700(:,1), Marubeni700(:,2))%, 'y', 'LineWidth', 3)
% Marubeni710 = RelativeGaussianGrabber('710.png', 660,760);
% plot(Marubeni710(:,1), Marubeni710(:,2))%, 'y', 'LineWidth', 3)
% LedEngin740 = RelativeGaussianGrabber('737.png', 550,850);
% plot(LedEngin740(:,1), LedEngin740(:,2))%, 'y', 'LineWidth', 3)
% Marubeni760 = RelativeGaussianGrabber('760.png', 710,810);
% plot(Marubeni760(:,1), Marubeni760(:,2))%, 'y', 'LineWidth', 3)
%
% Marubeni770 = RelativeGaussianGrabber('770.png', 720,820);
% plot(Marubeni770(:,1), Marubeni770(:,2))%, 'y', 'LineWidth', 3)
% Marubeni780 = RelativeGaussianGrabber('780.png', 730,830);
% plot(Marubeni780(:,1), Marubeni780(:,2))%, 'y', 'LineWidth', 3)
% Marubeni800 = RelativeGaussianGrabber('800.pnG', 750,850);
% plot(Marubeni800(:,1), Marubeni800(:,2))%, 'y', 'LineWidth', 3)
% Marubeni810 = RelativeGaussianGrabber('810.png', 760,860);
% plot(Marubeni810(:,1), Marubeni810(:,2))%, 'y', 'LineWidth', 3)
% q830 = RelativeGaussianGrabber('830.png',740 ,940);
% plot(q830(:,1), q830(:,2))%, 'y', 'LineWidth', 3)
% Marubeni850 = RelativeGaussianGrabber('850.jpg', 750,950);
% plot(Marubeni850(:,1), Marubeni850(:,2))%, 'y', 'LineWidth', 3)
% Marubeni870 = RelativeGaussianGrabber('870.png', 770,970);
% plot(Marubeni870(:,1), Marubeni870(:,2))%, 'y', 'LineWidth', 3)
% Osram880 = RelativeGaussianGrabber('Osram880.JPG', 750,1000);
% plot(Osram880(:,1), Osram880(:,2))%, 'y', 'LineWidth', 3)
% Marubeni890 = RelativeGaussianGrabber('890.jpg', 790,990);
% plot(Marubeni890(:,1), Marubeni890(:,2))%, 'y', 'LineWidth', 3)
% Marubeni910 = RelativeGaussianGrabber('910.png', 810,1010);
% plot(Marubeni910(:,1), Marubeni910(:,2))%, 'y', 'LineWidth', 3)
% Marubeni970 = RelativeGaussianGrabber('970.png', 870,1070);
% plot(Marubeni970(:,1), Marubeni970(:,2))%, 'y', 'LineWidth', 3)
% Marubeni1050 = RelativeGaussianGrabber('1050.png', 950,1150);
% plot(Marubeni1050(:,1), Marubeni1050(:,2))%, 'y', 'LineWidth', 3)
% Marubeni1070 = RelativeGaussianGrabber('1070.png', 970,1170);
% plot(Marubeni1070(:,1), Marubeni1070(:,2))%, 'y', 'LineWidth', 3)
xlabel('Wavelength (nm)')
ylabel('Relative Intensity')
title('LED Spectrum')
grid on