#pragma once
typedef VertexUVNormalTangentBlend ModelVertex;

struct MeshData {
	string name = "";
	uint meterialIndex = 0;
	vector<ModelVertex> vertices;
	vector<uint> indices;
};
struct NodeData {
	int parent = -1;
	int index = -1;
	string name = "";
	Matrix transform = Matrix(1);
};
struct BoneData {
	int index = 0;
	string name = "";
	Matrix offset = Matrix(1);
};

struct VertexWeights {
	uint  indices[4] = {}; // Bone Index
	float weights[4] = {}; // Bone Interpolation
	void Add(const uint& index, const float& weight) {
		for (uint i = 0; i < 4; i++) {
			if (weights[i] == 0.0f) {
				indices[i] = index;
				weights[i] = weight;
				return;
			}
		}
	}
	void Normalize() {
		float sum = weights[0] + weights[1] + weights[2] + weights[3];
		weights[0] /= sum;
		weights[1] /= sum;
		weights[2] /= sum;
		weights[3] /= sum;
	}
};

struct KeyTransform {
	Vector3    position;
	Quaternion rotation;
	Vector3    scale;
};

struct KeyFrame {
	string boneName;
	vector<KeyTransform> transforms;
};
struct ClipNode {
	aiString name;
	vector<KeyTransform> transforms;
};
struct Clip {
	string name;
	uint framecount;
	float tickPerSecond;
	vector<KeyFrame*> keyFrames;
};

struct ClipTransform {
	vector<vector<Matrix>> transform;
	//Matrix transform[FRAME][BONE]
};

struct ModelData {
	string model_name;

	vector<Material*> materials;
	vector<MeshData*> meshes;
	vector<NodeData*> nodes;
	vector<BoneData*> bones;

	map<string, uint> boneMap;
	uint boneCount = 0;

	ModelData() = default;
	~ModelData() {
		for (Material*& elem : materials) { SAFE_DELETE(elem); } materials.clear();
		for (MeshData*& elem : meshes   ) { SAFE_DELETE(elem); } meshes   .clear();
		for (NodeData*& elem : nodes    ) { SAFE_DELETE(elem); } nodes    .clear();
		for (BoneData*& elem : bones    ) { SAFE_DELETE(elem); } bones    .clear();
	}
};

struct InstanceData {
	Matrix world = Matrix(1);
	int index = 0;
};