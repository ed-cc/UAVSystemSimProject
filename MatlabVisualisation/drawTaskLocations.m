clear all;
close all;

inputData0 = readtable("TaskList_0.csv");
inputData1 = readtable("TaskList_1.csv");
inputData2 = readtable("TaskList_2.csv");
inputData3 = readtable("TaskList_3.csv");
inputData4 = readtable("TaskList_4.csv");
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

figure;
hold all;

for i = 1:length(inputData1.x)
    if inputData1.type(i) == 0
        plot(inputData1.x(i), inputData1.y(i), 'om')
    end
    if inputData1.type(i) == 1
        plot(inputData1.x(i), inputData1.y(i), 'ob')
    end 
    
end
plot ( -1000, 0, '*g');
plot ( 1000, 0, '*g');
title("Locations of Randomly Generated Tasks and Landing Pads");

figure;
hold all;

for i = 1:length(inputData2.x)
    if inputData2.type(i) == 0
        plot(inputData2.x(i), inputData2.y(i), 'om')
    end
    if inputData2.type(i) == 1
        plot(inputData2.x(i), inputData2.y(i), 'ob')
    end 
    
end
plot ( 866, -500, '*g');
plot ( -866, -500, '*g');
plot ( 0, 1000, '*g');
title("Locations of Randomly Generated Tasks and Landing Pads");

figure;
hold all;

for i = 1:length(inputData3.x)
    if inputData3.type(i) == 0
        plot(inputData3.x(i), inputData3.y(i), 'om')
    end
    if inputData3.type(i) == 1
        plot(inputData3.x(i), inputData3.y(i), 'ob')
    end 
    
end
plot ( -707, 707, '*g');
plot ( -707, -707, '*g');
plot ( 707, 707, '*g');
plot ( 707, -707, '*g');
title("Locations of Randomly Generated Tasks and Landing Pads");

figure;
hold all;

for i = 1:length(inputData4.x)
    if inputData4.type(i) == 0
        plot(inputData4.x(i), inputData4.y(i), 'om')
    end
    if inputData4.type(i) == 1
        plot(inputData4.x(i), inputData4.y(i), 'ob')
    end 
    
end
plot ( -707, 707, '*g');
plot ( -707, -707, '*g');
plot ( 707, 707, '*g');
plot ( 707, -707, '*g');
plot ( 0, 0, '*g');
title("Locations of Randomly Generated Tasks and Landing Pads");


%Time Taken
figure;
hold all;
lpNumber = 1:5;
plot(lpNumber, times.time);
xlabel("Number of Landing Pads");
xticks([1 2 3 4 5]);
ylabel("Time taken /s");
title("Time Taken to Complete 400 Random Tasks vs Number of Landing Pads");
grid on;


%Mean Distance Travelled 
figure;
hold all;
avDist(1) = mean(inputData0.distance);
avDist(2) = mean(inputData1.distance);
avDist(3) = mean(inputData2.distance);
avDist(4) = mean(inputData3.distance);
avDist(5) = mean(inputData4.distance);
plot (lpNumber, avDist);
ylabel("Mean Distance Travelled per Task /m");
xlabel("Number of Landing Pads");
title("Mean Distance Travelled vs Number of Landing Pads Available");
xticks([1 2 3 4 5]);
grid on;

%Area covered
figure;
hold all;
arCov(1) = pi;
arCov(2) = 2 * pi;
arCov(3) = ( 3 * pi - ( pi - 1.5 * 3^0.5) );
arCov(4) = ( 3 * pi - ( pi -2 ) );
arCov(5) = ( 3 * pi - ( pi -2 ) );
plot (lpNumber, arCov);
title("Area Covered vs Number of Landing Pads");
ylabel("Area Covered /km^2");
xlabel("Number of Landing Pads");
xticks([1 2 3 4 5]);
grid on;


% figure;
% hold all;
% plot ( -500, 500, 'om');
% plot ( -500, -500, 'ob');
% plot ( 500, 500, '*g');
% legend("Observation","Delivery","Landing Pad");





