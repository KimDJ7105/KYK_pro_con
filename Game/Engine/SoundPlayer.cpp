#include "pch.h"
#include "SoundPlayer.h"

//SoundPlayer::SoundPlayer()
//{
//	InitializeXAudio2();
//}
//
//SoundPlayer::~SoundPlayer()
//{
//	if (pMasterVoice) {
//		pMasterVoice->DestroyVoice();
//		pMasterVoice = nullptr;
//	}
//	if (pXAudio2) {
//		pXAudio2->Release();
//		pXAudio2 = nullptr;
//	}
//}
//
//void SoundPlayer::InitializeXAudio2()
//{
//	// 초기화 함수
//
//	CoInitializeEx(nullptr, COINIT_MULTITHREADED);
//	XAudio2Create(&pXAudio2);
//	pXAudio2->CreateMasteringVoice(&pMasterVoice);
//}
//
//void SoundPlayer::PlaySound(const wchar_t* filename) 
//{
//    IXAudio2SourceVoice* pSourceVoice = nullptr;
//
//    // WAV 파일 로드
//    WAVEFORMATEX wfx = {};
//    XAUDIO2_BUFFER buffer = {};
//
//    std::ifstream file(filename, std::ios::binary | std::ios::ate);
//    if (!file.is_open()) {
//        // 파일을 열 수 없는 경우 처리
//        return;
//    }
//
//    size_t fileSize = file.tellg();
//    file.seekg(0);
//
//    // WAV 헤더 읽기
//    WAVEFORMATEXTENSIBLE wfxe = {};
//    file.read(reinterpret_cast<char*>(&wfxe), sizeof(wfxe));
//
//    // WAV 데이터 읽기
//    BYTE* pDataBuffer = new BYTE[fileSize - sizeof(wfxe)];
//    file.read(reinterpret_cast<char*>(pDataBuffer), fileSize - sizeof(wfxe));
//
//    file.close();
//
//    // WAV 형식 설정
//    wfx.wFormatTag = wfxe.Format.wFormatTag;
//    wfx.nChannels = wfxe.Format.nChannels;
//    wfx.nSamplesPerSec = wfxe.Format.nSamplesPerSec;
//    wfx.nAvgBytesPerSec = wfxe.Format.nAvgBytesPerSec;
//    wfx.nBlockAlign = wfxe.Format.nBlockAlign;
//    wfx.wBitsPerSample = wfxe.Format.wBitsPerSample;
//    wfx.cbSize = sizeof(wfxe.Samples);
//
//    // Source Voice 생성
//    pXAudio2->CreateSourceVoice(&pSourceVoice, reinterpret_cast<WAVEFORMATEX*>(&wfx));
//
//    // 버퍼 설정
//    buffer.AudioBytes = static_cast<UINT32>(fileSize - sizeof(wfxe));
//    buffer.pAudioData = pDataBuffer;
//    buffer.Flags = XAUDIO2_END_OF_STREAM;
//
//    // 사운드 재생
//    pSourceVoice->SubmitSourceBuffer(&buffer);
//    pSourceVoice->Start(0);
//
//    // 재생이 끝난 후 Source Voice 정리
//    pSourceVoice->FlushSourceBuffers();
//    pSourceVoice->DestroyVoice();
//
//	delete[] pDataBuffer;
//}