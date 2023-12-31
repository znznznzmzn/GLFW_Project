#include "../Framework.h"

void Tween::Interpolate() {
	for (uint i = 0; i < properties.size(); i++) {
		properties[i]->Interpolation(playTime);
		if (properties[i]->IsEnd(playTime)) {
			delete properties[i];
			properties.erase(properties.begin() + i);
			i--;
		}
	}
}
void Tween::CallBack() {
	if (playTime < events.begin()->first) return;
	events.begin()->second();
	events.erase(events.begin());
}

Tween::Tween() { }
Tween::~Tween() { }

void Tween::Add_Property(float* target_value, float init_vlaue, float final_vlaue, 
	float duration, Easing::TYPE ease_type, float delay)
{ properties.emplace_back(new Property_float(target_value, init_vlaue, final_vlaue, duration, ease_type, delay)); }

void Tween::Add_Property(Vector3* target_value, Vector3 init_vlaue, Vector3 final_vlaue, 
	float duration, Easing::TYPE ease_type, float delay)
{ properties.emplace_back(new Property_Vector3(target_value, init_vlaue, final_vlaue, duration, ease_type, delay)); }

void Tween::Add_CallBack(float delay, Event target_event) {
	while (events.count(delay)) delay += 0.000001f;
	events[delay] = target_event;
}

void Tween::Update() {
	if (isPaused) return;
	if (properties.empty() && events.empty()) { // Interpolate할게 없고 && CallBack할 함수가 없다면
		Stop(); // 정지시키고
		return; // 탈주
	}
	playTime += delta;

	if(!properties.empty()) Interpolate();
	if(!events.empty()) CallBack();
}

void Tween::Start() {
	playTime = 0.0f;
	isPaused = false;
}
void Tween::Clear() {
	properties.clear();
	events.clear();
}
void Tween::Stop() { 
	playTime = 0.0f;
	isPaused = true;
	Clear();
}


