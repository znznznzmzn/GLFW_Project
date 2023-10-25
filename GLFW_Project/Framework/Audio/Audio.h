#pragma once

// �����ϰ� �Ϲ� ����� �÷��̸� ����
// ��ġ�� ���� ����� �÷��̴� ���������� �������� ��������
// 
// ����μ� .wav���ϸ� ����

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

	static void MuteAll(); // ��ü��Ʈ

	void Play();
	void Stop();
	void Pause();
	void SetVolume(float volume);
	void SetLoop(bool isLoop);
	void Mute() { SetVolume(0); }

	string GetPath() { return path; }
	bool IsLoop() { return loop; }
};

// -���� ������� �ٸ� �÷��̾�� ������ ������� ���
// class AudioPlayer { };
// class AudioClip { };
// class AudioSource { };
// ������
