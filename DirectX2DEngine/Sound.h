#pragma once
#include <xaudio2.h>
#include <string>

#ifdef _XBOX //Big-Endian
#define fourccRIFF 'RIFF'
#define fourccDATA 'data'
#define fourccFMT 'fmt '
#define fourccWAVE 'WAVE'
#define fourccXWMA 'XWMA'
#define fourccDPDS 'dpds'
#endif

#ifndef _XBOX //Little-Endian
#define fourccRIFF 'FFIR'
#define fourccDATA 'atad'
#define fourccFMT ' tmf'
#define fourccWAVE 'EVAW'
#define fourccXWMA 'AMWX'
#define fourccDPDS 'sdpd'
#endif

class Sound
{
private:
	friend class SoundSystem;
private:
	enum class Type {
		MUSIC,
		EFFECT,
		NONE
	};
public:
	Sound(const std::string& path, Type soundType);
	~Sound() = default;

private:
	void SetVoice(IXAudio2* pAudio);
	void Pause() const;

	HRESULT FindChunk(HANDLE hFile, DWORD fourcc, DWORD& dwChunkSize, DWORD& dwChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void* buffer, DWORD bufferSize, DWORD bufferOffset);

protected:
	IXAudio2SourceVoice* pSourceVoice;
	WAVEFORMATEXTENSIBLE wfx = { 0 };
	XAUDIO2_BUFFER buffer = { 0 };
	Type soundType = Type::NONE;
};

