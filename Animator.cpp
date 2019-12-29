#include "Application.h"
#include "Animator.h"



Animator::Animator(GameObject * gameobject, Component_Type type) : Component(type, gameobject){}

Animator::~Animator()
{
}

void Animator::Draw()
{
	glDisable(GL_LIGHTING);
	glColor3f(255.0f, 255.0f, 0.0f);
	glLineWidth(600.0f);
	LOGC("/////////////////////////////////////////////");
	if (myAnimatedBody != nullptr && currAnimation != nullptr) {
		Joint* root = myAnimatedBody->GetRootJoint();
		DrawJoints(currAnimation->keyframes_list[AnimationTime], root);
	}
	LOGC("----------------------------------------------");
	glEnable(GL_LIGHTING);
}


void Animator::UpdateAnim()
{
	if (currAnimation != nullptr) {
		
		IncreaseAnimationTime();
		std::map<string, mat4x4> currPose = CalculateCurrAnimationPose();
		ApplyPoseToJoints(currPose, myAnimatedBody->GetRootJoint(), mat4x4());
	
	}
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN){
		doAnimation(0);
	}if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN) {
		doAnimation(1);
	}
}

void Animator::DrawJoints(Keyframe* frame, Joint* root) {

	JointTransform* bone_transform = frame->pose[root->name];

	for (int i = 0; i < root->children.size(); ++i) {
		Joint* child = root->children[i];
		JointTransform* bone_transform2 = frame->pose[child->name];

		glBegin(GL_LINES);
		glVertex3f(bone_transform->Position.x, bone_transform->Position.y, bone_transform->Position.z);
		glVertex3f(bone_transform2->Position.x, bone_transform2->Position.y, bone_transform2->Position.z);
		glEnd();
		LOGC("%s - %s \n", root->name.c_str(), child->name.c_str());

		DrawJoints(frame, child);
	}


		/*	glVertex3f(bone_transform->Position.x, bone_transform->Position.y, bone_transform->Position.z);
			
			if (root->children.size() - 1 == i) {
				glVertex3f(bone_transform->Position.x, bone_transform->Position.y, bone_transform->Position.z);
			}
			else {
				Joint* child2 = root->children[i + 1];
				JointTransform* bone_transform2 = frame->pose[child->name];
				glVertex3f(bone_transform2->Position.x, bone_transform2->Position.y, bone_transform2->Position.z);
			}*/
}

void Animator::doAnimation(uint index) {
	if (index < Animations.size()) {
		AnimationTime = 0;
		currAnimation = Animations.at(index);
		id_anim = index;
	}
}
void Animator::IncreaseAnimationTime() {
	AnimationTime += 1; //need to create a separated stable 30fps system for animation, not good idea to make them depend of the engine framerate;
	if (AnimationTime >= currAnimation->GetLength()) {
		AnimationTime = 0;
	}
}

map<string, mat4x4> Animator::CalculateCurrAnimationPose()
{
	Keyframe frames[2];
	GetPreviousAndNextFrame(frames);
	
	float progression = CalculateProgression(frames[0], frames[1]);
	return InterpolatePoses(&frames[0],&frames[1],progression);
}

void Animator::ApplyPoseToJoints(map<string, mat4x4> currPose, Joint *ParentJoint, mat4x4& Parentmat) {
	mat4x4 currLocalTransform = currPose[ParentJoint->name];
	mat4x4 currTransform = Parentmat * currLocalTransform;


	for (int i = 0; i < ParentJoint->children.size(); ++i) {
		ApplyPoseToJoints(currPose, ParentJoint->children[i], currTransform);
	}
	mat4x4 container = currTransform * ParentJoint->GetInverseBindTransform();
	ParentJoint->SetAnimationTransform(container);
}

void Animator::GetPreviousAndNextFrame(Keyframe* frames) {

	std::map<uint, Keyframe*> allFrames = *currAnimation->GetKeyframes();
	Keyframe* prevFrame = allFrames[0];
	Keyframe* nextFrame = allFrames[0];

	for (int i = 0; i < currAnimation->keyframes_list.size(); i++) {
		nextFrame = allFrames[i];
		if (nextFrame->GetTimePosition() == AnimationTime) {
			break;
		}
		prevFrame = allFrames[i];
	}
	frames[0] = *prevFrame;
	frames[1] = *nextFrame;
	//DrawJoints(Keyframe *prevFrame);
}
float Animator::CalculateProgression(Keyframe prev, Keyframe next) {
	float Totaltime = next.GetTimePosition() - prev.GetTimePosition();
	float currentTime = AnimationTime - prev.GetTimePosition();
	return currentTime / Totaltime;
}

map<string, mat4x4> Animator::InterpolatePoses(Keyframe* prevFrame, Keyframe* nextFrame, float progression){
	map<string, mat4x4> currPose;

	for (std::map<std::string, JointTransform*>::iterator _it = prevFrame->pose.begin(); _it != prevFrame->pose.end(); ++_it) {
		
			JointTransform* prevTransform = prevFrame->pose[_it->first];
			JointTransform* nextTransform = nextFrame->pose[_it->first];


			prevFrame->pose[_it->first] = &prevTransform->Interpolate(prevTransform, nextTransform, progression);
			mat4x4 mat = prevTransform->GetLocalTransform();
			currPose[_it->first] = mat;
	}
	return currPose;
}
 

