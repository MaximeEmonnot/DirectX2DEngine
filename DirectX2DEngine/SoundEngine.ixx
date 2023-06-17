module;
#include <xaudio2.h>
#include <wrl.h>

export module SoundEngine;

// RIFF file chunks
// More information on : https://learn.microsoft.com/en-us/windows/win32/xaudio2/how-to--load-audio-data-files-in-xaudio2
#ifdef _XBOX
constexpr int fourccRIFF = 'RIFF';
constexpr int fourccDATA = 'data';
constexpr int fourccFMT = 'fmt ';
constexpr int fourccWAVE = 'WAVE';
constexpr int fourccXWMA = 'XWMA';
constexpr int fourccDPDS = 'dpds';
#endif

#ifndef _XBOX
constexpr int fourccRIFF = 'FFIR';
constexpr int fourccDATA = 'atad';
constexpr int fourccFMT = ' tmf';
constexpr int fourccWAVE = 'EVAW';
constexpr int fourccXWMA = 'AMWX';
constexpr int fourccDPDS = 'sdpd';
#endif

import <map>;
import <string>;
import <sstream>;

import CoreEngine;
import DebugEngine;

export class SoundEffectsException : public EngineException
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
export SoundEffectsException SFX_EXCEPTION(const std::string& note, HRESULT hr)
{
	return SoundEffectsException("DirectX 2D Engine - Sound Engine Exception", note, __FILE__, __LINE__, hr);
}

export
class Sound
{
public:
	enum class Type {
		MUSIC,
		EFFECT,
		NONE
	};
public:
	Sound(const std::string& path, Type soundType)
		:
		soundType(soundType)
	{
#ifdef _XBOX
		char* strFileName = "game:\\" + path;
#else
		const std::string strFileName = path;
#endif

		// We open the audio file

		HANDLE hFile = CreateFileA(
			strFileName.c_str(),
			GENERIC_READ,
			FILE_SHARE_READ,
			nullptr,
			OPEN_EXISTING,
			0,
			nullptr
		);

		if (INVALID_HANDLE_VALUE == hFile)
			throw SFX_EXCEPTION("An exception has been caught during Sound Initialization : Invalid handle value.", HRESULT_FROM_WIN32(GetLastError()));
		if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, nullptr, FILE_BEGIN))
			throw SFX_EXCEPTION("An exception has been caught during Sound Initialization : Invalid Set File Pointer.", HRESULT_FROM_WIN32(GetLastError()));

		// We locate the RIFF chunk and check the audio file type (WAVE format)

		DWORD dwChunkSize;
		DWORD dwChunkPosition;
		FindChunk(hFile, fourccRIFF, dwChunkSize, dwChunkPosition);
		DWORD fileType;
		ReadChunkData(hFile, &fileType, sizeof(DWORD), dwChunkPosition);
		if (fileType != fourccWAVE)
			throw SFX_EXCEPTION("An exception has been caught during Sound Initialization : Wrong file type.", S_FALSE);

		// We locate the FMT chunk and store its contents into a WAVEFORMATEXTENSIBLE structure
		// Basically the FMT chunk contains the format header for the audio file

		FindChunk(hFile, fourccFMT, dwChunkSize, dwChunkPosition);
		ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);

		// We locate the DATA chunk and store its contents into a buffer
		// The DATA chunk contains the audio data that will be played when passed to a source voice 

		FindChunk(hFile, fourccDATA, dwChunkSize, dwChunkPosition);
		BYTE* pDataBuffer = new BYTE[dwChunkSize];
		ReadChunkData(hFile, pDataBuffer, dwChunkSize, dwChunkPosition);

		// Finally, we populated a XAUDIO2_BUFFER structure, and define whether we loop the song or not

		buffer.AudioBytes = dwChunkSize;
		buffer.pAudioData = pDataBuffer;
		buffer.Flags = XAUDIO2_END_OF_STREAM;
		if (soundType == Sound::Type::MUSIC)
			buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
	}
	~Sound() = default;

	void		SetVoice(IXAudio2* pAudio)
	{
		HRESULT hr = S_OK;
		if (FAILED(hr = pAudio->CreateSourceVoice(&pSourceVoice, reinterpret_cast<WAVEFORMATEX*>(&wfx))))
			throw SFX_EXCEPTION("An exception has been caught during Source Voice Creation.", hr);
		if (FAILED(hr = pSourceVoice->SubmitSourceBuffer(&buffer)))
			throw SFX_EXCEPTION("An error exception has been caught during Source Buffer Submission.", hr);
	}
	void		Pause() const
	{
		pSourceVoice->Stop();
	}

	// Finds the chunk in the Audio File using the Resource Interchange File Format (RIFF)
	HRESULT		FindChunk(HANDLE hFile, DWORD fourcc, DWORD& dwChunkSize, DWORD& dwChunkDataPosition)
	{
		HRESULT hr = S_OK;
		// We initialize the file pointer at the beginning of the file
		if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN))
			return HRESULT_FROM_WIN32(GetLastError());

		DWORD dwChunkType;
		DWORD dwChunkDataSize;
		DWORD dwRIFFDataSize = 0;
		DWORD dwFileType;
		DWORD bytesRead = 0;
		DWORD dwOffset = 0;

		// Until we reach the desired chunk...

		while (hr == S_OK) {
			// ... we read the current chunk type and size
			DWORD dwRead;
			if (0 == ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL))
				hr = HRESULT_FROM_WIN32(GetLastError());
			if (0 == ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL))
				hr = HRESULT_FROM_WIN32(GetLastError());

			// If we are in the RIFF chunk, we check if the data is readable, else we check if we can move the file pointer
			switch (dwChunkType) {
			case fourccRIFF:
				dwRIFFDataSize = dwChunkDataPosition;
				dwChunkDataSize = 4;
				if (0 == ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL))
					hr = HRESULT_FROM_WIN32(GetLastError());
				break;
			default:
				if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT))
					return HRESULT_FROM_WIN32(GetLastError());
			}

			dwOffset += sizeof(DWORD) * 2;

			// When the desired chunk is reached, we return the chunk's size and position
			if (dwChunkType == fourcc) {
				dwChunkSize = dwChunkDataSize;
				dwChunkDataPosition = dwOffset;
				return S_OK;
			}

			dwOffset += dwChunkDataSize;

			// If the RIFF data size is negative, we return false
			if (bytesRead >= dwRIFFDataSize) return S_FALSE;
		}

		return S_OK;
	}
	// Reads the previously found chunk
	HRESULT		ReadChunkData(HANDLE hFile, void* buffer, DWORD bufferSize, DWORD bufferOffset)
	{
		HRESULT hr = S_OK;
		// We adjust the file pointer to the beginning of the chunk section ...
		if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, bufferOffset, NULL, FILE_BEGIN))
			return HRESULT_FROM_WIN32(GetLastError());
		// ... then we read all the data in the chunk section and store it in a buffer
		DWORD dwRead;
		if (0 == ReadFile(hFile, buffer, bufferSize, &dwRead, NULL))
			hr = HRESULT_FROM_WIN32(GetLastError());
		return hr;
	}

	IXAudio2SourceVoice* pSourceVoice;
	Type					soundType = Type::NONE;
