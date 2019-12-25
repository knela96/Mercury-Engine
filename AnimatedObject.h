#ifndef ANIMATEDOBJECT_H_
#define ANIMATEDOBJECT_H_

#include "Globals.h"
#include "Application.h"
#include "Joint.h"
#include "MeshObject.h"


class AnimatedObject
{
public:
	AnimatedObject();
	~AnimatedObject();

	//obj
	MeshObject AnimatedObj;

	Joint RootJoint;


};



#endif
