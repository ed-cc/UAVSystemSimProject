clear all;
close all;

inputData = readtable("UAVno_Time.csv");

inputData.time = inputData.time/1000;

figure;
plot(inputData.UAV_no, inputData.time);
hold all;
grid on;
xlabel("Number of UAVs Operating");
ylabel("Time Taken /s");
title("A graph of Time Taken to Complete 50 Random Tasks vs Number of UAVs Operating");
