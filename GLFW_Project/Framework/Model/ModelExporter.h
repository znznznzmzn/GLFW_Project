#pragma once

class ModelExporter {
private:
	Assimp::Importer* importer = nullptr;
	const aiScene* scene = nullptr;

	ModelData* data = nullptr;

	//cMaterial
	void ExtractMaterial();
	string CreateTexture(string aiPath);

	//Mesh
	void ExtractMesh(aiNode* node);
	void ExtractNode(aiNode* node, int index, int parent);
	void ExtractBone(aiMesh* mesh, vector<VertexWeights>& vertexWeights);

	//Clip
	Clip* ExtractClip(aiAnimation* animation);
	void ExtractKeyFrame(Clip* clip, aiNode* node, vector<ClipNode>& clipNodes);

	void aiTransferMatrix(Matrix& to, aiMatrix4x4& from) {
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++) to[i][j] = from[i][j];
	}
	void aiTransferVector3(Vector3& to, aiVector3D& from) {
		to.x = from.x;
		to.y = from.y;
		to.z = from.z;
	}
public:
	ModelExporter();
	~ModelExporter();

	void ExportModel(string model_name, string file_path = "Assets/Models/ModelSources/");
	void ExportClip(string clip_name, string file_path = "Assets/Models/ModelSources/Animations/");

	void Flush();
};