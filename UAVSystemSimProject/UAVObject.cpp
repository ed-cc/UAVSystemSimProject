#include "UAVObject.h"

void UAVObject::init(int currentTime, position initPos, std::string name, UAVType setType, RouteType routing, bool logging)
{
	startPosition = initPos;
	typeOfUAV = setType;
	available = true;
	isLogging = logging;
	uavName = name;

	if (isLogging)
	{
		logData tempPosT{ uavName, initPos, currentTime, UAVActivity::landed };
		positionLog.push_back(tempPosT);
	}

	uavSpecifications = getUAVSpecsFromType(typeOfUAV);
}

//Must be called to set specsUAV if UAVType::custom is used to initialise the UAV
void UAVObject::setUAVSpecification(specsUAV setSpecs)
{
	uavSpecifications = setSpecs;
}

bool UAVObject::setDestination(position dest)
{
	if (destination.z != -1) 
	{
		destination = dest;
		available = false;
		return true;
	}
	else 
	{
		return false;
	}
}

position UAVObject::getPosition()
{
	return currentPosition;
}

//Adds current Position at currentTime to positionLog vector
bool UAVObject::updatePositionLog(int currentTime)
{
	if (isLogging)
	{
		logData tempPosT{ uavName, currentPosition, currentTime , currentActivity};
		//logData previous = positionLog.back();
		//if (tempPosT.time == previous.time)
		//{
		//	//No time has passed therefore nothing to log
		//	return false;
		//}
		positionLog.push_back(tempPosT);
		return true;
	}
	return false;
}

bool UAVObject::checkAvailability()
{
	return available;
}

UAVType UAVObject::getType()
{
	return typeOfUAV;
}

/* Attempt to assign a task to the UAV
*  Returns 0 if the task is accepted or -1 if rejected
*/
int UAVObject::assignTask(TaskObject assignedTask)
{
	if (available)
	{
		available = false;
		currentTask = assignedTask;
		currentTask.setAssigned(true);
		return 0;
	}
	else return -1;
}

void UAVObject::loopFunc(int currentTime)
{
	//Find the step time to move everything by
	int timeStep = currentTime - previousTime;
	// Check to see if UAV is on a task
	if (!available)
	{
		//UAV is on a task - check if the UAV is waiting
		if (waitingRemaining != 0)
		{
			waitingRemaining -= timeStep; //Decrease waiting time by time increment
			if (waitingRemaining < 0)
			{
				waitingRemaining = 0;
			}
		}
		else
		{
			//UAV should be moving
			//Check we have latest destination
			auto tempDest = currentTask.getNextPosition(currentTime, currentPosition);
			if (tempDest.has_value())
			{
				destination = tempDest.value().pos;
				waitingRemaining += tempDest.value().time;
			}
			//Check it shouldn't now be waiting
			if (waitingRemaining == 0)
			{
				//Check we haven't reached the destination
				if (!comparePositions(currentPosition, destination))
				{
					//UAV is not at the destination yet
					//Therefore make the UAV move towards the destination
					currentActivity = UAVActivity::flying;
					move(currentTime, timeStep);
				}
				else
				{
					std::optional<posTime> nextPos = currentTask.getNextPosition(currentTime, currentPosition);
					if (nextPos.has_value())
					{
						//We have a new position to move to
						destination = nextPos.value().pos;
						waitingRemaining = nextPos.value().time;

						if (waitingRemaining > 0)
						{
							//The UAV is must wait before moving to the next position
							if (currentPosition.z > 0)
							{
								//The UAV is waiting above the ground and therefore hovering
								currentActivity = UAVActivity::hovering;
							}
							else
							{
								//The UAV is waiting on the ground
								//It is still doing a task therefore it must be loading
								currentActivity = UAVActivity::loading;
							}
						}
					}
					else
					{
						//There is no further location to move to
						//The UAV is therefore available for another task
						currentActivity = UAVActivity::landed;
						setAvailable();
					}
				}
			}
		}
	}

	//Update time and log what has happened
	previousTime = currentTime;
	updatePositionLog(currentTime);
}

void UAVObject::move(int currentTime, int timeStep)
{
	//See if UAV is less than one time increment away
	if ((get3DDistance(currentPosition, destination) < (uavSpecifications.cruiseSpeed * (double)timeStep / 1000.0)))
	{
		//Closer than distance moved in one time increment
		currentPosition = destination;
	}
	else
	{
		//Further away therefore move closer by one time increment
		position oneSVector = get3DVector(currentPosition, destination, uavSpecifications.cruiseSpeed);
		position movePosit;
		movePosit.x = currentPosition.x + (oneSVector.x * (double)timeStep / 1000.0);
		movePosit.y = currentPosition.y + (oneSVector.y * (double)timeStep / 1000.0);
		movePosit.z = currentPosition.z + (oneSVector.z * (double)timeStep / 1000.0);
		currentPosition = movePosit;
	}
}

std::vector<logData> UAVObject::getPositionLog()
{
	return positionLog;
}

void UAVObject::setAvailable()
{
	available = true;
	currentTask.setAssigned(false);
	currentActivity = UAVActivity::landed;
}

std::string UAVObject::getName()
{
	return uavName;
}

position UAVObject::getDestination()
{
	return destination;
}

UAVActivity UAVObject::getActivity()
{
	return currentActivity;
}