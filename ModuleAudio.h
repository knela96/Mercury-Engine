#ifndef __ModuleAudio_H__
#define __ModuleAudio_H__

#include "Module.h"
#include "SDL_mixer\include\SDL_mixer.h"

#define DEFAULT_MUSIC_FADE_TIME 2.0f

enum effects {
	CAR1 = 1,
	CAR2,
	CAR_IDLE1,
	CAR_IDLE2,
	CAR_BACK1,
	CAR_BACK2,
	POWERUP,
	JUMP,
	BOOST,
	TICK,
	GO,
	WHISTLE,
	GOAL,
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

	void ChangeFxVolume(Mix_Chunk * fx, float v_fx);


private:

	Mix_Music*			music;
	p2List<Mix_Chunk*>	fx;
};

#endif // __ModuleAudio_H__