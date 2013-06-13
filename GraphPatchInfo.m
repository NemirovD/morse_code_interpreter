function [ ] = GraphPatchInfo( filename )
%UNTITLED Summary of this function goes here
%   Detailed explanation goes here

fid = fopen(filename);
i=1;
j=1;
line = fgetl(fid);
while ischar(line)
    if(strcmp(line,'END'))
        i=i+1;
        j=1;
    else
        tokens = sscanf(line,'%f %f');
        times(i,j) = tokens(1);
        data(i,j) = tokens(2);
        j=j+1;
    end
    line = fgetl(fid);
end


maxSets = size(data ,1);

for  set =1:maxSets  
    plot(times(set,:),data(set,:));
    hold all;
    
end
    

end