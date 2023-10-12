#pragma once

class ModelReader {
	void ReadMaterial(ModelData*& out);
	void ReadMesh(ModelData*& out);
public:
	 ModelReader() = default;
	~ModelReader() = default;

	ModelData* ReadModel(string model_name);
};
class ModelWriter {
	void WriteMaterial(ModelData*& in_data);
	void WriteMesh(ModelData*& in_data);
public:
	 ModelWriter() = default;
	~ModelWriter() = default;

	void WriteModel(ModelData*& in_data);
};

class ClipReader {
public:
	 ClipReader() = default;
	~ClipReader() = default;

	ModelClip* ReadClip(string clipName, uint clipNum);
};

class ClipWriter {
public:
	 ClipWriter() = default;
	~ClipWriter() = default;

	void WriteClip(Clip* clip, string clipName, uint index);
};