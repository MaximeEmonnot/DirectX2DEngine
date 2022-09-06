#include "SoundSystem.h"

// STATIC VARIABLES INITIALIZATION
std::unique_ptr<SoundSystem> SoundSystem::pInstance = nullptr;

SoundSystem::SoundSystem()
{
	HRESULT hr = S_OK;
	if (FAILED(hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED)))
		throw SFX_EXCEPTION("An exception has been caught during COM Initialization.", hr);
	if (FAILED(hr = XAudio2Create(pAudio.GetAddressOf(), 0u, XAUDIO2_DEFAULT_PROCESSOR)))
		throw SFX_EXCEPTION("An exception has been caught during XAudio2 Creation.", hr);
	if (FAILED(hr = pAudio->CreateMasteringVoice(&pMasterVoice)))
		throw SFX_EXCEPTION("An exception has been caught during Mastering Voice Creation.", hr);
}

SoundSystem::~SoundSystem()
{
	pMasterVoice->DestroyVoice();
}

SoundSystem& SoundSystem::GetInstance()
{
	if (!pInstance)
		pInstance = std::make_unique<SoundSystem>();
	return *pInstance;
}

void SoundSystem::AddSong(const std::string& path, bool bIsLooping)
{
	if (soundMap.contains(path))
		throw SFX_EXCEPTION("This song has alreay been added!", S_FALSE);
	if (bIsLooping)
	{
		Sound toAdd(path, Sound::Type::MUSIC);
		toAdd.SetVoice(pAudio.Get());
		soundMap.insert(std::pair(path, toAdd));
	}
	else
		soundMap.insert(std::pair(path, Sound(path, Sound::Type::EFFECT)));
}

void SoundSystem::Play(const std::string& path, int posX) const
{
	if (!soundMap.contains(path))
		throw SFX_EXCEPTION("This song has not been added!\nPlease, add the song first before playing it.", S_FALSE);

	const float pVoiceChannelVolumes[2] = {
		1.0f - (static_cast<float>(posX) / (static_cast<float>(WND.GetWidth()) / 2)),
		1.0f + static_cast<float>(posX) / (static_cast<float>(WND.GetWidth()) / 2)
	};
	Sound toPlay = soundMap.at(path);
	HRESULT hr = S_OK;

	switch (toPlay.soundType) {
	case Sound::Type::EFFECT:
		toPlay.SetVoice(pAudio.Get());
	case Sound::Type::MUSIC:
		if (FAILED(hr = toPlay.pSourceVoice->SetChannelVolumes(2, pVoiceChannelVolumes)))
			throw SFX_EXCEPTION("An error has been caught during Channel Volumes Settings", hr);
		if (FAILED(hr = toPlay.pSourceVoice->Start(0)))
			throw SFX_EXCEPTION("An error has been caught during Source Voice Start", hr);
		break;
	default:
		break;
	}
}

void SoundSystem::Pause(const std::string& path) const
{
	if (!soundMap.contains(path))
		throw SFX_EXCEPTION("This song has not been added!\nPlease, add the song first before playing it.", S_FALSE);
	Sound toPlay = soundMap.at(path);
	if (toPlay.soundType != Sound::Type::MUSIC)
		throw SFX_EXCEPTION("The sound is not a music.", S_FALSE);

	toPlay.Pause();
}

void SoundSystem::SetVolume(const std::string& path, int posX) const
{
	HRESULT hr;
	if (!soundMap.contains(path))
		throw SFX_EXCEPTION("This song has not been added!\nPlease, add the song first before playing it.", S_FALSE);
	Sound toPlay = soundMap.at(path);
	if (toPlay.soundType != Sound::Type::MUSIC)
		throw SFX_EXCEPTION("The sound is not a music.", S_FALSE);
	const float pVoiceChannelVolumes[2] = {
		2.0f - (static_cast<float>(posX) / (static_cast<float>(WND.GetWidth()) / 2)),
		static_cast<float>(posX) / (static_cast<float>(WND.GetWidth()) / 2)
	};
	if (FAILED(hr = toPlay.pSourceVoice->SetChannelVolumes(2, pVoiceChannelVolumes)))
		throw SFX_EXCEPTION("An error has been caught during Channel Volumes Settings", hr);
}