protected:
	WAVEFORMATEXTENSIBLE	wfx = { 0 };
	XAUDIO2_BUFFER			buffer = { 0 };
};

export
class SoundSystem
{
public:
	SoundSystem()
	{
		HRESULT hr = S_OK;
		if (FAILED(hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED)))
			throw SFX_EXCEPTION("An exception has been caught during COM Initialization.", hr);
		if (FAILED(hr = XAudio2Create(pAudio.GetAddressOf(), 0u, XAUDIO2_DEFAULT_PROCESSOR)))
			throw SFX_EXCEPTION("An exception has been caught during XAudio2 Creation.", hr);
		if (FAILED(hr = pAudio->CreateMasteringVoice(&pMasterVoice)))
			throw SFX_EXCEPTION("An exception has been caught during Mastering Voice Creation.", hr);
	}
	SoundSystem(const SoundSystem&) = delete;
	SoundSystem& operator=(const SoundSystem&) = delete;
	~SoundSystem()
	{
		pMasterVoice->DestroyVoice();
	}

	static SoundSystem& GetInstance()
	{
		if (!pInstance)
			pInstance = std::make_unique<SoundSystem>();
		return *pInstance;
	}

	// Adds a non-existing song to the System, defining it as an Effect or a Music based on the bIsLooping value. A Music is a song that can be looped, whereas an Effect can't be looped.
	void					AddSong(const std::string& path, bool bIsLooping)
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

	// Plays a song using the path, only if the song has been added first. The posX variable is used for audio spatialization, for example a jukebox in the scene playing on the left part of the screen.
	void					Play(const std::string& path, int posX = 0) const
	{
		if (!soundMap.contains(path))
			throw SFX_EXCEPTION("This song has not been added!\nPlease, add the song first before playing it.", S_FALSE);

		// Setup Left and Right Channel Volumes based on the X-position 
		const float pVoiceChannelVolumes[2] = {
			1.0f - (static_cast<float>(posX) / (static_cast<float>(WND.GetWidth()) / 2)),
			1.0f + static_cast<float>(posX) / (static_cast<float>(WND.GetWidth()) / 2)
		};
		Sound toPlay = soundMap.at(path);
		HRESULT hr = S_OK;

		// Effects can be played multiple times simultaneously, Musics are played only once
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

	// Pauses a Music currently playing. Doesn't work with Effects.
	void					Pause(const std::string& path) const
	{
		if (!soundMap.contains(path))
			throw SFX_EXCEPTION("This song has not been added!\nPlease, add the song first before playing it.", S_FALSE);
		const Sound toPlay = soundMap.at(path);
		if (toPlay.soundType != Sound::Type::MUSIC)
			throw SFX_EXCEPTION("The sound is not a music.", S_FALSE);
		toPlay.Pause();
	}

	// Defines the Audio Spatialization for a Music currently playing.
	void					SetVolume(const std::string& path, int posX = 0) const
	{
		HRESULT hr;
		if (!soundMap.contains(path))
			throw SFX_EXCEPTION("This song has not been added!\nPlease, add the song first before playing it.", S_FALSE);
		const Sound toPlay = soundMap.at(path);
		if (toPlay.soundType != Sound::Type::MUSIC)
			throw SFX_EXCEPTION("The sound is not a music.", S_FALSE);
		const float pVoiceChannelVolumes[2] = {
			2.0f - (static_cast<float>(posX) / (static_cast<float>(WND.GetWidth()) / 2)),
			static_cast<float>(posX) / (static_cast<float>(WND.GetWidth()) / 2)
		};
		if (FAILED(hr = toPlay.pSourceVoice->SetChannelVolumes(2, pVoiceChannelVolumes)))
			throw SFX_EXCEPTION("An error has been caught during Channel Volumes Settings", hr);
	}

private:
	static std::unique_ptr<SoundSystem>		pInstance;

	Microsoft::WRL::ComPtr<IXAudio2>		pAudio;
	IXAudio2MasteringVoice* pMasterVoice;

	std::map<std::string, Sound>			soundMap;
};

// STATIC VARIABLES INITIALIZATION
std::unique_ptr<SoundSystem> SoundSystem::pInstance = nullptr;

export SoundSystem& SFX = SoundSystem::GetInstance();