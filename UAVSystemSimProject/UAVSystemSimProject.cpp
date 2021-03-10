#include <iostream>
#include <fstream>
#include <time.h>
#include "Header.h"
#include "UAVObject.h"
#include "TaskObject.h"


//Look through provided list of UAVs to find one that is available and is of UAVType type.
//Returns the place in the list of the first appropriate UAV.
int findAvailableUAV(std::vector<UAVObject> listOfUAVs, UAVType type);

//Returns the first unassigned task in the task stack
int findUnassignedTask(std::vector<TaskObject> taskStack);

//Write a CSV file with all of the data logged from the given UAV
bool writeCSV(UAVObject sourceUAV);

//Pulls a list of UAV objects defined in a file
std::vector<UAVObject> getUAVsFromFile(std::string filename);

//Pulls a list of Task objects defined in a file
std::vector<TaskObject> getTasksFromFile(std::string filename);

//Finds the nearest of comparisonLocations to startingPos and returns this position along with the distance as a pair
std::pair<double, position> findClosest(position startingPos, std::vector<position> comparisonLocations);

//Generates a random list of tasks of specified type - default is random
std::vector<TaskObject> randomTasklistGenerator(int noTasks, double maxTotalDist, position initalLocation, std::vector<position> finalLocations, TaskType requestedType = TaskType::random);

position UAVDepotPos{ 0, 0, 0};
int GlobalTime = 0;

int main()
{
    std::vector<UAVObject> extantUAVs;
    for (int i = 0; i < 2; i++)
    {
        UAVObject tempUAV;
        std::string tempName = ("UAV_" + std::to_string(i));
        tempUAV.init(GlobalTime, UAVDepotPos, tempName, UAVType::standard, RouteType::direct, true);
        extantUAVs.push_back(tempUAV);
    }

    std::vector<TaskObject> listOfTasks;
    for (int i = 0; i < 2; i++)
    {
        TaskObject tempUAV;
        tempUAV.deliveryTask(GlobalTime, UAVDepotPos, position{ 50,100,0 }, position{ 100,50,0 }, UAVDepotPos, 500, UAVType::standard);
        listOfTasks.push_back(tempUAV);
    }

    position anotherDepot{ 20,20,0 };
    std::vector<position> positionList;
    positionList.push_back(UAVDepotPos);
    positionList.push_back(anotherDepot);


    auto moreTasks = randomTasklistGenerator(100, 100, UAVDepotPos, positionList);
    
    for (int i = 0; i < 2; i++)
    {
        //int testAvail = findAvailableUAV(extantUAVs, UAVType::standard);
        extantUAVs[i].assignTask(moreTasks[i]);
        moreTasks[i].setAssigned(true);
    }

    //Timestep for each simulation loop
    int timeStep = 100; //In ms

    //Number of times to run the simulation loop
    int numberOfLoops = 10 * 20000;

    //Simulation loop
    for (int ii = 0; ii < numberOfLoops; ii++)
    {
        

        for (unsigned int i = 0; i < extantUAVs.size(); i++)
        {
            //Run each UAVs loop function with the current time
            extantUAVs[i].loopFunc(GlobalTime);
        }

        if (extantUAVs[0].checkAvailability())
        {
            int nextTask = findUnassignedTask(moreTasks);
            if (nextTask == -1)
            {
                //No more tasks
            }
            else
            {
                //Assign the same UAV the next task
                moreTasks[nextTask].resetStartingPosition(extantUAVs[0].getPosition());
                extantUAVs[0].assignTask(moreTasks[nextTask]);
                moreTasks[nextTask].setAssigned(true);
                std::cout << "New task accepted!\n";
            }
        }

        //Finally increment the global time variable
        GlobalTime += timeStep;
    }

    //Do something with gathered simulation data here
    //...

    writeCSV(extantUAVs[0]);

}

int findAvailableUAV(std::vector<UAVObject> listOfUAVs, UAVType type)
{
    int availableUAV = -1;
    bool ignoreTypeCheck = false;
    //If type is set to any we don't need to check the UAV type
    if (type == UAVType::any)
    {
        ignoreTypeCheck = true;
    }

    for (unsigned int i = 0; i < listOfUAVs.size(); i++)
    {
        if (listOfUAVs[i].checkAvailability())
        {
            if (ignoreTypeCheck || (listOfUAVs[i].getType() == type))
            {
                availableUAV = i;
                break;
            }
            
        }
    }
    return availableUAV;
}

