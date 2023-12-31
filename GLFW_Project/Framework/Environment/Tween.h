#pragma once

class Tween {
private:
	class Property {
	protected:
		float duration = 1.0f;
		Easing::TYPE ease_type = Easing::TYPE::LINEAR;
		float delay = 0.0f;

		float progress = 0.0f;
	public:
		Property(
			float duration, Easing::TYPE ease_type = Easing::TYPE::LINEAR, float delay = 0.0f) :
			duration(duration), ease_type(ease_type), delay(delay), progress(0.0f) {  }
		virtual ~Property() = default;

		virtual void Interpolation(float const& playTime) = 0;
		bool IsEnd(float const& playTime) { return duration + delay < playTime; }
	};

	class Property_float : public Property {
		float* target_value = nullptr;
		float init_vlaue;
		float final_vlaue;

		float range = 1.0f;
	public:
		Property_float(float*& target_value, float init_vlaue, float final_vlaue,
			float duration, Easing::TYPE ease_type = Easing::TYPE::LINEAR, float delay = 0.0f) : 
			target_value(target_value), init_vlaue(init_vlaue), final_vlaue(final_vlaue),
			Property(duration, ease_type, delay), range(final_vlaue - init_vlaue) {}
		~Property_float() 
		{ *target_value = final_vlaue; }

		void Interpolation(float const& playTime) override {
			if (delay > playTime) return;
			progress = (playTime - delay) / duration;
			*target_value = init_vlaue + range * Easing::Type(ease_type, progress);
		}
	};

	class Property_Vector3 : public Property {
		Vector3* target_value = nullptr;
		Vector3 init_vlaue;
		Vector3 final_vlaue;

		Vector3 range;
	public:
		Property_Vector3(Vector3*& target_value, Vector3 init_vlaue, Vector3 final_vlaue,
			float duration, Easing::TYPE ease_type = Easing::TYPE::LINEAR, float delay = 0.0f) :
			target_value(target_value), init_vlaue(init_vlaue), final_vlaue(final_vlaue),
			Property(duration, ease_type, delay), range(final_vlaue - init_vlaue) {}
		~Property_Vector3() { 
			*target_value = final_vlaue; 
		}

		void Interpolation(float const& playTime) override {
			if (delay > playTime) return;
			progress = (playTime - delay) / duration;
			*target_value = init_vlaue + range * Easing::Type(ease_type, progress);
		}
	};

private:
	float playTime = 0.0f;
	map<float, Event> events;
	vector<Property*> properties;

	bool isPaused = false;

	void Interpolate();
	void CallBack();
public:
	Tween();
	~Tween();

	// Start()시 target_value값을 delay init_vlaue에서 final_vlaue까지 duration동한 ease_type에 대한 Easing을 수행, delay = 0.0f
	void Add_Property(float* target_value, float init_vlaue, float final_vlaue,
		float duration, Easing::TYPE ease_type = Easing::TYPE::LINEAR, float delay = 0.0f);
	void Add_Property(Vector3* target_value, Vector3 init_vlaue, Vector3 final_vlaue,
		float duration, Easing::TYPE ease_type = Easing::TYPE::LINEAR, float delay = 0.0f);
	void Add_CallBack(float delay, Event target_event); // Start()시 delay후에 함수 작동

	void Update();

	void Start();
	void Pause()  { isPaused = true ; }
	void Resume() { isPaused = false; }
	void Clear();
	void Stop();

	bool IsPaused() { return isPaused; }
};