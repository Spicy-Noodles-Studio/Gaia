#include "MathUtils.h"

double random()
{
	return (double)rand() / (double)RAND_MAX;
}

double random(double from, double to)
{
	return from + random() * (to - from);
}

int random(int from, int to)
{
	return rand() % (to - from) + from;
}
