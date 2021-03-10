#pragma once
#include "Header.h"
#include "TaskObject.h"

class UAVObject
{
public:
	void init(int currentTime, position initPos, std::string name, UAVType setType = UAVType::standard, RouteType routing = RouteType::direct, bool logging = false);
	bool setDestination(position dest);
	position getPosition();
	//Returns true if available
	bool checkAvailability(); 
	UAVType getType();
	int assignTask(TaskObject assignedTask);
	void loopFunc(int currentTime);
	std::vector<logData> getPositionLog();
	void setUAVSpecification(specsUAV setSpecs);
	std::string getName();
	position getDestination();

private:
	//Variables:
	position startPosition{ 0, 0, 0 };
	position destination{ 0, 0, 0 };
	position currentPosition{ 0, 0, 0 };
	UAVType typeOfUAV = UAVType::any;
	specsUAV uavSpecifications = STANDARDUAV;
	TaskObject currentTask;
	std::vector<logData> positionLog; //Vector containing a list of positions, times and activity
	UAVActivity currentActivity = UAVActivity::landed; //What the UAV is doing at the moment
	std::string uavName = "";
	int remainingEndurance = 0; //Endurance remaining in ms
	bool available = true; //True if not on a mission
	bool isLogging = false;
	int previousTime = 0; //So we know how much time has passed
	bool isLanded = true; //False if UAV is flying
	int waitingRemaining = 0;
	double distanceTravelled = 0;

	//Functions:
	bool updatePositionLog(int currentTime);
	void move(int currentTime, int timeStep); //Moves the UAV from current position to destination by timeStep*cruiseSpeed
	void setAvailable();
};