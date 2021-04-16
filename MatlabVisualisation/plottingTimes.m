clear all;
close all;

inputData = readtable("UAVno_Time.csv");

inputData.time = inputData.time/1000;

%UAVi = linspace(inputData.UAV_no(1), inputData.UAV_no(end), 10000);
%yi = interp1(inputData.UAV_no, inputData.time, UAVi);


%yy = smooth(yi, 1000);

figure;
plot(inputData.UAV_no, inputData.time);
hold all;
%plot(UAVi, yy);
grid on;
xlabel("Number of UAVs Operating");
ylabel("Time Taken /s");
title("A graph of Time Taken to Complete 50 Random Tasks vs Number of UAVs Operating");

