#include "../../Framework.h"

void ModelExporter::ExtractMaterial() {
	for (uint i = 0; i < scene->mNumMaterials; i++) {
		aiMaterial* srcMaterial = scene->mMaterials[i];
		Material* material = new Material();

		string mat_Name = srcMaterial->GetName().C_Str();
		material->SetName(mat_Name);

		aiColor3D color;
		srcMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color);
		material->GetData_Diffuse() = Vector4(color.r, color.g, color.b, 1.0f);

		srcMaterial->Get(AI_MATKEY_COLOR_SPECULAR, color);
		material->GetData_Specular() = Vector4(color.r, color.g, color.b, 1.0f);

		srcMaterial->Get(AI_MATKEY_COLOR_AMBIENT, color);
		material->GetData_Emissive() = Vector4(color.r, color.g, color.b, 1.0f);

		srcMaterial->Get(AI_MATKEY_SHININESS, material->GetData_Shininess());

		aiString texture_path;
		srcMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &texture_path);
		material->SetDiffuseMap(CreateTexture(texture_path.C_Str()));
		texture_path.Clear();

		srcMaterial->GetTexture(aiTextureType_SPECULAR, 0, &texture_path);
		material->SetSpecularMap(CreateTexture(texture_path.C_Str()));
		texture_path.Clear();

		srcMaterial->GetTexture(aiTextureType_NORMALS, 0, &texture_path);
		material->SetNormalMap(CreateTexture(texture_path.C_Str()));
		texture_path.Clear();

		data->materials.push_back(material);
	}
}
// Mesh
void ModelExporter::ExtractMesh(aiNode* node) {
	for (uint i = 0; i < node->mNumMeshes; i++) {
		MeshData* mesh = new MeshData();
		mesh->name = node->mName.C_Str();

		uint idx = node->mMeshes[i];
		aiMesh* srcMesh = scene->mMeshes[idx];

		mesh->meterialIndex = srcMesh->mMaterialIndex;

		uint startVertex = mesh->vertices.size();

		vector<VertexWeights> vertexWeights(srcMesh->mNumVertices);
		ExtractBone(srcMesh, vertexWeights);

		mesh->vertices.clear();
		for (uint v = 0; v < srcMesh->mNumVertices; v++) {
			ModelVertex vertex;

			aiTransferVector3(vertex.pos, srcMesh->mVertices[v]); // Pos
			if (srcMesh->HasTextureCoords(0)) { // UV
				vertex.uv.x = srcMesh->mTextureCoords[0][v].x;
				vertex.uv.y = srcMesh->mTextureCoords[0][v].y;
			}
			if (srcMesh->HasNormals()) // Normal
				aiTransferVector3(vertex.normal, srcMesh->mNormals[v]); 
			if (srcMesh->HasTangentsAndBitangents())  // Tangent
				aiTransferVector3(vertex.tangent, srcMesh->mTangents[v]);
			

			if (!vertexWeights.empty()) {
				vertexWeights[v].Normalize();
				vertex.indices.x = static_cast<float>(vertexWeights[v].indices[0]);
				vertex.indices.y = static_cast<float>(vertexWeights[v].indices[1]);
				vertex.indices.z = static_cast<float>(vertexWeights[v].indices[2]);
				vertex.indices.w = static_cast<float>(vertexWeights[v].indices[3]);

				vertex.weights.x = vertexWeights[v].weights[0];
				vertex.weights.y = vertexWeights[v].weights[1];
				vertex.weights.z = vertexWeights[v].weights[2];
				vertex.weights.w = vertexWeights[v].weights[3];
			}
			mesh->vertices.emplace_back(vertex);
		}

		mesh->indices.clear();
		for (uint f = 0; f < srcMesh->mNumFaces; f++) {
			aiFace& face = srcMesh->mFaces[f];
			for (uint k = 0; k < face.mNumIndices; k++) 
				mesh->indices.emplace_back(face.mIndices[k]);
		}

		data->meshes.push_back(mesh);
	}

	for (uint i = 0; i < node->mNumChildren; i++) ExtractMesh(node->mChildren[i]);
}
// Node
void ModelExporter::ExtractNode(aiNode* node, int index, int parent) {
	NodeData* nodeData = new NodeData();
	nodeData->index = index;
	nodeData->parent = parent;
	nodeData->name = node->mName.C_Str();

	aiTransferMatrix(nodeData->transform, node->mTransformation);
	nodeData->transform = glm::transpose(nodeData->transform);

	data->nodes.push_back(nodeData);
	for (uint i = 0; i < node->mNumChildren; i++)
		ExtractNode(node->mChildren[i], data->nodes.size(), index);
}
// Bone
void ModelExporter::ExtractBone(aiMesh* mesh, vector<VertexWeights>& vertexWeights) {
	for (uint i = 0; i < mesh->mNumBones; i++) {
		uint boneIndex = 0;

		string name = mesh->mBones[i]->mName.C_Str();
		if (data->boneMap.count(name) == 0) {
			boneIndex = data->boneCount++;
			data->boneMap[name] = boneIndex;

			BoneData* boneData = new BoneData(); 
			boneData->name = name; 
			boneData->index = static_cast<int>(boneIndex); 

			aiTransferMatrix(boneData->offset, mesh->mBones[i]->mOffsetMatrix);
			boneData->offset = glm::transpose(boneData->offset);

			data->bones.push_back(boneData);
		}
		else boneIndex = data->boneMap[name];

		for (uint j = 0; j < mesh->mBones[i]->mNumWeights; j++) {
			uint index = mesh->mBones[i]->mWeights[j].mVertexId;
			vertexWeights[index].Add(boneIndex, mesh->mBones[i]->mWeights[j].mWeight);
		}
	}
}

