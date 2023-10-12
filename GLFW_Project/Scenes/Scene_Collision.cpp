#include "../Framework.h"
#include "Scene_Collision.h"

Scene_Collision::Scene_Collision() {
	collider1 = new BoxCollider();
	collider1->Tag() = "col_1";
	collider1->Position().x -= 10;
	collider2 = new BoxCollider();
	collider2->Tag() = "col_2";
	collider2->Position().x += 10;
	collider2->UpdateWorld();


	collider1->Scale() *= 0.1f;

	v.emplace_back(Vector3( 0, 0, +1));
	v.emplace_back(Vector3(+1, 0, -1));
	v.emplace_back(Vector3(-1, 0, -1));

	for (int i = 0; i < 3; i++) {
		indicator[i] = new BoxCollider();
		indicator[i]->Position() = v[i];
		indicator[i]->Scale() *= 0.1f;
		indicator[i]->UpdateWorld();
	}
}
Scene_Collision::~Scene_Collision() {
	SAFE_T_DELETE(collider1);
	SAFE_T_DELETE(collider2);

	for (int i = 0; i < 3; i++) { SAFE_T_DELETE(indicator[i]); }
}

void Scene_Collision::Update() {
	r = MAIN_CAMERA->ScreenPointToRay();
	//collider2->UpdateWorld();
	//if (collider1->Collision(collider2)) {
	//	collider1->SetColor(Collider::COLOR_COLLIDE);
	//	collider2->SetColor(Collider::COLOR_COLLIDE);
	//}
	//else {
	//	collider1->SetColor(Collider::COLOR_NORMAL);
	//	collider2->SetColor(Collider::COLOR_NORMAL);
	//}

	if(Calc::RayTriangle_Distance(r, v[0], v[1], v[2], dist))
		collider1->Position() = r.position + r.direction * dist;

	collider1->UpdateWorld();
}
void Scene_Collision::PreRender() {
}
void Scene_Collision::Render() {
	collider1->Render();
	collider2->Render();
	for (int i = 0; i < 3; i++) indicator[i]->Render();
}
void Scene_Collision::PostRender() {
}
void Scene_Collision::GUIRender() {
	collider1->GUIRender();
	collider2->GUIRender();
}
