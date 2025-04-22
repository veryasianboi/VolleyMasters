clear;
close all;
%for plotting
minspeed = 0;
maxspeed = 100;
minrpm = 0;
maxrpm = 6000;

weight = 0.022; %kg
dt = 0.007;%s

bruh = ble("Kyle");
theString = characteristic(bruh, "4FAFC201-1FB5-459E-8FCC-C5C9C331914B", "BEB5483E-36E1-4688-B7F5-EA07361B26A8");

time = 200; %ms
prevdata = zeros(time,2); %30 seconds of data

count = 1;
force = [];
R = 8.917; %% change
r = 0.3346; %%change
ratio = R/r;

while count < time
    data = char(read(theString,'latest'));
    splt = strsplit(data, ',');
    data = str2double(splt);
    
    prevdata(count,:) = data;
    rpm = data(1);
    speed = mod(data(2) * ratio,100); % change
    disp('RPM: ');
    disp(rpm);
    disp('Speed: ');
    disp(speed);
    
    count = count + 1;
end
minRPMpeak = 5; %%change
maxRPMpeak = 6000; %%change
minSpeedpeak = 5; %%change
maxSpeedpeak = 70; %%change
speedSmooth = smoothdata(prevdata(:,2), 'movmean',1); %possibly change last value
RPMSmooth = smoothdata(prevdata(:,1), 'movmean',1);%possibly change last value

speedPeaks = findpeaks(speedSmooth, "MinPeakHeight", minSpeedpeak);
RPMPeaks = findpeaks(RPMSmooth, "MinPeakHeight", minRPMpeak);

%RPM Plot
figure(1)
plot(RPMPeaks);
%ylim([minrpm maxrpm]);
%legend('RPM', Location='northeast');
xlabel('Hit Number');
ylabel('Revolutions Per Minute');

figure(2)
plot(speedPeaks);
%ylim([minspeed maxspeed])
%legend('Translational Speed', Location='northeast');
ylabel('Translational Speed [MPH]');
xlabel('Hit Number');

figure(3) %%just translational force
force = (speedPeaks * 0.44704 * weight) / dt;
plot(force)
%legend('Translational Force', Location='northeast');
xlabel('Translational Force [N]');
ylabel('Hit Number');

figure(4)
plot(speedSmooth)

figure(5)
plot(prevdata(:,2))