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
//	// �ʱ�ȭ �Լ�
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
//    // WAV ���� �ε�
//    WAVEFORMATEX wfx = {};
//    XAUDIO2_BUFFER buffer = {};
//
//    std::ifstream file(filename, std::ios::binary | std::ios::ate);
//    if (!file.is_open()) {
//        // ������ �� �� ���� ��� ó��
//        return;
//    }
//
//    size_t fileSize = file.tellg();
//    file.seekg(0);
//
//    // WAV ��� �б�
//    WAVEFORMATEXTENSIBLE wfxe = {};
//    file.read(reinterpret_cast<char*>(&wfxe), sizeof(wfxe));
//
//    // WAV ������ �б�
//    BYTE* pDataBuffer = new BYTE[fileSize - sizeof(wfxe)];
//    file.read(reinterpret_cast<char*>(pDataBuffer), fileSize - sizeof(wfxe));
//
//    file.close();
//
//    // WAV ���� ����
//    wfx.wFormatTag = wfxe.Format.wFormatTag;
//    wfx.nChannels = wfxe.Format.nChannels;
//    wfx.nSamplesPerSec = wfxe.Format.nSamplesPerSec;
//    wfx.nAvgBytesPerSec = wfxe.Format.nAvgBytesPerSec;
//    wfx.nBlockAlign = wfxe.Format.nBlockAlign;
//    wfx.wBitsPerSample = wfxe.Format.wBitsPerSample;
//    wfx.cbSize = sizeof(wfxe.Samples);
//
//    // Source Voice ����
//    pXAudio2->CreateSourceVoice(&pSourceVoice, reinterpret_cast<WAVEFORMATEX*>(&wfx));
//
//    // ���� ����
//    buffer.AudioBytes = static_cast<UINT32>(fileSize - sizeof(wfxe));
//    buffer.pAudioData = pDataBuffer;
//    buffer.Flags = XAUDIO2_END_OF_STREAM;
//
//    // ���� ���
//    pSourceVoice->SubmitSourceBuffer(&buffer);
//    pSourceVoice->Start(0);
//
//    // ����� ���� �� Source Voice ����
//    pSourceVoice->FlushSourceBuffers();
//    pSourceVoice->DestroyVoice();
//
//	delete[] pDataBuffer;
//}