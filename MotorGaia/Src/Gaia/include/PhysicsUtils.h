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
	COL_GROUP_1 = (1<<2),
	COL_GROUP_2 = (1<<3),
	COL_GROUP_3 = (1<<4),
	COL_GROUP_4 = (1<<5),
	COL_GROUP_5 = (1<<6),
	COL_GROUP_6 = (1<<7),
	COL_GROUP_7 = (1<<8),
	COL_GROUP_8 = (1<<9),
	COL_GROUP_9 = (1<<10),
	COL_GROUP_10 = (1<<11),
	COL_GROUP_11 = (1<<12),
	COL_GROUP_12 = (1<<13),
	COL_GROUP_13 = (1<<14),
	COL_GROUP_14 = (1<<15),
	ALL = (1 << 16) -1 //Todos los bits hasta 15 -> 1111111111111111
};


#endif 
