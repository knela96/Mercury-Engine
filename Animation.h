#ifndef ANIMATION_H_
#define ANIMATION_H_


#include "Globals.h"
#include "Application.h"
#include "Keyframe.h"


class Animation
{
public:
	Animation();
	~Animation();

private:
	float                lenght; //seconds
	vector<Keyframe*>    keyframes;

public:
	float GetLength() { return lenght; }
	vector<Keyframe*>* GetKeyframes() { return &keyframes; };
};

Animation::Animation()
{
}

Animation::~Animation()
{
}


#endif