#include "TaskObject.h"

//Creates a blank task object
void TaskObject::init()
{

}

//Must be called before a task setup function if UAVType::custom is used
void TaskObject::setUAVSpecification(specsUAV setSpecs)
{
	assignedSpecification = setSpecs;
}

void TaskObject::deliveryTask(int currentTime, position currentLocation, position pickUp, 
	position dropOff, position finalLocation, int collectedPayloadMass, UAVType typeAssigned)
{
	typeOfTask = TaskType::delivery;
	//Set assigned UAV type
	if (typeAssigned != UAVType::custom)
	{
		assignedUAVType = typeAssigned;
		assignedSpecification = getUAVSpecsFromType(typeAssigned);
	}
	
	
	activeTask = true;
	payloadMass = collectedPayloadMass;

	position startCruise = currentLocation;
	startCruise.z = assignedSpecification.cruiseAlt;
	position pickupCruise = pickUp;
	pickupCruise.z = assignedSpecification.cruiseAlt;
	position dropOffCruise = dropOff;
	dropOffCruise.z = assignedSpecification.cruiseAlt;
	position finalLocationCruise = finalLocation;
	finalLocationCruise.z = assignedSpecification.cruiseAlt;

	locationTimeList.push_back(posTime{ startCruise, 0 });
	locationTimeList.push_back(posTime{ pickupCruise, 0 });
	locationTimeList.push_back(posTime{ pickUp, 0 });
	locationTimeList.push_back(posTime{ pickupCruise, assignedSpecification.loadingTime });
	locationTimeList.push_back(posTime{ dropOffCruise, 0 });
	locationTimeList.push_back(posTime{ dropOff, 0 });
	locationTimeList.push_back(posTime{ dropOffCruise, assignedSpecification.loadingTime });
	locationTimeList.push_back(posTime{ finalLocationCruise, 0 });
	locationTimeList.push_back(posTime{ finalLocation, 0 });
}

void TaskObject::observationTask(int currentTime, position currentLocation, position taskLocation, 
	position finalLocation, int observeTime, UAVType typeAssigned)
{
	typeOfTask = TaskType::observation;
	assignedUAVType = typeAssigned;
	activeTask = true;

	position startCruise = currentLocation;
	startCruise.z = assignedSpecification.cruiseAlt;
	position taskLocationCruise = taskLocation;
	taskLocationCruise.z = assignedSpecification.cruiseAlt;
	position finalLocationCruise = finalLocation;
	finalLocationCruise.z = assignedSpecification.cruiseAlt;

	locationTimeList.push_back(posTime{ startCruise, 0 });
	locationTimeList.push_back(posTime{ taskLocationCruise, 0 });
	locationTimeList.push_back(posTime{ taskLocation, 0 });
	locationTimeList.push_back(posTime{ taskLocationCruise, observeTime });
	locationTimeList.push_back(posTime{ finalLocationCruise, 0 });
	locationTimeList.push_back(posTime{ finalLocation, 0 });
}

void TaskObject::relocationTask(int currentTime, position currentLocation, position newLocation, UAVType typeAssigned)
{
	typeOfTask = TaskType::relocation;
	assignedUAVType = typeAssigned;
	activeTask = true;

	position startCruise = currentLocation;
	startCruise.z = assignedSpecification.cruiseAlt;
	position newLocationCruise = newLocation;
	newLocationCruise.z = assignedSpecification.cruiseAlt;

	locationTimeList.push_back(posTime{ startCruise, 0 });
	locationTimeList.push_back(posTime{ newLocationCruise, 0 });
	locationTimeList.push_back(posTime{ newLocation, 0 });
}

//Returns an optional containing a posTime of the next position and time to wait before departure 
//If this does not contain a value there is no next position required and the task is complete
std::optional<posTime> TaskObject::getNextPosition(int currentTime, position currentLocation)
{
	//posTime outputObj;

	//Check if we have completed the current objective in the location time list
	if (comparePositions(currentLocation, locationTimeList[locTimeListPostion].pos))
	{
		//The current location has been reached
		//Therefore check if we have reached the end of the task list
		if (locTimeListPostion == (locationTimeList.size() - 1))
		{
			//This means that the task is complete
			return std::nullopt;
		}
		else
		{
			//The task is not complete so move to the next position in the list and return it
			locTimeListPostion++;
			return locationTimeList[locTimeListPostion];
		}
	}
	else
	{
		//The expected location has not been reached so return where we want to be
		//Return 0 for waiting time because waiting should only be done when each leg begins
		return posTime{ locationTimeList[locTimeListPostion].pos, 0 };
	}
}

bool TaskObject::checkAssigned()
{
	return assignedTask;
}

void TaskObject::setAssigned(bool assigned = true)
{
	assignedTask = assigned;
}

void TaskObject::resetStartingPosition(position newStart)
{
	if (get2DDistance(locationTimeList[0].pos, newStart) < 0.01)
	{
		//This is the same point so we don't need to change anything
	}
	else
	{
		//A new point has been given so rewrite the first line
		position startCruise = newStart;
		startCruise.z = assignedSpecification.cruiseAlt;
		locationTimeList[0] = posTime{ startCruise, 0 };
	}
	
}

//This returns the first place that the UAV must be at (ignoring the starting position)
position TaskObject::getFirstRequiredPos()
{
	return locationTimeList[2].pos;
}

/*
* Assign a task object to a UAV object
* UAV object update function called every time step
* Moves position of UAV to wherever set by task
* When this position is reached the task object update is called
* This provides a new destination for the UAV object or tells it to wait at the current location
* 
*/