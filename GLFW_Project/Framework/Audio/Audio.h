#pragma once

// 간단하게 일반 오디오 플레이만 구현
// 위치에 따른 오디오 플레이는 복잡해진다 이정도만 구현하자
// 
// 현재로선 .wav파일만 지원

class Audio {
public:
	enum class CLIP_STATE { STOP, PLAYING, PAUSE };
protected:
	static unordered_map<string, Audio*> audios;

	string path; // Key
	float volume = 1.0f;
	bool  loop = false;
	CLIP_STATE state = CLIP_STATE::STOP;

	IXAudio2SourceVoice* SourceVoice = nullptr;
	XAUDIO2_BUFFER       AudioBuffer = { 0 };

	Audio(const string& path);
	~Audio();
public:
	static void Init();
	static Audio* Load(const string& path);
	static void Unload(const string& path);
	static void Clear();

	static void MuteAll(); // 전체뮤트

	void Play();
	void Stop();
	void Pause();
	void SetVolume(float volume);
	void SetLoop(bool isLoop);
	void Mute() { SetVolume(0); }

	string GetPath() { return path; }
	bool IsLoop() { return loop; }
};

// -같은 오디오의 다른 플레이어로 동일한 오디오를 재생
// class AudioPlayer { };
// class AudioClip { };
// class AudioSource { };
// 다음에
