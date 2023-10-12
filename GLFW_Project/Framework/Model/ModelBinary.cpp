#include "../../Framework.h"

void ModelReader::ReadMaterial(ModelData*& out) {
	string path = "Assets/Models/BinaryModels/" + out->model_name + "/" + out->model_name + ".mats"; 
	BinaryReader* r = new BinaryReader(path);

	if (not r->isFileOpened()) {
		cout << "Warning! " + out->model_name + ".mats file not exist" << endl;
		delete r;
		return;
	}

	out->materials.resize(Utility::String::ToUint(r->ReadLine()));
	for (Material*& elem : out->materials) {
		elem = new Material();
		elem->LoadMaterial(r->ReadLine()); 
	}

	SAFE_DELETE(r); 
}
void ModelReader::ReadMesh(ModelData*& out) {
	string path = "Assets/Models/BinaryModels/" + out->model_name + "/" + out->model_name + ".mesh";
	BinaryReader* r = new BinaryReader(path);

	if (not r->isFileOpened()) {
		cout << "Warning! " + out->model_name + ".mats file not exist" << endl;
		delete r;
		return;
	}

	uint size = Utility::String::ToUint(r->ReadLine());

	out->meshes.resize(size);
	for (MeshData*& elem : out->meshes) {
		elem = new MeshData();
		elem->name = r->ReadLine();
		elem->meterialIndex = Utility::String::ToUint(r->ReadLine());

		elem->vertices.resize(Utility::String::ToUint(r->ReadLine()));
		r->ReadByte(elem->vertices.data(), sizeof(ModelVertex) * elem->vertices.size());

		elem->indices.resize(Utility::String::ToUint(r->ReadLine()));
		r->ReadByte(elem->indices.data(), sizeof(uint) * elem->indices.size());
	}

	out->nodes.resize(Utility::String::ToUint(r->ReadLine()));
	for (NodeData*& elem : out->nodes) {
		elem = new NodeData();
		elem->name = r->ReadLine();
		elem->index = Utility::String::ToInt(r->ReadLine());
		elem->parent = Utility::String::ToInt(r->ReadLine());
		elem->transform = Utility::String::ToMatrix(r->ReadLine());
	}

	out->bones.resize(Utility::String::ToInt(r->ReadLine()));
	for (BoneData*& elem : out->bones) {
		elem = new BoneData();
		elem->name = r->ReadLine();
		elem->index = Utility::String::ToInt(r->ReadLine());
		elem->offset = Utility::String::ToMatrix(r->ReadLine());

		out->boneMap[elem->name] = elem->index;
	}

	delete r;
}
ModelData* ModelReader::ReadModel(string model_name) {
	ModelData* result = new ModelData();
	result->model_name = model_name;
	ReadMesh(result);
	ReadMaterial(result);
	return result;
}

void ModelWriter::WriteMaterial(ModelData*& in_data) {
	string savePath = "Assets/Models/BinaryModels/" + in_data->model_name + "/";
	string file = in_data->model_name + ".mats";

	filesystem::create_directories(savePath);

	BinaryWriter* w = new BinaryWriter(savePath + file);

	w->WriteLine(Utility::String::From(in_data->materials.size()));
	for (Material* elem : in_data->materials) {
		string path = savePath + elem->GetName() + ".mat";
		elem->SaveMaterial(path);
		w->WriteLine(path);
	}

	delete w;
}
void ModelWriter::WriteMesh(ModelData*& in_data) {
	string savePath = "Assets/Models/BinaryModels/" + in_data->model_name + "/";
	string file = in_data->model_name + ".mesh";

	filesystem::create_directories(savePath);

	BinaryWriter* w = new BinaryWriter(savePath + file);

	w->WriteLine(to_string(in_data->meshes.size()));
	for (MeshData* elem : in_data->meshes) {
		w->WriteLine(elem->name);
		w->WriteLine(to_string(elem->meterialIndex));

		w->WriteLine(to_string(elem->vertices.size()));
		w->WriteByte(elem->vertices.data(), sizeof(ModelVertex) * elem->vertices.size());

		w->WriteLine(to_string(elem->indices.size()));
		w->WriteByte(elem->indices.data(), sizeof(uint) * elem->indices.size());
	}

	w->WriteLine(to_string(in_data->nodes.size()));
	for (NodeData* elem : in_data->nodes) {
		w->WriteLine(elem->name);
		w->WriteLine(to_string(elem->index));
		w->WriteLine(to_string(elem->parent));
		w->WriteLine(Utility::String::From(elem->transform));
	}

	w->WriteLine(Utility::String::From(in_data->bones.size()));
	for (BoneData* elem : in_data->bones) {
		w->WriteLine(elem->name);
		w->WriteLine(to_string(elem->index));
		w->WriteLine(Utility::String::From(elem->offset));
	}

	delete w;
}
void ModelWriter::WriteModel(ModelData*& in_data) {
	WriteMaterial(in_data);
	WriteMesh(in_data);
}


// Clip
void ClipWriter::WriteClip(Clip* clip, string clipName, uint index) {
	string savePath = "Assets/Models/BinaryClips/";
	string file = clipName + "_" + to_string(index) + ".clip";

	filesystem::create_directories(savePath);

	BinaryWriter* w = new BinaryWriter(savePath + file);
	w->WriteLine(clip->name);
	w->WriteLine(to_string(clip->framecount));
	w->WriteLine(Utility::String::From(clip->tickPerSecond));

	w->WriteLine(to_string(clip->keyFrames.size())); 
	for (KeyFrame* elem : clip->keyFrames) { 
		w->WriteLine(elem->boneName); 
		w->WriteLine(to_string(elem->transforms.size())); 
		w->WriteByte(elem->transforms.data(), sizeof(KeyTransform) * elem->transforms.size());
	}
	delete w;
}

ModelClip* ClipReader::ReadClip(string clipName, uint clipNum) {
	string path = "Assets/Models/BinaryClips/" + clipName + "_" + to_string(clipNum) + ".clip";
	BinaryReader* r = new BinaryReader(path);
	if (not r->isFileOpened()) {
		cout << "Wrong Clip Path >> " << path << endl;
		delete r;
		return nullptr;
	}
	ModelClip* clip = new ModelClip();
	clip->name = r->ReadLine();
	clip->frameCount = Utility::String::ToUint(r->ReadLine());
	clip->tickPerSecond = Utility::String::ToFloat(r->ReadLine());

	uint boneCount = Utility::String::ToUint(r->ReadLine());
	for (uint i = 0; i < boneCount; i++) {
		KeyFrame* keyFrame = new KeyFrame();

		keyFrame->boneName = r->ReadLine();

		uint size = Utility::String::ToUint(r->ReadLine());
		keyFrame->transforms.resize(size);
		void* ptr = (void*)keyFrame->transforms.data();
		r->ReadByte(ptr, sizeof(KeyTransform) * size);
		clip->keyFrames[keyFrame->boneName] = keyFrame;
	}
	delete r;
	return clip;
}
