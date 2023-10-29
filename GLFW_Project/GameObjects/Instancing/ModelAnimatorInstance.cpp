#include "../Framework.h"

void ModelAnimatorInstance::CreateClipTexture() {
	const uint clipCount = clips.size();
	if (clipCount <= 0) return;
	clipTransforms.resize(clipCount);
	nodeTransforms.resize(clipCount);

	// make data
	uint max_bone = data->nodes.size();
	uint max_frame = 0;
	for (uint i = 0; i < clipCount; i++) {
		clipTransforms[i] = new ClipTransform();
		clipTransforms[i]->transform.resize(clips[i]->frameCount);
		nodeTransforms[i] = new ClipTransform();
		nodeTransforms[i]->transform.resize(clips[i]->frameCount);
		for (vector<Matrix>& elem : clipTransforms[i]->transform) elem.resize(max_bone);
		for (vector<Matrix>& elem : nodeTransforms[i]->transform) elem.resize(max_bone);
		max_frame = max(max_frame, clips[i]->frameCount);
	}
	for (uint i = 0; i < clipCount; i++) CreateClipTransform(i);

	// Gegenerate virtual texture
	glGenTextures(1, &transformMapBuffer->texture_id);
	transformMapBuffer->width = max_bone * 4; // Matrix == Vector4 * 4
	transformMapBuffer->height = max_frame;
	transformMapBuffer->count = clipCount;
	transformMapBuffer->slot = 3;
	glActiveTexture(GL_TEXTURE0 + transformMapBuffer->slot);
	glBindTexture(GL_TEXTURE_2D_ARRAY, transformMapBuffer->texture_id);

	// GL_TEXTURE_2D_ARRAY Parameter
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Create storage for the virtual texture.
	glTexStorage3D(GL_TEXTURE_2D_ARRAY,
		1,          // Mipmap Level (No Mipmap)
		GL_RGBA32F, // Internal format
		transformMapBuffer->width,  // width
		transformMapBuffer->height, // height
		transformMapBuffer->count); // depth (Number of layers)

	// Make virtual texture
	uint lineSize = max_bone * sizeof(Matrix);
	uint pageSize = lineSize * max_frame;
	for (uint c = 0; c < clipCount; c++) {
		void* pixel = malloc(pageSize); // make
		for (uint y = 0; y < max_frame; y++) {
			void* line_point = (std::byte*)pixel + lineSize * y;
			if (y >= clips[c]->frameCount) memcpy(line_point, new Matrix[max_bone], lineSize);
			else memcpy(line_point, clipTransforms[c]->transform[y].data(), lineSize);
		}
		glTexSubImage3D(GL_TEXTURE_2D_ARRAY, // Store Texture
			0,		 // Mipmap level
			0, 0, c, // offset x, y, z
			transformMapBuffer->width,  // width
			transformMapBuffer->height, // height
			1, // depth
			GL_RGBA,	//format
			GL_FLOAT,	//type
			pixel);		//pointer to data
		free(pixel);
	}
}
void ModelAnimatorInstance::CreateClipTransform(uint index) {
	ModelClip* clip = clips[index];
	for (uint f = 0; f < clip->frameCount; f++) {
		uint nodeIndex = 0;
		for (NodeData*& node : data->nodes) {
			Matrix animation = Matrix(1);
			KeyFrame* frame = clip->GetKeyFrame(node->name);
			if (frame != nullptr) {
				KeyTransform& transform = frame->transforms[f];
				animation = glm::translate(Matrix(1), transform.position); // Position
				animation = glm::scale(animation, transform.scale); // Scale
				animation *= glm::toMat4(transform.rotation); // Rotation
			}

			int parentIndex = node->parent;
			Matrix parent = (parentIndex < 0) ? Matrix(1) : nodeTransforms[index]->transform[f][parentIndex];
			nodeTransforms[index]->transform[f][nodeIndex] = parent * animation;

			int boneIndex = (data->boneMap.count(node->name) == 0) ? -1 : data->boneMap[node->name];
			if (boneIndex >= 0)
				clipTransforms[index]->transform[f][boneIndex] =
				nodeTransforms[index]->transform[f][nodeIndex] *
				data->bones[boneIndex]->offset;
			nodeIndex++;
		}
	}
}

