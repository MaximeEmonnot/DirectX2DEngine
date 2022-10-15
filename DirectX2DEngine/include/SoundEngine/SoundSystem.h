#pragma once
#include "DebugEngine/EngineException.h"
#include "CoreEngine/Window.h"
#include "Sound.h"
#include <xaudio2.h>
#include <wrl.h>
#include <map>

#define SFX SoundSystem::GetInstance()
#define SFX_EXCEPTION(note, hr) SoundSystem::SoundEffectsException("DirectX 2D Engine - Sound Engine Exception", note, __FILE__, __LINE__, hr)

class SoundSystem
{
public:
	class SoundEffectsException : public EngineException
	{
	public:
		SoundEffectsException(const std::string& type, const std::string& note, const std::string& file, unsigned int line, HRESULT hr) noexcept
			:
			EngineException(type, note, file, line)
		{
			std::stringstream ss;
			ss << std::hex << hr;
			infos = GetNote() + "\nDetails:\n" + GetLocation() + "\nError code : " + ss.str();
		}
	};
public:
	SoundSystem();
	SoundSystem(const SoundSystem&) = delete;
	SoundSystem& operator=(const SoundSystem&) = delete;
	~SoundSystem();

	static SoundSystem& GetInstance();

	// Adds a non-existing song to the System, defining it as an Effect or a Music based on the bIsLooping value. A Music is a song that can be looped, whereas an Effect can't be looped.
	void AddSong(const std::string& path, bool bIsLooping);

	// Plays a song using the path, only if the song has been added first. The posX variable is used for audio spatialization, for example a jukebox in the scene playing on the left part of the screen.
	void Play(const std::string& path, int posX = 0) const;

	// Pauses a Music currently playing. Doesn't work with Effects.
	void Pause(const std::string& path) const;

	// Defines the Audio Spatialization for a Music currently playing.
	void SetVolume(const std::string& path, int posX = 0) const;

private:
	static std::unique_ptr<SoundSystem> pInstance;

	Microsoft::WRL::ComPtr<IXAudio2> pAudio;
	IXAudio2MasteringVoice* pMasterVoice;

	std::map<std::string, Sound> soundMap;
};

