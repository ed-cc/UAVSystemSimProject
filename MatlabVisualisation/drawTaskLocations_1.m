clear all;
close all;

inputData0 = readtable("TaskList_0.csv");
times = readtable("UAVno_Time.csv");

figure;
hold all;

for i = 1:length(inputData0.x)
    if inputData0.type(i) == 0
        plot(inputData0.x(i), inputData0.y(i), 'om')
    end
    if inputData0.type(i) == 1
        plot(inputData0.x(i), inputData0.y(i), 'ob')
    end 
    
end
plot ( 0, 0, '*g');
title("Locations of Randomly Generated Tasks and Landing Pads");

histData = zeros(21);
total = 0;

for i = 1:length(inputData0.x)
    xVal = floor( floor(inputData0.x(i) + 1000) / 100 ) + 1;
    yVal = floor( floor(inputData0.y(i) + 1000) / 100 ) + 1;
    histData(xVal, yVal) = histData(xVal, yVal) + 1;
    total = total + 1;
end
x = (-1000:100:1000);
y = (-1000:100:1000);
figure;
surf(x, y, histData); % 'LineStyle', 'none', 'FaceColor', 'interp'
colorbar;
title("Task Density");
