#pragma once

#ifndef PHYSICS_UTILS_H
#define PHYSICS_UTILS_H

enum ImpulseMode
{
	IMPULSE, PUSH, TORQUE, TORQUE_TURN
};

enum RB_Shape
{
	BOX_RB_SHAPE, SPHERE_RB_SHAPE
};

//Potencias de 2 hasta 2^15
enum Col_Filters
{
	NONE = -1,
	DEFAULT = (1 << 0),
	ALL = (1 << 15)
};


#endif 
