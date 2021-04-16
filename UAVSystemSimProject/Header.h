#pragma once
#include <vector>
#include <math.h>
#include <optional>
#include <string>


constexpr auto PI = 3.14159265359;

enum class UAVType
{
	standard,
	heavy,
	light,
	custom,
	any
};

enum class RouteType
{
	direct
};

enum class TaskType
{
	observation, //Travel to point B perform an observation -> return to home
	delivery,    //Travel to point B, collect payload then travel to C -> return to home
	relocation,  //Move UAV to point B
	random       //Select another task type at random
};

enum class UAVActivity
{
	landed, //Waiting on the ground
	hovering, //In flight, not moving
	flying, //Moving through space
	loading, //Waiting for a payload to be loaded or unloaded
	recharging //Waiting for the battery to recharge
};

//Struct to store a 3D position
struct position
{
	double x;
	double y;
	double z;
};

//Store a position and corresponding time
struct posTime
{
	position pos; //3D position
	int time; //Time in ms
};

struct logData
{
	std::string name; //Name of UAV or additional information that we want to store
	position pos; //3D position
	int time; //Time in ms
	UAVActivity currentActivity; //What the UAV is doing at this time
};

struct specsUAV
{
	double cruiseSpeed; //Cruise speed in m/s
	double cruiseAlt; //Cruise altitude in m
	double payloadMass; //Max Payload mass in kg
	int endurance; //Endurance in ms
	int rechargeTime; //Time to recharge in ms
	int loadingTime; //Time to load/unload in ms
};

constexpr specsUAV STANDARDUAV{ 10, 15, 0.5, 45 * 60 * 1000, 120 * 60 * 1000 , 3000};

//Compares positions 1 and 2 -> Returns true if they are identical
bool comparePositions(position posit_1, position posit_2);

//Returns a position vector from pointA to pointB of length length
position get3DVector(position pointA, position pointB, double length);

//Returns the distance in 3D space between pointA and pointB
double get3DDistance(position pointA, position pointB);

//Returns the distance in 2D space between pointA and pointB - ignores z positions
double get2DDistance(position pointA, position pointB);

//Returns UAV specification associated with UAVType
specsUAV getUAVSpecsFromType(UAVType requiredType);

//Finds the nearest of comparisonLocations to startingPos and returns this position along with the distance as a pair
std::pair<double, position> findClosest(position startingPos, std::vector<position> comparisonLocations);