// Clip
Clip* ModelExporter::ExtractClip(aiAnimation* animation) {
	Clip* clip = new Clip();
	clip->name = animation->mName.C_Str();
	clip->tickPerSecond = (float)animation->mTicksPerSecond;
	clip->framecount = (uint)(animation->mDuration) + 1;

	vector<ClipNode> clipNodes;
	for (uint i = 0; i < animation->mNumChannels; i++) {
		aiNodeAnim* srcNode = animation->mChannels[i];

		ClipNode node;
		node.name = srcNode->mNodeName;

		uint keyCount = max(srcNode->mNumPositionKeys, srcNode->mNumRotationKeys);
		keyCount = max(keyCount, srcNode->mNumScalingKeys);

		KeyTransform transform;

		for (uint k = 0; k < keyCount; k++) {
			bool isFound = false;
			float t = static_cast<float>(node.transforms.size());

			if (k < srcNode->mNumPositionKeys && abs((float)srcNode->mPositionKeys[k].mTime - t) <= FLT_EPSILON) {
				aiVectorKey key = srcNode->mPositionKeys[k];
				memcpy_s(&transform.position, sizeof(Vector3), &key.mValue, sizeof(aiVector3D));
				isFound = true;
			}
			if (k < srcNode->mNumRotationKeys && abs((float)srcNode->mRotationKeys[k].mTime - t) <= FLT_EPSILON) {
				aiQuatKey key = srcNode->mRotationKeys[k];
				transform.rotation.x = static_cast<float>(key.mValue.x);
				transform.rotation.y = static_cast<float>(key.mValue.y);
				transform.rotation.z = static_cast<float>(key.mValue.z);
				transform.rotation.w = static_cast<float>(key.mValue.w);
				isFound = true;
			}
			if (k < srcNode->mNumScalingKeys && abs((float)srcNode->mScalingKeys[k].mTime - t) <= FLT_EPSILON) {
				aiVectorKey key = srcNode->mScalingKeys[k];
				memcpy_s(&transform.scale, sizeof(Vector3), &key.mValue, sizeof(aiVector3D));
				isFound = true;
			}
			if (isFound) node.transforms.push_back(transform);
		}
		if (node.transforms.size() < clip->framecount) {
			uint count = clip->framecount - node.transforms.size();
			KeyTransform keyTransform = node.transforms.back();
			for (uint n = 0; n < count; n++) node.transforms.push_back(keyTransform);
		}
		clipNodes.push_back(node);
	}

	ExtractKeyFrame(clip, scene->mRootNode, clipNodes);

	return clip;
}

