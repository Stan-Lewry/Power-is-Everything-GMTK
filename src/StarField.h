/**
----------UNUSED CLASS------------
*/

#ifndef STARFIELD_HEADER
#define STARFIELD_HEADER

#include "Star.h"
#include <vector>

class StarField{
private:
	std::vector<Star*> stars;
public:
	StarField(int layers, int starsPerLayer);
	~StarField();

	void update(float dTime);

};

#endif	// STARFIELD_HEADER