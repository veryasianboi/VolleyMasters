clear;
close all;

bruh = ble("Kyle");
theString = characteristic(bruh, "4FAFC201-1FB5-459E-8FCC-C5C9C331914B", "BEB5483E-36E1-4688-B7F5-EA07361B26A8");

while true
    data = char(read(theString,'latest'));
    %disp(data);
    splt = strsplit(data,','); 
    %disp(splt);
    rpm = str2double(splt(1));
    speed = str2double(splt(2));
    disp('RPM: ');
    disp(rpm);
    disp('Speed: ');
    disp(speed);
    pause(2);
    
end