void ModelAnimatorInstance::EndEvent(void* obj) { PlayClip(((InstanceData*)obj)->index, 0); }

void ModelAnimatorInstance::InitUniformBuffer() {
	SAFE_DELETE(transformMapBuffer);
	SAFE_DELETE(frameInstanceBuffer);

	frameInstanceBuffer = new FrameInstanceBuffer();
	frameInstanceBuffer->Bind(data->materials[0]->Get_ProgramID());
	transformMapBuffer = new ModelAnimator::VirtualTexture(data->materials[0]->Get_ProgramID());
	transformMapBuffer->slot = 3;
}

void ModelAnimatorInstance::SetUniformBuffer() {
	frameInstanceBuffer->Set(); 
	transformMapBuffer->Set();
}
void ModelAnimatorInstance::SetMeshInstance() {
	ModelInstance::SetMeshInstance();
	glBindBuffer(GL_ARRAY_BUFFER, index_buffer_id);
	glBufferData(GL_ARRAY_BUFFER, instance_count * sizeof(uint), indices.data(), GL_STATIC_DRAW);
}

void ModelAnimatorInstance::AttachInstanceIndex() {
	indices.clear();
	indices.resize(instance_count);
	for (uint i = 0; i < instance_count; i++) indices[i] = i;

	if (index_buffer_id != -1) glDeleteBuffers(1, &index_buffer_id);

	glGenBuffers(1, &index_buffer_id);
	glBindBuffer(GL_ARRAY_BUFFER, index_buffer_id);
	glBufferData(GL_ARRAY_BUFFER, instance_count * sizeof(uint), indices.data(), GL_STATIC_DRAW);

	for (auto elem : render_map) {
		for (ModelMesh*& mesh : elem.second) {
			mesh->Set();
			glEnableVertexAttribArray(10);
			glVertexAttribPointer(10, 1, GL_UNSIGNED_INT, GL_FALSE, sizeof(uint), (void*)0);
			glBindVertexArray(0);
		}
	}
}

ModelAnimatorInstance::ModelAnimatorInstance(string model_name, uint instanceCount) {
	instance_count = instanceCount;
	LoadModel(model_name);
	SetShaders(
		"Assets/GLSL/Universal/ModelAnimatorInstance.vert",
		"Assets/GLSL/Universal/Universal.frag");

	AttachInstanceMatrix();
	AttachInstanceIndex();
}
ModelAnimatorInstance::~ModelAnimatorInstance() {
	SAFE_DELETE(transformMapBuffer);
	SAFE_DELETE(frameInstanceBuffer);

	for (ModelClip*& elem : clips) { SAFE_DELETE(elem); }
	clips.clear();

	for (ClipTransform*& elem : clipTransforms) { SAFE_DELETE(elem); }
	for (ClipTransform*& elem : nodeTransforms) { SAFE_DELETE(elem); }
	clipTransforms.clear();
	nodeTransforms.clear();
}

void ModelAnimatorInstance::UpdateFrames() { for (uint i = 0; i < instance_count; i++) UpdateFrame(i); }
void ModelAnimatorInstance::UpdateFrame(uint instance_index) {
	if (!isPlay) return;
	FrameInstanceBuffer::FrameInstance& motion = frameInstanceBuffer->data.frames[instance_index];
	{ // Cur Anim
		ModelClip*& clip = clips[motion.cur.clip];
		motion.runningTime += motion.cur.speed * DELTA;
		motion.cur.time += clip->tickPerSecond * motion.cur.speed * DELTA;
		if (motion.cur.time >= 1.0f) {
			motion.cur.curFrame
				= Calc::RepeatIdx(motion.cur.curFrame + 1, clip->frameCount - 1);
			motion.cur.time = Calc::Repeat(motion.cur.time, 0.0f, 1.0f);
		}
	}

	{ // Next Anim
		if (motion.next.clip < 0) return;
		ModelClip*& clip = clips[motion.next.clip];
		motion.tweenTime += DELTA / motion.takeTime;

		if (motion.tweenTime >= 1.0f) { // EndBlending
			motion.cur = motion.next;
			motion.tweenTime = 0.0f;

			motion.next.clip = -1;
			motion.next.curFrame = 0;
			motion.next.time = 0.0f;
		}
		else {
			motion.next.time += clip->tickPerSecond * motion.next.speed * DELTA;
			if (motion.next.time >= 1.0f) {
				motion.next.curFrame = Calc::RepeatIdx(motion.next.curFrame + 1, clip->frameCount - 1);
				motion.next.time = Calc::Repeat(motion.next.time, 0.0f, 1.0f);
			}
		}
	}
}

