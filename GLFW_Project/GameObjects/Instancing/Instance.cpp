#include "../Framework.h"

void Instance::ClearController() {
	for (TransformPointer*& elem : controllers) { SAFE_T_DELETE(elem); }
	controllers.clear();
}

void Instance::CreateInstance() {
	matrices.clear();
	matrices.resize(instance_count);
	ClearController();
	controllers.resize(instance_count);
	for (uint i = 0; i < instance_count; i++) {
		matrices[i] = Matrix(1);
		controllers[i] = new TransformPointer(&matrices[i]);
		controllers[i]->UpdateWorld();
	}
}

void Instance::Arrange_Flat(float gap, Vector3 position, Vector3 scale) {
	const uint stack = static_cast<uint>(ceil(sqrt(static_cast<float>(instance_count))));
	UPoint idx = UPoint(0, 0);

	for (TransformPointer*& elem : controllers) {
		elem->Position().x += idx.x * gap;
		elem->Position().z += idx.y * gap;
		elem->Scale() = scale;
		elem->UpdateWorld();
		idx.x++;
		if (idx.x >= stack) {
			idx.x = 0;
			idx.y++;
		}
	}
}
void Instance::Arrange_3D(float gap, Vector3 position, Vector3 scale) {
	const uint stack = static_cast<uint>(ceil(pow(static_cast<float>(instance_count), 1.0f / 3.0f)));
	UPoint3 idx = UPoint3(0, 0, 0);

	for (TransformPointer*& elem : controllers) {
		elem->Position() = position + idx * gap;
		elem->Scale() = scale;
		elem->UpdateWorld();
		idx.x++;
		if (idx.x >= stack) {
			idx.x = 0;
			idx.z++;
			if (idx.z >= stack) {
				idx.z = 0;
				idx.y++;
			}
		}
	}
}
void Instance::Arrange_RandomArea(Vector3 position, Vector3 area_size) {
	for (TransformPointer*& elem : controllers) {
		elem->Position() = position +
			Vector3(
				Random::Range(-area_size.x, area_size.x),
				Random::Range(-area_size.y, area_size.y),
				Random::Range(-area_size.z, area_size.z));
		elem->Rotation() = Vector3(Random::PI(), Random::PI(), Random::PI());
		elem->UpdateWorld();
	}
}
void Instance::Arrange_RandomRange(Vector3 position, float range) {
	for (TransformPointer*& elem : controllers) {
		elem->Rotation() = Vector3(Random::PI(), Random::PI(), Random::PI()) * 5.0f;
		elem->UpdateWorld();
		elem->Position() = position + elem->Forward() * Random::Length(range);
		elem->UpdateWorld();
	}
}

void Instance::UpdateWorld(int index) {
	if (index == -1) {
		for (TransformPointer*& elem : controllers) elem->UpdateWorld();
	}
	else controllers[index]->UpdateWorld();
}
void Instance::GUIRender_Instance(uint index) {
	controllers[index]->GUIRender_Local();
	controllers[index]->UpdateWorld(); 
}

void Instance::Save() {
	//-
}
void Instance::Load() {
	//-
}
