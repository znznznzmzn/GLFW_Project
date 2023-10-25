#include "../Framework.h"

const DWORD fourccRIFF = 'FFIR'; // 4���� �ڵ�
const DWORD fourccDATA = 'atad';
const DWORD fourccFMT  = ' tmf';
const DWORD fourccWAVE = 'EVAW';
const DWORD fourccXWMA = 'AMWX';
const DWORD fourccDPDS = 'sdpd';

unordered_map<string, Audio*> Audio::audios;

namespace { // private static �Լ� �� ����
	// static ���� ��� cpp������ ����, �ϳ��� �����ص� �Ǵ� ����
	IXAudio2* XAudio2 = nullptr;
	IXAudio2MasteringVoice* MasterVoice = nullptr;
	WAVEFORMATEXTENSIBLE wfx = { 0 };

	// ����� ���� �б⸦ �����ϰ� �� Audio namespace�� private �Լ�
	void ReadAudioInfo(HRESULT& hr, HANDLE& hAudio, DWORD& read) {
		DWORD dwRead;
		if (0 == ReadFile(hAudio, &read, sizeof(DWORD), &dwRead, NULL)) {
			cout << "FATAL_ERORR!! on ReadAudioInfo(HRESULT& hr, HANDLE& hAudio, DWORD& read)" << endl;
			cout << "hr = " << hr << endl;
			cout << "hAudio(file_handle) = " << hAudio << endl;
			cout << "read = " << hAudio << endl;
			cout << "ReadFile == " << 0 << endl;
		}
	}
	HRESULT FindAudio(HANDLE audio_handle, DWORD fourcc, DWORD& dwAudioSize, DWORD& dwAudioDataPosition) {
		HRESULT hr = S_OK;
		if (INVALID_SET_FILE_POINTER == SetFilePointer(audio_handle, 0, NULL, FILE_BEGIN)) {
			cout << "FATAL_ERORR!! on func FindAudio();" << endl;
			cout << "audio_handle = " << audio_handle << endl;
			return S_FALSE;
		}

		DWORD dwAudioType = 0;
		DWORD dwAudioDataSize = 0;
		DWORD dwRIFFDataSize = 0; //  RIFF = ���ҽ� ��ȯ ���� ����
		DWORD dwFileType = 0;
		DWORD bytesRead = 0;
		DWORD dwOffset = 0;

		while (hr == S_OK) { // �����Ͱ� �ùٸ� �������� ����
			ReadAudioInfo(hr, audio_handle, dwAudioType);		// dwAudioType ����� Ÿ�� �޾ƿ���
			ReadAudioInfo(hr, audio_handle, dwAudioDataSize); // dwAudioDataSize ����� ������ ������ �޾ƿ���

			if (dwAudioType == fourccRIFF) { // ����� Ÿ���� RIFF���
				dwRIFFDataSize = dwAudioDataSize; // RIFF ������ ������ = ����� ������ ������
				dwAudioDataSize = 4; // ����� ������ ������ = 4
				ReadAudioInfo(hr, audio_handle, dwFileType); // dwFileType ���� Ÿ�� �޾ƿ���
			}
			else if (INVALID_SET_FILE_POINTER == SetFilePointer(audio_handle, dwAudioDataSize, NULL, FILE_CURRENT)) {
				cout << " FATAL_ERORR!! on func FindAudio();" << endl;
				cout << " While reading audio info" << endl;
				cout << " audio_handle = " << audio_handle << endl;
				cout << " dwAudioDataSize = " << dwAudioDataSize << endl;
				return S_FALSE;
			}
			dwOffset += sizeof(DWORD) * 2; // += 8
			if (dwAudioType == fourcc) { // ����� Ÿ�� == ã������ fourcc ���°� �´ٸ� 
				dwAudioSize = dwAudioDataSize; // ����� ������ = ���� ����� ������
				dwAudioDataPosition = dwOffset; // ����� ���� ��ġ = ������
				return S_OK; // �̻� ��ȯ
			}
			dwOffset += dwAudioDataSize; // dwOffset += ���� ����� ũ��(4)
			if (bytesRead >= dwRIFFDataSize) return S_FALSE; // ������ ã�� ���ߴٸ� -> ���� ��ȯ 
		}
		return S_OK;
	}
	HRESULT ReadAudioData(HANDLE hFile, void* buffer, DWORD buffersize, DWORD bufferoffset) {
		HRESULT hr = S_OK;
		if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, bufferoffset, NULL, FILE_BEGIN)) {
			cout << "FATAL_ERORR! on func ReadAudioData();" << endl;
			cout << "hFile = " << hFile << endl;
			return S_FALSE;
		}
		DWORD dwRead;
		if (0 == ReadFile(hFile, buffer, buffersize, &dwRead, NULL)) hr = S_FALSE;
		return hr;
	}
}

