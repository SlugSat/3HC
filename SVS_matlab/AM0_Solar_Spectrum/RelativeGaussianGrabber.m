%******************** Relative Gaussian Grabber **************************%
%
% Zachary Klein - LED Solar Simulator - Summer 2010
% University of Illinois at Urbana-Champaign
%
% This m file reads in an image of a relative intesity vs wavelength
% Gaussian curve and turns it into a binary matrix the same dimensions as
% the image's pixel dimensions. A zero in the matrix corresponds to a
% non-white spot in the image. The image is then scanned to find the curve,
% whose x and y coordinates are stored in a two column matrix representing
% wavelength and intensity. This matrix is then scaled so that the
% resultant matrix represents the relative intesity gaussian curve seen
% in the image. In order for this code to run properly the image must be
% cleaned of all colored pixel above the gaussian curve. The resultant
% curve is more accurate if the image's pixel height is unaltered and
% large. The function call takes the arguments
%
% RelativeGaussianGrabber('fileName', StartWL, EndWL)
% where:
% StartWL is the starting wavelength of the image
% EndWL is the ending wavelength of the image
%
%
%*************************************************************************%
function Gauss = RelativeGaussianGrabber(file, StartWL, EndWL)
Image = imread(file);
BW = im2bw(Image, graythresh(Image));
rows = length(BW);
columns=EndWL-StartWL;
BW=imresize(BW, [rows, columns]);

GaussImage(columns, 2)=zeros;
for i=1:columns
 for j=1:rows
 if BW(j, i) == 0
 GaussImage(i,1)=i;
 %image indices are highest for zero intensity, so subtract
 %current intensity reading (j) from highest index (row)
 GaussImage(i,2)=rows-j;
 break
 else %the intensity at wavelength i is zero
 GaussImage(i,1)=i;
  GaussImage(i,2)=0;
 end
 end
end
%determine the size of the scaled matix to declare prior to
%copying values into it. This speeds up program execution. We are copying
%only values where intensity is not zero
ScaledSize=0;
for i=1:columns
 if GaussImage(i,2) ~= 0
 ScaledSize=ScaledSize+1;
 end
end
GaussImageScaled(ScaledSize,2)=zeros;
%%Relative Intensity
%Scale the curve to show proper wavelength and relative intensity
maxIntens=max(GaussImage(:,2)); %find max to calculate relative intensity
j=1;
for i=1:columns
 if GaussImage(i,2) ~= 0
 GaussImageScaled(j,1)=GaussImage(i,1)+StartWL;
 GaussImageScaled(j,2)=GaussImage(i,2)/maxIntens;
 j=j+1;
 end
end
%Return Relative Intensity Data
Gauss(:,1) = GaussImageScaled(:,1);
Gauss(:,2) = GaussImageScaled(:,2);
end