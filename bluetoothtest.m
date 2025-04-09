clear;
close all;

bruh = ble("Kyle");
theString = characteristic(bruh, "4FAFC201-1FB5-459E-8FCC-C5C9C331914B", "BEB5483E-36E1-4688-B7F5-EA07361B26A8");

while true
    data = char(read(theString,'latest'));
    data = str2double(data);
    rpm = floor(data/100);
    speed = mod(data,100);
    disp('RPM: ');
    disp(rpm);
    disp('Speed: ');
    disp(speed);
    pause(2);
end