clear; 

number = "4";

inputData0 = readtable("UAV_"+ number +"_0.csv");
inputData1 = readtable("UAV_"+ number +"_1.csv");
inputData2 = readtable("UAV_"+ number +"_2.csv");
inputData3 = readtable("UAV_"+ number +"_3.csv");
inputData4 = readtable("UAV_"+ number +"_4.csv");
inputData5 = readtable("UAV_"+ number +"_5.csv");
inputData6 = readtable("UAV_"+ number +"_6.csv");
inputData7 = readtable("UAV_"+ number +"_7.csv");
inputData8 = readtable("UAV_"+ number +"_8.csv");
inputData9 = readtable("UAV_"+ number +"_9.csv");
inputData10 = readtable("UAV_"+ number +"_10.csv");
inputData11 = readtable("UAV_"+ number +"_11.csv");

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
plot3(inputData10.x, inputData10.y, inputData10.z);
plot3(inputData11.x, inputData11.y, inputData11.z);

ainputData4 = readtable("TaskList_4.csv");
for i = 1:length(ainputData4.x)
    if ainputData4.type(i) == 0
        plot(ainputData4.x(i), ainputData4.y(i), 'om')
    end
    if ainputData4.type(i) == 1
        plot(ainputData4.x(i), ainputData4.y(i), 'ob')
    end 
    
end
plot ( -500, 500, '*g');
plot ( -500, -500, '*g');
plot ( 500, 500, '*g');
plot ( 500, -500, '*g');
plot ( 0, 0, '*g');
