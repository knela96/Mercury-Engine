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

	string               name;
	float                lenght = 0; //seconds
	int                  keyFrameCount = 0;

	
	vector<Keyframe*>    keyframes; //here we store a list of all bones and keyframes
	std::map<uint, Keyframe*> keyframes_list;

public:
	void SetName(string name_) { name = name_; }
	string GetName() { return name; }
	void setLenght(float lenght_) { lenght = lenght_; }
	float GetLength() { return lenght; }
	vector<Keyframe*>* GetKeyframes() { return &keyframes; };
};

//mirar en qorden de tiempo me pasan los keyframes de los bones

#endif