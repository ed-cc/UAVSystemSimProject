clear all;
close all;

inputData = readtable("UAVno_Time.csv");

inputData.time = inputData.time/1000;

timeSumA = 0;
timeSumB = 0;
totalA = 0;
totalB = 0;
for i = 1:length(inputData.time)
    if (mod(inputData.UAV_no(i), 2) == 0)
        timeSumA = timeSumA + inputData.time(i);
        totalA = totalA + 1;
    end
    if (mod(inputData.UAV_no(i), 2) ~= 0)
        timeSumB = timeSumB + inputData.time(i);
        totalB = totalB + 1;
    end       
end

timeSumA = timeSumA / totalA
timeSumB = timeSumB / totalB
diff = (timeSumB - timeSumA) / timeSumB * 100