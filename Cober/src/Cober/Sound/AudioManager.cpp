#include "pch.h"

#include "AudioManager.h"
#include <stdexcept>
#include <fmod.hpp>
#include <fmod.h>
#include <fmod_errors.h>

namespace Cober {

    AudioManager* AudioManager::instance = 0;

    AudioManager::AudioManager()
    {
        system = nullptr;

        result = FMOD::System_Create(&system);      // Create the main system object.
        if (result != FMOD_OK)
            throw std::runtime_error("FMOD error! (%d) %s\n" + result + std::string(FMOD_ErrorString(result)));

        result = system->init(512, FMOD_INIT_NORMAL, 0);    // Initialize FMOD.
        if (result != FMOD_OK)
            throw std::runtime_error("FMOD error! (%d) %s\n" + result + std::string(FMOD_ErrorString(result)));

        channelGroup = nullptr;
        result = system->createChannelGroup("inGameSoundEffects", &channelGroup);
        if (result != FMOD_OK)
            throw std::runtime_error("FMOD error! (%d) %s\n" + result + std::string(FMOD_ErrorString(result)));

        listenerVelocity = glm::vec3(1.0f);
        listenerUp = glm::vec3(0.0f, 1.0f, 0.0f);
        listenerForward = glm::vec3(0.0f, 0.0f, 1.0f);

        for (int i = 0; i < 32; i++) {
            emisores->SoundPos = glm::vec3(0.0f);
            emisores->SoundVel = glm::vec3(0.0f);
        }

        for (int i = 0; i < 32; i++)
            activo[i] = false;

        system->set3DSettings(1.0f, 10.0f, 1.0f);
    }

    AudioManager* AudioManager::GetInstance()
    {
        if (!instance)
            return nullptr;

        return instance;
    }

    bool AudioManager::SetupInstance()
    {
        if (!instance) {
            instance = new AudioManager();
            return true;
        }
        return false;
    }

    void AudioManager::Clean()
    {
        delete instance;
    }

    AudioManager::~AudioManager()
    {
        channelGroup->release();
        system->close();
        system->release();

    }

    int AudioManager::AddEmisor(const glm::vec3& position, const glm::vec3& velocity)
    {
        int i = 0;

        while (activo[i])
            i++;

        emisores[i].SoundPos = position;
        emisores[i].SoundVel = velocity;

        activo[i] = true;
        return i;
    }

    void AudioManager::RemoveEmisor(int numObj)
    {
        activo[numObj] = false;
        emisores[numObj].SoundPos = glm::vec3(0.0f);
        emisores[numObj].SoundVel = glm::vec3(0.0f);
    }

    void AudioManager::SetVolume(float vol, int nChannel)
    {
        channels[nChannel]->setVolume(vol);
    }

    void AudioManager::PauseChannel(int nChannel)
    {
        bool isPaused;
        channels[nChannel]->getPaused(&isPaused);
        if (isPaused)
            channels[nChannel]->setPaused(false);

        else if (!isPaused)
            channels[nChannel]->setPaused(true);
    }

    void AudioManager::StopChannel(int nChannel)
    {
        channels[nChannel]->stop();
    }

    bool AudioManager::IsPlaying()
    {
        bool isPlaying;
        channelGroup->isPlaying(&isPlaying);
        return isPlaying;
    }

    void AudioManager::Update()
    {
        if (IsPlaying())
            system->update();
    }

    bool AudioManager::IsPlayingChannel(int nChannel)
    {
        bool isPaused;
        channels[nChannel]->getPaused(&isPaused);
        return !isPaused;
    }


    void AudioManager::PlayMusic(const char* path, int nChannel, bool loop)
    {
        FMOD::Sound* sound;

        system->createSound(path, FMOD_CREATESTREAM, nullptr, &sound);

        if (loop)
            sound->setMode(FMOD_LOOP_NORMAL);
        
        result = system->playSound(sound, nullptr, false, &channels[nChannel]);

        if (result != FMOD_OK)
            throw std::runtime_error("FMOD error! (%d) %s\n" + result + std::string(FMOD_ErrorString(result)));
        
        result = channels[nChannel]->setChannelGroup(channelGroup);
        if (result != FMOD_OK)
            throw std::runtime_error("FMOD error! (%d) %s\n" + result + std::string(FMOD_ErrorString(result)));
    }

    FMOD_VECTOR vec3ToFMOD(const glm::vec3& vector) {
        FMOD_VECTOR fvec;
        fvec.x = vector.x;
        fvec.y = vector.y;
        fvec.z = vector.z;
        return fvec;
    }

    void AudioManager::UpdateListener(const glm::vec3& position, const glm::vec3&  velocity, const glm::vec3& forward, const glm::vec3& up)
    {
        listenerVelocity = velocity;
        listenerPos = position;
        listenerForward = forward;
        listenerUp = up;

        system->set3DListenerAttributes(0, &vec3ToFMOD(listenerPos), &vec3ToFMOD(listenerVelocity), &vec3ToFMOD(listenerForward), &vec3ToFMOD(listenerUp));
    }

    void AudioManager::UpdateSound(const glm::vec3& position, const glm::vec3& velocity, int nChannel, int numObj)
    {
        emisores[numObj].SoundPos = position;
        emisores[numObj].SoundVel = velocity;

        channels[nChannel]->set3DAttributes(&vec3ToFMOD(emisores[numObj].SoundPos), &vec3ToFMOD(emisores[numObj].SoundVel));
    }

    void AudioManager::PlaySound(const char* path, int nChannel)
    {

        FMOD::Sound* sound;
        system->createSound(path, FMOD_3D_HEADRELATIVE, nullptr, &sound);

        result = system->playSound(sound, nullptr, false, &channels[nChannel]);


        if (result != FMOD_OK)
            throw std::runtime_error("FMOD error! (%d) %s\n" + result + std::string(FMOD_ErrorString(result)));
        result = channels[nChannel]->setChannelGroup(channelGroup);
        if (result != FMOD_OK)
            throw std::runtime_error("FMOD error! (%d) %s\n" + result + std::string(FMOD_ErrorString(result)));
    }
}