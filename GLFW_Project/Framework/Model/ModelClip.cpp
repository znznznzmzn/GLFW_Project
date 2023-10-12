#include "../Framework.h"

ModelClip::~ModelClip() { for (auto& elem : keyFrames) { SAFE_DELETE(elem.second); } }

void ModelClip::Init() {
	eventIter = events.begin();
	playTime = 0.0f;
	duration = GetEndTime();
}

void ModelClip::Excute() {
	if (events.empty() || eventIter == events.end()) return;
	if (eventIter->first >= playTime) return;
	eventIter->second();
	eventIter++;
}

KeyFrame* ModelClip::GetKeyFrame(string boneName)
{ return ((keyFrames.count(boneName) == 0) ? nullptr : keyFrames[boneName]); }

void ModelClip::SetEvents(Event event, float time) { if (events.count(time) <= 0) events[time] = event; }
