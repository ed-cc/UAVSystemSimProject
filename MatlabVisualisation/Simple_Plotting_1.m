


for number = 0:4
    for j = 0:11
        inputData{j+1} = readtable("UAV_"+ number +"_" + j + ".csv");
    end
    
    figure;
    hold all;
    for i = 1:length(inputData)
        plot (inputData{i}.x, inputData{i}.y);
    end
    
    ainputData4 = readtable("TaskList_" + number + ".csv");
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
    title("Routes Taken to Complete Tasks With " + (number+1) + " Landing Pads");
end