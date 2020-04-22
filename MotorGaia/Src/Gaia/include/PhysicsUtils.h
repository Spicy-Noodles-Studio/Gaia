#pragma once

#ifndef PHYSICS_UTILS_H
#define PHYSICS_UTILS_H

enum ImpulseMode
{
	IMPULSE, PUSH, TORQUE, TORQUE_TURN
};

enum RB_Shape
{
	BOX_RB_SHAPE, SPHERE_RB_SHAPE, CAPSULE_RB_SHAPE, CYLINDER_RB_SHAPE, CONE_RB_SHAPE
};

//Potencias de 2 hasta 2^15
enum Col_Filters
{
	NONE = -1,
	DEFAULT = (1 << 0),// 0000000000000001
	IGNORE_RAYCAST = (1 << 1),// 0000000000000010
	ALL = (1 << 16) -1 //Todos los bits hasta 15 -> 1111111111111111
};


#endif 
