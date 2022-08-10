#pragma once
#include "EngineException.h"
#include "Window.h"
#include "Sound.h"
#include <xaudio2.h>
#include <wrl.h>
#include <map>

#define SFX

#define SFX_EXCEPTION(note, hr) SoundSystem::SoundEffectsException("DirectX 2D Engine - Sound Engine Exception", note, __FILE__, __LINE__, hr)

class SoundSystem
{
public:
	class SoundEffectsException : public EngineException
	{
	public:
		SoundEffectsException(const std::string& type, const std::string& note, const std::string& file, unsigned int line, HRESULT hr) noexcept
			:
			EngineException(type, note, file, line),
			hr(hr)
		{
			std::stringstream ss;
			ss << std::hex << hr;
			infos = GetNote() + "\nDetails:\n" + GetLocation() + "\nError code : " + ss.str();
		}

		const char* what() const override
		{
			return infos.c_str();
		}
	private:
		HRESULT hr;
	};
public:
	SoundSystem();
	SoundSystem(const SoundSystem&) = delete;
	SoundSystem& operator=(const SoundSystem&) = delete;
	~SoundSystem();

	static SoundSystem& GetInstance();

	void AddSong(const std::string& path, bool bIsLooping);

	void Play(const std::string& path, int posX = 0) const;
	void Pause(const std::string& path) const;
	void SetVolume(const std::string& path, int posX = 0) const;

private:
	static std::unique_ptr<SoundSystem> pInstance;

	Microsoft::WRL::ComPtr<IXAudio2> pAudio;
	IXAudio2MasteringVoice* pMasterVoice;

	std::map<std::string, Sound> soundMap;
};

