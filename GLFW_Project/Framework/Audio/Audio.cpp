#include "../Framework.h"

const DWORD fourccRIFF = 'FFIR'; // 4글자 코드
const DWORD fourccDATA = 'atad';
const DWORD fourccFMT  = ' tmf';
const DWORD fourccWAVE = 'EVAW';
const DWORD fourccXWMA = 'AMWX';
const DWORD fourccDPDS = 'sdpd';

unordered_map<string, Audio*> Audio::audios;

namespace { // private static 함수 및 변수
	// static 선언 대신 cpp에서만 접근, 하나만 존재해도 되는 변수
	IXAudio2* XAudio2 = nullptr;
	IXAudio2MasteringVoice* MasterVoice = nullptr;
	WAVEFORMATEXTENSIBLE wfx = { 0 };

	// 오디오 파일 읽기를 용이하게 할 Audio namespace의 private 함수
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
		DWORD dwRIFFDataSize = 0; //  RIFF = 리소스 교환 파일 형식
		DWORD dwFileType = 0;
		DWORD bytesRead = 0;
		DWORD dwOffset = 0;

		while (hr == S_OK) { // 데이터가 올바른 형태인지 검증
			ReadAudioInfo(hr, audio_handle, dwAudioType);		// dwAudioType 오디오 타입 받아오기
			ReadAudioInfo(hr, audio_handle, dwAudioDataSize); // dwAudioDataSize 오디오 데이터 사이즈 받아오기

			if (dwAudioType == fourccRIFF) { // 오디오 타입이 RIFF라면
				dwRIFFDataSize = dwAudioDataSize; // RIFF 데이터 사이즈 = 오디오 데이터 사이즈
				dwAudioDataSize = 4; // 오디오 데이터 사이즈 = 4
				ReadAudioInfo(hr, audio_handle, dwFileType); // dwFileType 파일 타입 받아오기
			}
			else if (INVALID_SET_FILE_POINTER == SetFilePointer(audio_handle, dwAudioDataSize, NULL, FILE_CURRENT)) {
				cout << " FATAL_ERORR!! on func FindAudio();" << endl;
				cout << " While reading audio info" << endl;
				cout << " audio_handle = " << audio_handle << endl;
				cout << " dwAudioDataSize = " << dwAudioDataSize << endl;
				return S_FALSE;
			}
			dwOffset += sizeof(DWORD) * 2; // += 8
			if (dwAudioType == fourcc) { // 오디오 타입 == 찾으려는 fourcc 형태가 맞다면 
				dwAudioSize = dwAudioDataSize; // 오디오 사이즈 = 읽은 오디오 사이즈
				dwAudioDataPosition = dwOffset; // 오디오 정보 위치 = 오프셋
				return S_OK; // 이상무 반환
			}
			dwOffset += dwAudioDataSize; // dwOffset += 읽은 오디오 크기(4)
			if (bytesRead >= dwRIFFDataSize) return S_FALSE; // 끝까지 찾지 못했다면 -> 실패 반환 
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
	// 파일 찾기
	HANDLE file_handle = CreateFile( // 파일을 열거나 만든다
		w_path.c_str(),		// 파일명
		GENERIC_READ,		// 읽기용 플래그
		FILE_SHARE_READ,
		NULL,				// 보안성에 대한 설명서
		OPEN_EXISTING,		// 존재하면 열라는 플래그 
		0, NULL);

	if (INVALID_HANDLE_VALUE == file_handle ||
		INVALID_SET_FILE_POINTER == SetFilePointer(file_handle, 0, NULL, FILE_BEGIN)) return;

	// 파일 타입 검증
	DWORD dwAudioSize;
	DWORD dwAudioPosition;
	FindAudio(file_handle, fourccRIFF, dwAudioSize, dwAudioPosition); // 오디오 파일 찾기
	DWORD filetype;
	ReadAudioData(file_handle, &filetype, sizeof(DWORD), dwAudioPosition); // 찾은 파일의 오디오 정보 읽기
	if (filetype != fourccWAVE) return; // wav파일이 아니라면 탈출

	// (wfx -> WAVEFORMATEXTENSIBLE) 정보 읽어오기
	FindAudio(file_handle, fourccFMT, dwAudioSize, dwAudioPosition);	// 오디오 파일 찾기
	ReadAudioData(file_handle, &wfx, dwAudioSize, dwAudioPosition);	// 찾은 파일의 오디오 정보 읽기

	// 버퍼에 대한 정보와 버퍼 받아오기
	FindAudio(file_handle, fourccDATA, dwAudioSize, dwAudioPosition); // 버퍼 길이 읽기(dwAudioSize)
	BYTE* pDataBuffer = new BYTE[dwAudioSize];
	ReadAudioData(file_handle, pDataBuffer, dwAudioSize, dwAudioPosition); // 버퍼 읽기(pDataBuffer)

	// 오디오 버퍼 받아오기
	AudioBuffer.AudioBytes = dwAudioSize; // 오디오 버퍼 길이
	AudioBuffer.pAudioData = pDataBuffer; // 오디오 버퍼
	AudioBuffer.Flags = XAUDIO2_END_OF_STREAM; // 플래그

	XAudio2->CreateSourceVoice(&SourceVoice, (WAVEFORMATEX*)&wfx); // 받아온 정보를 토대로 IXAudio2SourceVoice 생성
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
	if (state == CLIP_STATE::PAUSE) { // 일시정지 중이라면
		state = CLIP_STATE::PLAYING;
		SourceVoice->Start(); // resume
		return;
	}
	SourceVoice->Stop(); // 해당 플레이어 중단
	SourceVoice->FlushSourceBuffers(); // 버퍼 클리어
	SourceVoice->SubmitSourceBuffer(&AudioBuffer); // 버퍼 삽입
	SourceVoice->SetVolume(volume); //  볼륨 설정
	SourceVoice->Start(); // 시작
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
