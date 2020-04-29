function [spectrum] = combineSpectrum( source1, source2, num1, num2)
    spectrum = [source1(:,1) source1(:,2).*num1];
    source2(:,2)=source2(:,2).*num2;
    band = source1 (source1(:,1)>source2(1,1) & source1(:,1)<source2(end,1),1);
    interp=interp1(source2(:,1),source2(:,2),band, 'spine');
    spectrum(spectrum(:,1)>=band(1) & spectrum(:,1)<=band(end) ,2) = spectrum(spectrum(:,1)>=band(1) & spectrum(:,1)<=band(end) ,2) + interp;
    if source2(end,1)>source1(end,1)   
        spectrum = [spectrum; source2(source1(end,1)<source2(:,1),:)];   
    end
    if source2(1,1)<source1(1,1)   
        spectrum = [ source2(source1(1,1)>source2(:,1),:); spectrum];
    end
end