#pragma once
#include <iostream>
#include "Math.h"

// Note that in Fatal Frame engine. _wrk will always imply a struct

typedef struct cam_wrk
{
	Vector4 position;
	Vector4 rotation;
	float unknown;
	float unknown;
};

typedef struct plyr_wrk
{

};

// Look for positon and queued action as well as potential future position in here.
typedef struct ene_wrk
{

};

// Assuming there's any data on flying ghost position and queued in here we care about.
typedef struct fly_wrk
{

};