void ModelAnimatorInstance::Render() {
	if (!is_active) return;
	if (transformMapBuffer->texture_id == -1) CreateClipTexture();
	ModelInstance::Render();
}
void ModelAnimatorInstance::GUIRender_Instance(uint index) {
	Instance::GUIRender_Instance(index);

	if (clips.empty()) return;

	FrameInstanceBuffer::FrameInstance& frame = frameInstanceBuffer->data.frames[index];

	int& clip = frame.cur.clip;

	if (ImGui::Button("Play")) PlayClip(index, clip);
	ImGui::SameLine();
	if (ImGui::Button("Pause")) isPlay = false;
	ImGui::SameLine();
	if (ImGui::Button("Resume")) isPlay = true;

	if (ImGui::SliderInt("Clip", &clip, 0, clips.size() - 1)) PlayClip(index, clip);
	ImGui::SliderInt("Frame", (int*)&frame.cur.curFrame, 0, clips[clip]->frameCount - 1);
}

void ModelAnimatorInstance::ReadClip(string clipName, uint clipNum) {
	ClipReader* cr = new ClipReader();
	ModelClip* clip = cr->ReadClip(clipName, clipNum);
	if (clip) clips.emplace_back(clip);
	delete cr;
}
void ModelAnimatorInstance::PlayClips(uint clip, float speed, float takeTime) 
{ for (uint i = 0; i < instance_count; i++) PlayClip(i, clip, speed, takeTime); }
void ModelAnimatorInstance::PlayClip(uint instance, uint clip, float speed, float takeTime) {
	isPlay = true;
	frameInstanceBuffer->data.frames[instance].next.clip = clip;
	frameInstanceBuffer->data.frames[instance].next.speed = speed;
	frameInstanceBuffer->data.frames[instance].takeTime = takeTime;
	frameInstanceBuffer->data.frames[instance].runningTime = 0.0f;
}

Matrix ModelAnimatorInstance::GetTransformByNode(int nodeIndex, int instanceIndex) {
	if (transformMapBuffer->texture_id == -1) return Matrix(1);
	nodeIndex = Calc::Clamp(nodeIndex, 0, data->nodes.size() - 1);
	Matrix curAnim;
	{//CurAnim
		FrameBuffer::Frame& curFrame = frameInstanceBuffer->data.frames[instanceIndex].cur;
		Matrix cur = nodeTransforms[curFrame.clip]->transform[curFrame.curFrame][nodeIndex];
		Matrix next = nodeTransforms[curFrame.clip]->transform[curFrame.curFrame + 1][nodeIndex];
		curAnim = LERP(cur, next, curFrame.time);
	}

	{//NextAnim
		FrameBuffer::Frame& nextFrame = frameInstanceBuffer->data.frames[instanceIndex].next;
		if (nextFrame.clip == -1) return curAnim;
		Matrix cur = nodeTransforms[nextFrame.clip]->transform[nextFrame.curFrame][nodeIndex];
		Matrix next = nodeTransforms[nextFrame.clip]->transform[nextFrame.curFrame + 1][nodeIndex];
		Matrix nextAnim = LERP(cur, next, nextFrame.time);
		return LERP(curAnim, nextAnim, frameInstanceBuffer->data.frames[instanceIndex].tweenTime);
	}
}
