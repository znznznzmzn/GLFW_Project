#include "../Framework.h"

void ModelAnimator::CreateClipTexture() {
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
	transformMapBuffer->width  = max_bone * 4; // Matrix == Vector4 * 4
	transformMapBuffer->height = max_frame;
	transformMapBuffer->count  = clipCount;
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
void ModelAnimator::CreateClipTransform(uint index) {
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

ModelAnimator::ModelAnimator(string model_name) {
	LoadModel(model_name);
	SetShaders(
		"Assets/GLSL/Universal/ModelAnimator.vert", 
		"Assets/GLSL/Universal/Universal.frag");
	LockShader();
	fBuffer = new FrameBuffer();
	fBuffer->Bind(commonProgramID);
	transformMapBuffer = new VirtualTexture(commonProgramID);
	transformMapBuffer->slot = 3;
}
ModelAnimator::~ModelAnimator() {
	for (ModelClip*& elem : clips) { SAFE_DELETE(elem); }
	clips.clear();

	SAFE_DELETE(transformMapBuffer);
	SAFE_DELETE(fBuffer);

	for (ClipTransform*& elem : clipTransforms) { SAFE_DELETE(elem); }
	for (ClipTransform*& elem : nodeTransforms) { SAFE_DELETE(elem); }
	clipTransforms.clear();
	nodeTransforms.clear();
}

void ModelAnimator::UpdateFrame() {
	if (!isPlay) return;
	FrameBuffer::Data& frameData = fBuffer->data;
	{ // Cur Anim
		ModelClip*& clip = clips[frameData.cur.clip];
		clip->playTime += frameData.cur.speed * DELTA;
		frameData.cur.time += clip->tickPerSecond * frameData.cur.speed * DELTA;
		if (frameData.cur.time >= 1.0f) {
			frameData.cur.curFrame
				= Calc::RepeatIdx(frameData.cur.curFrame + 1, clip->frameCount - 1);
			frameData.cur.time = Calc::Repeat(frameData.cur.time, 0.0f, 1.0f);
		}
		clip->Excute();
	}
	{ // Next Anim
		if (frameData.next.clip < 0) return;
		ModelClip*& clip = clips[frameData.next.clip];
		frameData.tweenTime += DELTA / frameData.takeTime;

		if (frameData.tweenTime >= 1.0f) { // EndBlending
			frameData.cur = frameData.next;
			frameData.tweenTime = 0.0f;

			frameData.next.clip = -1;
			frameData.next.curFrame = 0;
			frameData.next.time = 0.0f;
		}
		else {
			frameData.next.time += clip->tickPerSecond * frameData.next.speed * DELTA;
			if (frameData.next.time >= 1.0f) {
				frameData.next.curFrame = Calc::RepeatIdx(frameData.next.curFrame + 1, clip->frameCount - 1);
				frameData.next.time = Calc::Repeat(frameData.next.time, 0.0f, 1.0f);
			}
		}
	}
}

void ModelAnimator::Render() {
	if (!Active()) return;
	if (transformMapBuffer->texture_id == -1) CreateClipTexture();
	Model::Render();
}
void ModelAnimator::SetUniformBuffer() {
	Model::SetUniformBuffer();
	fBuffer->Set();
	transformMapBuffer->Set();
}

void ModelAnimator::GUIRender() {
	if (ImGui::TreeNode(tag.c_str())) {
		int clip = fBuffer->data.cur.clip;

		if (ImGui::Button("Play")) PlayClip(clip);
		ImGui::SameLine();
		if (ImGui::Button("Pause")) isPlay = false;
		ImGui::SameLine();
		if (ImGui::Button("Resume")) isPlay = true;
		
		if (ImGui::SliderInt("Clip", &clip, 0, clips.size() - 1)) PlayClip(clip);
		ImGui::SliderInt("Frame", (int*)&fBuffer->data.cur.curFrame, 0, clips[clip]->frameCount - 1);
		ImGui::Text(("model_name : " + data->model_name).c_str());
		char name_buf[128] = "";
		if (ImGui::InputTextWithHint("Tag", tag.c_str(), name_buf, IM_ARRAYSIZE(name_buf)))
			if (KEY_DOWN(ImGuiKey_Enter)) tag = string(name_buf);
		Transform::GUIRender();
		for (auto elem : render_map) elem.first->GUIRender();
		ImGui::TreePop();
	}
}

void ModelAnimator::ReadClip(string clipName, uint clipNum) {
	ClipReader* cr = new ClipReader();
	ModelClip* clip = cr->ReadClip(clipName, clipNum);
	if (clip) clips.emplace_back(clip);
	delete cr;
}
void ModelAnimator::PlayClip(uint clip, float speed, float takeTime) {
	isPlay = true;
	fBuffer->data.next.clip = clip;
	fBuffer->data.next.speed = speed;
	fBuffer->data.takeTime = takeTime;
	clips[clip]->duration /= speed;
	clips[clip]->Init();
}

Matrix ModelAnimator::GetTransformByNode(int nodeIndex) {
	if (transformMapBuffer->texture_id == -1) return Matrix(1);
	nodeIndex = Calc::Clamp(nodeIndex, 0, data->nodes.size() - 1);
	Matrix curAnim;

	{//CurAnim
		FrameBuffer::Frame& curFrame = fBuffer->data.cur;
		Matrix cur = nodeTransforms[curFrame.clip]->transform[curFrame.curFrame][nodeIndex];
		Matrix next = nodeTransforms[curFrame.clip]->transform[curFrame.curFrame + 1][nodeIndex];
		curAnim = LERP(cur, next, curFrame.time);
	}

	{//NextAnim
		FrameBuffer::Frame& nextFrame = fBuffer->data.next;
		if (nextFrame.clip == -1) return curAnim;
		Matrix cur = nodeTransforms[nextFrame.clip]->transform[nextFrame.curFrame][nodeIndex];
		Matrix next = nodeTransforms[nextFrame.clip]->transform[nextFrame.curFrame + 1][nodeIndex];
		Matrix nextAnim = LERP(cur, next, nextFrame.time);
		return LERP(curAnim, nextAnim, fBuffer->data.tweenTime);
	}
}
