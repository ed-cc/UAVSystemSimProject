#pragma once
#include "Header.h"

class TaskObject
{
public:
	void init();
	void deliveryTask(int currentTime, position currentLocation, position pickUp, position dropOff, position finalLocation, int collectedPayloadMass, UAVType typeAssigned);
	void observationTask(int currentTime, position currentLocation, position taskLocation, position finalLocation, int observationTime, UAVType typeAssigned);
	void relocationTask(int currentTime, position currentLocation, position newLocation, UAVType typeAssigned);
	std::optional<posTime> getNextPosition(int currentTime, position currentLocation);
	void setUAVSpecification(specsUAV setSpecs);
	bool checkAssigned();
	bool checkComplete();
	void setAssigned(bool assigned);
	void resetStartingPosition(position newStart);
	void recalculateFinalPosition(std::vector<position> finalPositions);
	position getFirstRequiredPos();
	position returnableLocation();
	TaskType getTaskType();
	double getTotalDistance();
	bool isGrouped();
	void setGrouping(bool grouping);
	void setCompleteForGrouping();
	std::vector<posTime> getEntirePosTimeList();
	int addTasksForGrouping(std::vector<TaskObject> groupTasks);
	

private:
	//Variables:
	UAVType assignedUAVType = UAVType::any;
	specsUAV assignedSpecification = STANDARDUAV;
	TaskType typeOfTask = TaskType::random;

	std::vector<posTime> locationTimeList; //List of expected locations and time to wait before departure for the current task
	int locTimeListPostion = 0; //Indicates where in the list the current objective is

	int payloadMass = 0;

	bool activeTask = false;

	bool assignedTask = false;

	bool taskComplete = false;

	bool taskGrouped = false;

	position returnableLoc = { 0,0,0 };
	//Functions:
	
};

