function [spectrum] = guass_estimate(lamda,bandwidth)
    dl=0.1;
    wavelength = (lamda-bandwidth*5):dl:(lamda+bandwidth*5);
    r_intensity = exp(-1.*( ( 2.7726.*((wavelength-lamda).^2)) ./ (bandwidth.^2) ));
    spectrum = [wavelength.' r_intensity.'];
end
