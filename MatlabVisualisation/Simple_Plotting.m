clear; 

inputData0 = readtable("UAV_5_0.csv");
inputData1 = readtable("UAV_5_1.csv");
inputData2 = readtable("UAV_5_2.csv");
inputData3 = readtable("UAV_5_3.csv");
inputData4 = readtable("UAV_5_4.csv");
inputData5 = readtable("UAV_5_5.csv");
inputData6 = readtable("UAV_5_6.csv");
inputData7 = readtable("UAV_5_7.csv");
inputData8 = readtable("UAV_5_8.csv");
inputData9 = readtable("UAV_5_9.csv");

plot3(inputData0.x, inputData0.y, inputData0.z);
hold all;
plot3(inputData1.x, inputData1.y, inputData1.z);
plot3(inputData2.x, inputData2.y, inputData2.z);
plot3(inputData3.x, inputData3.y, inputData3.z);
plot3(inputData4.x, inputData4.y, inputData4.z);
plot3(inputData5.x, inputData5.y, inputData5.z);
plot3(inputData6.x, inputData6.y, inputData6.z);
plot3(inputData7.x, inputData7.y, inputData7.z);
plot3(inputData8.x, inputData8.y, inputData8.z);
plot3(inputData9.x, inputData9.y, inputData9.z);
