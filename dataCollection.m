
clear;
clc;

s=serial('COM4','BaudRate',9600);
fopen(s);
writedata=uint16(500); %0x01F4
fwrite(s,writedata,'uint16') %write datab

for i=1:50 %read 2 lines of data
    for j = 1:50
        readData=fscanf(s);
        data(j,i) = str2num(readData)
    end
end

imagesc(data)
fclose(s);

delete(s);