//Returns the first unassigned task in the task stack
int findUnassignedTask(std::vector<TaskObject> taskStack)
{
    int availableTask = -1;

    for (unsigned int i = 0; i < taskStack.size(); i++)
    {
        if (!taskStack[i].checkAssigned())
        {
            availableTask = i;
            break;
        }
    }
    return availableTask;
}

//Compares positions 1 and 2 -> Returns true if they are identical
bool comparePositions(position posit_1, position posit_2)
{
    double tolerance = 0.01;
    if ((abs(posit_1.x - posit_2.x) < tolerance) && (abs(posit_1.y - posit_2.y) < tolerance) && (abs(posit_1.z - posit_2.z) < tolerance))
    {
        return true;
    }
    else
    {
        return false;
    }
}

//Returns a position vector from pointA to pointB of length length
position get3DVector(position pointA, position pointB, double length)
{
    double xVal = pointB.x - pointA.x;
    double yVal = pointB.y - pointA.y;
    double zVal = pointB.z - pointA.z;
    double lengthA = sqrt(pow(xVal, 2) + pow(yVal, 2) + pow(zVal, 2));
    double lengthRatio = lengthA / length;

    return position{ xVal / lengthRatio, yVal / lengthRatio, zVal / lengthRatio };
}

//Returns the distance in 3D space between pointA and pointB
double get3DDistance(position pointA, position pointB)
{
    double xVal = pointA.x - pointB.x;
    double yVal = pointA.y - pointB.y;
    double zVal = pointA.z - pointB.z;
    return sqrt(pow(xVal, 2) + pow(yVal, 2) + pow(zVal, 2));
}

//Returns the distance in 2D space between pointA and pointB - ignores z positions
double get2DDistance(position pointA, position pointB)
{
    double xVal = pointA.x - pointB.x;
    double yVal = pointA.y - pointB.y;
    return sqrt(pow(xVal, 2) + pow(yVal, 2));
}

specsUAV getUAVSpecsFromType(UAVType requiredType)
{
    switch (requiredType) //Add cases for other UAV types
    {
    case(UAVType::standard):
        return STANDARDUAV;
        break;
    case(UAVType::custom):
        return STANDARDUAV;
        break;
    default:
        return STANDARDUAV;
        break;
    }
}

bool writeCSV(UAVObject sourceUAV)
{
    std::vector<logData> retrievedLogData = sourceUAV.getPositionLog();
    std::ofstream targetFile;
    std::string fileName = (sourceUAV.getName() + ".csv");
    targetFile.open(fileName);
    //Write a header
    targetFile << "x, y, z, Time, Activity,\n";

    //Write Data from log
    for (unsigned int i = 0; i < retrievedLogData.size(); i++)
    {
        targetFile << std::to_string(retrievedLogData[i].pos.x) << "," << std::to_string(retrievedLogData[i].pos.y) <<
            "," << std::to_string(retrievedLogData[i].pos.z) << "," << std::to_string(retrievedLogData[i].time) << "," <<
            std::to_string((int)(retrievedLogData[i].currentActivity)) << ",\n";
    }

    targetFile.close();
    return true;
}

//Takes tasks from a csv file and returns an object containing all of them
// Task formatting:
//
std::vector<TaskObject> getTasksFromFile(std::string filename)
{
    //Output tasks variable
    std::vector<TaskObject> outputTasks;
    
    std::ofstream targetFile;
    targetFile.open(filename);
    //Read first line of file
    //Turn into TaskObject
    //Append to output vector

    return outputTasks;
}

//Takes UAVs from a csv file and returns an object containing all of them
// UAV formatting:
//
std::vector<UAVObject> getUAVsFromFile(std::string filename)
{
    //Output tasks variable
    std::vector<UAVObject> outputUAVs;

    std::ofstream targetFile;
    targetFile.open(filename);
    //Read first line of file
    //Turn into UAVObject
    //Append to output vector

    return outputUAVs;
}


