clear all;
close all;

inputData0 = readtable("UAV_0_0.csv");


plot3(inputData0.x, inputData0.y, inputData0.z);
hold all;

