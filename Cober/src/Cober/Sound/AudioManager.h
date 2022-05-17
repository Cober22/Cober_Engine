#pragma once

#include <stdexcept>
#include <Fmod/fmod.hpp>
#include <Fmod/fmod.h>
#include <Fmod/fmod_errors.h>

#include <glm/glm.hpp>
#include <glm//gtc/matrix_transform.hpp>

namespace FMOD {
	class ChannelGroup;
	class Channel;
	class System;
}

enum FMOD_RESULT;

namespace Cober {

	class AudioManager {
	public:
		//static FMOD::System fmodAudio;
		struct Emisor {
			glm::vec3 SoundPos;
			glm::vec3 SoundVel;
		};

		static AudioManager* GetInstance();
		static bool SetupInstance();
		static void Clean();

		~AudioManager();

		void PlaySound(const char* path, int nChannel);
		void PlayMusic(const char* path, int nChannel, bool loop);

		void PauseChannel(int nChannel);
		void StopChannel(int nChannel);
		void SetVolume(float vol, int nChannel);

		bool IsPlaying();
		bool IsPlayingChannel(int nChannel);

		void Update();
		void UpdateListener(const glm::vec3& position, const glm::vec3& velocity, const glm::vec3& forward, const glm::vec3& up);
		void UpdateSound(const glm::vec3& position, const glm::vec3&  velocity, int nChannel, int numObj);

		int AddEmisor(const glm::vec3& position, const glm::vec3& velocity);
		void RemoveEmisor(int numObj);

	private:
		AudioManager();

		FMOD::System* system;
		FMOD_RESULT result;

		Emisor emisores[32];
		glm::vec3 listenerVelocity, listenerUp, listenerForward, listenerPos;
		bool activo[32];
		FMOD::ChannelGroup* channelGroup;
		FMOD::Channel* channels[32];

		static AudioManager* instance;
	};
}