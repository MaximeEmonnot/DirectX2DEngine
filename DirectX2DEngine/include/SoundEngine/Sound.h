#pragma once

#include <xaudio2.h>
#include <string>

// RIFF file chunks
// More information on : https://learn.microsoft.com/en-us/windows/win32/xaudio2/how-to--load-audio-data-files-in-xaudio2
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
	void		SetVoice(IXAudio2* pAudio);
	void		Pause() const;

	// Finds the chunk in the Audio File using the Resource Interchange File Format (RIFF)
	HRESULT		FindChunk(HANDLE hFile, DWORD fourcc, DWORD& dwChunkSize, DWORD& dwChunkDataPosition);
	// Reads the previously found chunk
	HRESULT		ReadChunkData(HANDLE hFile, void* buffer, DWORD bufferSize, DWORD bufferOffset);

protected:
	IXAudio2SourceVoice*	pSourceVoice;
	WAVEFORMATEXTENSIBLE	wfx = { 0 };
	XAUDIO2_BUFFER			buffer = { 0 };
	Type					soundType = Type::NONE;
};