void ModelExporter::ExtractKeyFrame(Clip* clip, aiNode* node, vector<ClipNode>& clipNodes) {
	KeyFrame* keyFrame = new KeyFrame();
	keyFrame->boneName = node->mName.C_Str();
	for (uint i = 0; i < clip->framecount; i++) {
		ClipNode* clipNode = nullptr;
		for (ClipNode& tmp : clipNodes) {
			if (tmp.name == node->mName) {
				clipNode = &tmp;
				break;
			}
		}
		KeyTransform keyTransform;
		if (clipNode == nullptr) {
			Matrix transform = Matrix(1);
			aiTransferMatrix(transform, node->mTransformation);
			transform = glm::transpose(transform);

			keyTransform.position = transform[3];
			for (int i = 0; i < 3; i++)
				keyTransform.scale[i] = glm::length(Vector3(transform[i]));
			keyTransform.rotation = 
				glm::quat_cast(glm::mat3(
					Vector3(transform[0]) / keyTransform.scale[0],
					Vector3(transform[1]) / keyTransform.scale[1],
					Vector3(transform[2]) / keyTransform.scale[2]));
		}
		else keyTransform = clipNode->transforms[i];

		keyFrame->transforms.push_back(keyTransform);
	}
	clip->keyFrames.push_back(keyFrame);

	for (uint i = 0; i < node->mNumChildren; i++)
		ExtractKeyFrame(clip, node->mChildren[i], clipNodes);
}

string ModelExporter::CreateTexture(string aiPath) {
	if (aiPath.length() == 0) return "";
	string folderPath = "Assets/Models/BinaryModels/" + data->model_name + "/";
	string file = Utility::String::GetFileNameWidthoutExtension(aiPath) + ".png";
	string path = folderPath + file;

	BinaryWriter* w = new BinaryWriter(path);
	if (!w->isFileOpened()) {
		filesystem::create_directories(folderPath);
		w->OpenFile(path);
	}

	const aiTexture* texture = scene->GetEmbeddedTexture(aiPath.c_str());
	
	if (texture->mHeight < 1)
		w->WriteBytePure(texture->pcData, texture->mWidth);
	else {
		SAFE_DELETE(w);
		stbi_write_png(path.c_str(), texture->mWidth, texture->mHeight, 3, texture->pcData, texture->mWidth * 3);
	}

	SAFE_DELETE(w);

	return path;
}

// public
ModelExporter::ModelExporter() {
	importer = new Assimp::Importer();
	importer->SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false); 
}
ModelExporter::~ModelExporter() { 
	Flush();
	SAFE_DELETE(importer);
}

void ModelExporter::Flush() { SAFE_DELETE(data); }

void ModelExporter::ExportModel(string model_name, string file_path) {
	if (file_path.find(".fbx") == string::npos)
		file_path = file_path + model_name + ".fbx";
	Flush();
	data = new ModelData();
	data->model_name = model_name;
	scene = importer->ReadFile(file_path,
		aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_MaxQuality);
	assert(scene != nullptr);

	ExtractMaterial();
	ExtractNode(scene->mRootNode, -1, -1);
	ExtractMesh(scene->mRootNode);

	ModelWriter* mw = new ModelWriter();
	mw->WriteModel(data);
	SAFE_DELETE(mw);
}
void ModelExporter::ExportClip(string clip_name, string file_path) {
	if (file_path.find(".fbx") == string::npos)
		file_path = file_path + clip_name + ".fbx";
	scene = importer->ReadFile(file_path,
		aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_MaxQuality);
	assert(scene != nullptr);

	ClipWriter* cw = new ClipWriter();
	for (uint i = 0; i < scene->mNumAnimations; i++) {
		Clip* clip = ExtractClip(scene->mAnimations[i]);
		cw->WriteClip(clip, clip_name, i);
		SAFE_DELETE(clip);
	}
	SAFE_DELETE(cw);
}