//Creates a list of random tasks originating at initialLocation of requestedType or of random type.
//Tasks will finish at the nearest location in finalLocation.
std::vector<TaskObject> randomTasklistGenerator(int noTasks, double maxLegDist, position initalLocation, std::vector<position> finalLocations, TaskType requestedType)
{
    //Initialise random seed with time
    srand((unsigned int)time(NULL));
    bool random = true;
    TaskType newTaskType = requestedType;
    std::vector<TaskObject> outputTasks;

    if (requestedType == TaskType::random)
    {
        //We should randomly choose each task as we allocate
        random = true;
    }
    else random = false;

    for (int i = 0; i < noTasks; i++)
    {
        TaskObject tempTask;
        if (random)
        {
            int tempType = rand() % 3;
            switch (tempType)
            {
            case(1):
                newTaskType = TaskType::observation;
                break;
            case(2):
                newTaskType = TaskType::delivery;
                break;
            default:
                newTaskType = TaskType::observation;
                break;
            }
        }
        //Generate a task of requested type
        if (newTaskType == TaskType::delivery)
        {
            position pickUp;
            position dropOff;
            //Create a random pick up location near the initialLocation
            double angle = ((rand() % 36000) / 100) / 180 * PI; //Angle in degrees of location from initialLocation
            double distance = (double)(rand() % (int)(maxLegDist * 100)) / 100.0; //Distance of location from initialLocation
            double xPart = distance * cos(angle); //Find x and y distances using trigonometry
            double yPart = distance * sin(angle);
            pickUp.x = initalLocation.x + xPart;
            pickUp.y = initalLocation.y + yPart;
            pickUp.z = initalLocation.z;

            std::pair<double, position> testFinalPos;
            do /******************************************************************************* WORK NEEDED ******************************************************/
            {
                //Create a random dropOff position and check if it is within maxLeg of a finalLocation - if not try again
                angle = ((double)(rand() % 36000) / 100.0) / 180.0 * PI; //Angle in degrees of location from initialLocation
                distance = (double)(rand() % (int)(maxLegDist * 100)) / 100.0; //Distance of location from initialLocation
                xPart = distance * cos(angle); //Find x and y distances using trigonometry
                yPart = distance * sin(angle);
                dropOff.x = initalLocation.x + xPart;
                dropOff.y = initalLocation.y + yPart;
                dropOff.z = initalLocation.z;

                testFinalPos = findClosest(dropOff, finalLocations);
                if (testFinalPos.first <= maxLegDist) break; //There is scope here to make this more intelligent - probably do this at some point
                //As this code can theoretically run forever - this must be improved
            } while (1);

            tempTask.deliveryTask(0, initalLocation, pickUp, dropOff, testFinalPos.second, 100, UAVType::any);
        }
        else if (newTaskType == TaskType::observation)
        {
            position location;
            int timeSpent = rand() % 15*60*1000; //Time spent at location in ms

            // A random point at a distance less than maxLegDist is the task location and then the nearest final location to this is chosen
            // This ensures that the second leg is always equal or shorter in length to the first.
            double angle = ((double)(rand() % 36000) / 100.0) / 180.0 * PI; //Angle in degrees of location from initialLocation
            double distance = (double)(rand() % (int)(maxLegDist * 100)) / 100.0; //Distance of location from initialLocation
            double xPart = distance * cos(angle); //Find x and y distances using trigonometry
            double yPart = distance * sin(angle);

            location.x = initalLocation.x + xPart;
            location.y = initalLocation.y + yPart;
            location.z = initalLocation.z;

            std::pair<double, position> finalLoc = findClosest(location, finalLocations);
            position finalLocation = finalLoc.second;

            tempTask.observationTask(0, initalLocation, location, initalLocation, timeSpent, UAVType::any);
        }
        outputTasks.push_back(tempTask);
    }

    return outputTasks;
}

//Finds the nearest of comparisonLocations to startingPos and returns this position along with the distance as a pair
std::pair<double, position> findClosest(position startingPos, std::vector<position> comparisonLocations)
{
    if (comparisonLocations.size() == 0)
    {
        return std::pair<double, position>{-1.0, position{ 0,0,0 }};
    }
    
    unsigned int i = 0;
    int nearest = i;
    double previous = get2DDistance(startingPos, comparisonLocations[i]);
    for (i = 1; i < comparisonLocations.size(); i++)
    {
        double currentDist = get2DDistance(startingPos, comparisonLocations[i]);
        if (currentDist < previous)
        {
            previous = currentDist;
            nearest = i;
        }
    }
    return std::pair<double, position>{previous, comparisonLocations[nearest]};
}

/*How is this going to work
* 
* A UAV is placed at a location
* A task is generated at a location
* Find a free UAV capable of completing the task
* Assign UAV to the task
* Get UAV to carry out task
* Set UAV to charge
* 
* 
* 
* Get to a point where I can produce some information and vary the number of tasks / task density
* Sweep through some variable and plot how this changes things
* 
* So make a way to randomly generate tasks
* Assign these tasks as UAVs become free
* Log time between task creation, acceptance and completion
* 
* 
* Functions:
* Generate tasks from file?
* Generate UAVs from file?
* Add recharging functionality
* 
* Logic to allow UAVs to start tasks not from first position
*  - Add get position to task object to allow finding nearest
*  - specify UAV position when assigning task and calculate route based upon this //DONE
*/
