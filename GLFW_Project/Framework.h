#pragma once

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define SCREEN_SIZE Vector2(SCREEN_WIDTH, SCREEN_HEIGHT)
#define SCREEN_ASPECT (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT
#define CENTER_X SCREEN_WIDTH / 2
#define CENTER_Y SCREEN_HEIGHT / 2

#define MAX_BONES 256

#define FLOAT4_CLEAR_COLOR 0.2f, 0.2f, 0.2f, 1.0f

#define SAFE_DELETE(PTR) if(PTR != nullptr) delete PTR; PTR = nullptr
#define SAFE_DELETE_LIST(LIST) if(LIST != nullptr) delete[] LIST; LIST = nullptr
#define SAFE_T_DELETE(TRS) if(TRS != nullptr) TRS->NullParent(); SAFE_DELETE(TRS)

#define SCENE SceneManager::Get()

#define ENVIRONMENT Environment::Get()
#define MAIN_CAMERA Environment::Get()->GetMainCamera()
#define MAIN_TWEEN Environment::Get()->GetMainTween()
#define MAIN_LIGHT Environment::Get()->GetMainLight()

// ImGui
#define IMGUI_DIALOG ImGuiFileDialog::Instance()

// 키
#define KEY_PRESS(key) ImGui::IsKeyDown(key)
#define KEY_DOWN(key) ImGui::IsKeyPressed(key, false)
#define KEY_DOWN_ING(key) ImGui::IsKeyPressed(key, true)
#define KEY_UP(key) ImGui::IsKeyReleased(key)

// 마우스 - 0:좌, 1:우, 2:중간
#define MOUSE_PRESS(idx) ImGui::IsMouseDown(idx)
#define MOUSE_CLICK(idx) ImGui::IsMouseClicked(idx, false)
#define MOUSE_CLICK_ING(idx) ImGui::IsMouseClicked(idx, true)
#define MOUSE_DBCLICK(idx) ImGui::IsMouseDoubleClicked(idx)
#define MOUSE_UP(idx) ImGui::IsMouseReleased(idx)
#define MOUSE_POSITION ImGui::GetMousePos()

// MathDefines
#define PI_F glm::pi<float>()
#define LERP(s, e, t) (s + (e - s) * t)
#define DELTA delta

//ImGui
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <ImGuiFileDialog.h>

// basic
#include <iostream>
#include <random>
#include <string>
#include <vector>
#include <stack>
#include <map>
#include <unordered_map>
#include <functional>
#include <fstream>
#include <sstream>
#include <memory>
#include <stdexcept>
#include <filesystem>

// GL
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "Libraries/GLEW/lib/glew32.lib")
#pragma comment(lib, "Libraries/GLFW/lib/glfw3.lib")

// glm
#include <glm.hpp>
#include "Libraries/glm/gtc/matrix_transform.hpp"
#include "Libraries/glm/gtx/transform.hpp"
#include "Libraries/glm/gtx/quaternion.hpp"
#include "Libraries/glm/gtx/euler_angles.hpp"
#include "Libraries/glm/gtc/type_ptr.hpp"

typedef glm::vec4 Vector4;
typedef glm::vec3 Vector3;
typedef glm::vec2 Vector2;
typedef glm::mat4 Matrix;
typedef glm::quat Quaternion;
typedef unsigned int uint;

extern float delta;
extern Vector2 mouseVelocity;
extern Vector2 mousePos;

// Image Loader
#include <stb_image.h>
#include <stb_image_write.h>
 // 지원되는 이미지 파일 TGA, PNG, JPEG, BMP, GIF, HDR, PNM

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#pragma comment(lib, "assimp-vc143-mtd.lib")

using namespace std;
typedef function<void()> Event;

// Framework Header
 // Utility
#include "Framework/Utility/Utility.h"
#include "Framework/Utility/Singleton.h"
#include "Framework/Utility/BinaryReader.h"
#include "Framework/Utility/BinaryWriter.h"

 // Buffer
#include "Framework/Buffer/VertexLayout.h"
#include "Framework/Buffer/UniformBuffer.h"
#include "Framework/Buffer/GlobalBuffer.h"

 // Render
#include "Framework/Render/Shader.h"
#include "Framework/Render/Texture.h"
#include "Framework/Render/Material.h"
#include "Framework/Render/Mesh.h"

// Model
#include "Framework/Model/ModelTypes.h"
#include "Framework/Model/ModelClip.h"
#include "Framework/Model/ModelBinary.h"
#include "Framework/Model/ModelExporter.h"

 // 에디터용 ImGui 패턴
#include "Framework/Utility/EditorGUI.h" 

 // Math
#include "Framework/Math/Calc.h"
#include "Framework/Math/Transformation.h"
#include "Framework/Math/Transform.h"

 // Collision
#include "Framework/Collision/Collider.h"
#include "Framework/Collision/BoxCollider.h"
#include "Framework/Collision/SphereCollider.h"
#include "Framework/Collision/CapsuleCollider.h"
// #include "Framework/Collision/RectangleCollider.h" -> 2D

 // Environment
#include "Framework/Environment/Camera.h"
#include "Framework/Environment/Tween.h"
#include "Framework/Environment/Environment.h"

 // GameObjects
#include "GameObjects/Basic/Grid.h"

#include "GameObjects/Basic/GameObject.h"
#include "GameObjects/Basic/Cube.h"
#include "GameObjects/Basic/Cylinder.h"
#include "GameObjects/Basic/Sphere.h"
#include "GameObjects/Basic/Plane.h"
#include "GameObjects/Basic/Quad.h"

#include "GameObjects/Landscape/Terrain.h"
#include "GameObjects/Landscape/TerrainEditor.h"

#include "GameObjects/Model/Model.h"
#include "GameObjects/Model/ModelNode.h"
#include "GameObjects/Model/ModelBone.h"
#include "GameObjects/Model/ModelAnimator.h"

 // Scene Header
#include "Scenes/Scene.h"
#include "GameManager/GameManager.h"
#include "GameManager/SceneManager.h"