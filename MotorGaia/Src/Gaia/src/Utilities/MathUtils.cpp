#include "MathUtils.h"

double random()
{
	return ((double)rand()) / (double)RAND_MAX;;
}

double random(double from, double to)
{
	return from + (((double)rand()) / (double)RAND_MAX) * (to - from);
}

int random(int from, int to)
{
	return rand() % (to - from) + from;
}
