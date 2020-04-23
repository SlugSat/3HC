function [spectrum] = addSpectrum( source1, source2)
    size1=size(source1);
    size2=size(source2);
    index2=1;
    index=1;
    indexOut=1;
    output=source1;
    while index <= size1(1)
        if index2>size2(1)
            break;
        end
        if source1(index,1)>source2(index2,1)
            while source1(index,1)>source2(index2,1)
               if indexOut>1
                    output=[output(1:indexOut-1,:);source2(index2,:);output(indexOut:end,:)];
               else
                   output =[source2(index2,:);output];
                   index2+index2+1;
                   indexOut=indexOut+1;
                   continue;
               end
               if index>1 && source2(index2)>source1(index-1)
                   slope= (source1(index,2)-source1(index-1,2))/(source1(index,1)-source1(index-1,1));
                   output(indexOut,2)=slope*(source2(index2,1)-source1(index-1,1))+source1(index,2) + source2(index2,2);
               end
               indexOut=indexOut+1;
               index2=index2+1;
               if index2>size2(1)
                break;
               end
            end
        end
        if index2>size2(1)
            break;
        end
       if source1(index,1)==source2(index2,1)
           output(indexOut,2)=output(indexOut,2)+source2(index2,2);
           index2=index2+1;
      elseif index2>1 && index2<=size2(1)
            slope= (source2(index2,2)-source2(index2-1,2))/(source2(index2,1)-source2(index2-1,1));
            output(indexOut,2)=slope*(source1(index,1)-source2(index2-1,1))+source2(index2,2) + source1(index,2);
        end
        indexOut=indexOut+1;
        index=index+1;
    end
    if index2<size2(1)
        if source2(index2,1)>output(end,1)
            output=[output;source2(index2:end,:)];
        end
    end
    spectrum=output;
end