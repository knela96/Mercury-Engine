#ifndef __ModuleAudio_H__
#define __ModuleAudio_H__

#include "Module.h"

#define DEFAULT_MUSIC_FADE_TIME 2.0f

enum effects {

};

class ModuleAudio : public Module
{
public:

	ModuleAudio(Application* app, bool start_enabled = true);
	~ModuleAudio();

	bool Init();
	bool CleanUp();

	// Play a music file
	bool PlayMusic(const char* path, float fade_time = DEFAULT_MUSIC_FADE_TIME);

	// Load a WAV in memory
	unsigned int LoadFx(const char* path);

	// Play a previously loaded WAV
	bool PlayFx(unsigned int fx, int repeat = 0, int channel = -1);

	bool playingFX(int channel);

	void StopMusic();

	void StopFx(int channel = -1);

	void ChangeMusicVolume(float v_music);

	//void ChangeFxVolume(Mix_Chunk * fx, float v_fx);


private:
};

#endif // __ModuleAudio_H__