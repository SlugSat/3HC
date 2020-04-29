function [spectrum] = guass_estimate(lamda,bandwidth)
    dl=0.1;
    wavelength = (lamda-bandwidth*5):dl:(lamda+bandwidth*5);
    r_intensity = exp(-( ( 4.*log(2).*((wavelength-lamda).^2)) ./ (2.*sqrt(2.*log(2)).*bandwidth) ));
    spectrum = [wavelength.' r_intensity.'];
end
