#include "SoundEngine/Sound.h"

#include "SoundEngine/SoundSystem.h"

Sound::Sound(const std::string& path, Type soundType)
	:
	soundType(soundType)
{
#ifdef _XBOX
	char* strFileName = "game:\\" + path;
#else
	const std::string strFileName = path;
#endif

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

	DWORD dwChunkSize;
	DWORD dwChunkPosition;

	FindChunk(hFile, fourccRIFF, dwChunkSize, dwChunkPosition);
	DWORD fileType;
	ReadChunkData(hFile, &fileType, sizeof(DWORD), dwChunkPosition);
	if (fileType != fourccWAVE)
		throw SFX_EXCEPTION("An exception has been caught during Sound Initialization : Wrong file type.", S_FALSE);

	FindChunk(hFile, fourccFMT, dwChunkSize, dwChunkPosition);
	ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);

	FindChunk(hFile, fourccDATA, dwChunkSize, dwChunkPosition);
	BYTE* pDataBuffer = new BYTE[dwChunkSize];
	ReadChunkData(hFile, pDataBuffer, dwChunkSize, dwChunkPosition);

	buffer.AudioBytes = dwChunkSize;
	buffer.pAudioData = pDataBuffer;
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	if (soundType == Sound::Type::MUSIC)
		buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
}

void Sound::SetVoice(IXAudio2* pAudio)
{
	HRESULT hr = S_OK;
	if (FAILED(hr = pAudio->CreateSourceVoice(&pSourceVoice, reinterpret_cast<WAVEFORMATEX*>(&wfx))))
		throw SFX_EXCEPTION("An exception has been caught during Source Voice Creation.", hr);
	if (FAILED(hr = pSourceVoice->SubmitSourceBuffer(&buffer)))
		throw SFX_EXCEPTION("An error exception has been caught during Source Buffer Submission.", hr);
}

void Sound::Pause() const
{
	pSourceVoice->Stop();
}

HRESULT Sound::FindChunk(HANDLE hFile, DWORD fourcc, DWORD& dwChunkSize, DWORD& dwChunkDataPosition)
{
	HRESULT hr = S_OK;
	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN))
		return HRESULT_FROM_WIN32(GetLastError());

	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD bytesRead = 0;
	DWORD dwOffset = 0;

	while (hr == S_OK) {
		DWORD dwRead;
		if (0 == ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL))
			hr = HRESULT_FROM_WIN32(GetLastError());
		if (0 == ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL))
			hr = HRESULT_FROM_WIN32(GetLastError());

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

		if (dwChunkType == fourcc) {
			dwChunkSize = dwChunkDataSize;
			dwChunkDataPosition = dwOffset;
			return S_OK;
		}

		dwOffset += dwChunkDataSize;

		if (bytesRead >= dwRIFFDataSize) return S_FALSE;
	}

	return S_OK;
}

HRESULT Sound::ReadChunkData(HANDLE hFile, void* buffer, DWORD bufferSize, DWORD bufferOffset)
{
	HRESULT hr = S_OK;
	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, bufferOffset, NULL, FILE_BEGIN))
		return HRESULT_FROM_WIN32(GetLastError());
	DWORD dwRead;
	if (0 == ReadFile(hFile, buffer, bufferSize, &dwRead, NULL))
		hr = HRESULT_FROM_WIN32(GetLastError());
	return hr;
}