Audio::Audio(const string& path) {
	wstring w_path;
	w_path.assign(path.begin(), path.end());
	// ���� ã��
	HANDLE file_handle = CreateFile( // ������ ���ų� �����
		w_path.c_str(),		// ���ϸ�
		GENERIC_READ,		// �б�� �÷���
		FILE_SHARE_READ,
		NULL,				// ���ȼ��� ���� ����
		OPEN_EXISTING,		// �����ϸ� ����� �÷��� 
		0, NULL);

	if (INVALID_HANDLE_VALUE == file_handle ||
		INVALID_SET_FILE_POINTER == SetFilePointer(file_handle, 0, NULL, FILE_BEGIN)) return;

	// ���� Ÿ�� ����
	DWORD dwAudioSize;
	DWORD dwAudioPosition;
	FindAudio(file_handle, fourccRIFF, dwAudioSize, dwAudioPosition); // ����� ���� ã��
	DWORD filetype;
	ReadAudioData(file_handle, &filetype, sizeof(DWORD), dwAudioPosition); // ã�� ������ ����� ���� �б�
	if (filetype != fourccWAVE) return; // wav������ �ƴ϶�� Ż��

	// (wfx -> WAVEFORMATEXTENSIBLE) ���� �о����
	FindAudio(file_handle, fourccFMT, dwAudioSize, dwAudioPosition);	// ����� ���� ã��
	ReadAudioData(file_handle, &wfx, dwAudioSize, dwAudioPosition);	// ã�� ������ ����� ���� �б�

	// ���ۿ� ���� ������ ���� �޾ƿ���
	FindAudio(file_handle, fourccDATA, dwAudioSize, dwAudioPosition); // ���� ���� �б�(dwAudioSize)
	BYTE* pDataBuffer = new BYTE[dwAudioSize];
	ReadAudioData(file_handle, pDataBuffer, dwAudioSize, dwAudioPosition); // ���� �б�(pDataBuffer)

	// ����� ���� �޾ƿ���
	AudioBuffer.AudioBytes = dwAudioSize; // ����� ���� ����
	AudioBuffer.pAudioData = pDataBuffer; // ����� ����
	AudioBuffer.Flags = XAUDIO2_END_OF_STREAM; // �÷���

	XAudio2->CreateSourceVoice(&SourceVoice, (WAVEFORMATEX*)&wfx); // �޾ƿ� ������ ���� IXAudio2SourceVoice ����
}
Audio::~Audio() {
	SourceVoice->Stop();
	SourceVoice->FlushSourceBuffers();
	SourceVoice->DestroyVoice();
	SAFE_DELETE(AudioBuffer.pAudioData);
	SAFE_DELETE(AudioBuffer.pContext);
}

void Audio::Init() {
	CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);
	XAudio2Create(&XAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
	XAudio2->CreateMasteringVoice(&MasterVoice);
}
Audio* Audio::Load(const string& path) {
	if (audios.count(path) > 0) return audios[path];

	if (Utility::String::GetExtension(path).compare("wav") != 0) return nullptr;

	Audio* result = new Audio(path);
	audios[path] = result;
	return result;
}
void Audio::Unload(const string& path) {
	if (audios.count(path) <= 0) return;
	SAFE_DELETE(audios[path]);
	audios.erase(path);
}
void Audio::Clear() {
	for (auto& elem : audios) { SAFE_DELETE(elem.second); }
	audios.clear();
}
void Audio::MuteAll() { for (auto& elem : audios) elem.second->Mute();  }

void Audio::Play() {
	Audio::SetLoop(loop);
	if (state == CLIP_STATE::PAUSE) { // �Ͻ����� ���̶��
		state = CLIP_STATE::PLAYING;
		SourceVoice->Start(); // resume
		return;
	}
	SourceVoice->Stop(); // �ش� �÷��̾� �ߴ�
	SourceVoice->FlushSourceBuffers(); // ���� Ŭ����
	SourceVoice->SubmitSourceBuffer(&AudioBuffer); // ���� ����
	SourceVoice->SetVolume(volume); //  ���� ����
	SourceVoice->Start(); // ����
}
void Audio::Stop() {
	SourceVoice->Stop();
	SourceVoice->FlushSourceBuffers();
}
void Audio::Pause() {
	if (state == CLIP_STATE::PAUSE) return;
	state = CLIP_STATE::PAUSE;
	SourceVoice->Stop();
}
void Audio::SetVolume(float volume) {
	volume = (volume < 0) ? 0 : volume;
	SourceVoice->SetVolume(this->volume = volume);
}
void Audio::SetLoop(bool isLoop) {
	AudioBuffer.LoopCount = 
		(loop = isLoop) ? 
		XAUDIO2_LOOP_INFINITE :
		XAUDIO2_NO_LOOP_REGION;
}
