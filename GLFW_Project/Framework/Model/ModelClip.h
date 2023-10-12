#pragma once

class ModelClip {
	friend class ModelAnimator;
	friend class ClipReader;
private:
	string name = "";
	uint frameCount = 0;
	float tickPerSecond = 0.0f;
	float duration = 0.0f;
	float playTime = 0.0f;
	float endTime = 0.0f;
	unordered_map<string, KeyFrame*> keyFrames;
	map<float, Event> events;
	map<float, Event>::iterator eventIter;
public:
	ModelClip() = default;
	~ModelClip();
	void Init();
	void Excute();

	KeyFrame* GetKeyFrame(string boneName);
	void SetEvents(Event event, float time);
	float GetEndTime() { return frameCount / tickPerSecond; }

	bool isRepeat = true;
};