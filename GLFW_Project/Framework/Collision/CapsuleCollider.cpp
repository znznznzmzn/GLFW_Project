#include "../Framework.h"

CapsuleCollider::CapsuleCollider(float height) : Collider(TYPE::CAPSULE) {
    uint Count = 30;    
	float Step = Calc::PI2 / Count;

    //Vertices
    for (uint i = 0; i <= Count; i++) { // x축 회전
        float x = i * Step;
        Vector3 vertex = Vector3(0, cos(x), sin(x));
        vertex.y += height / ((vertex.y > 0) ? 2.0f : -2.0f);
        vertices.push_back(vertex); 
    }
    for (uint i = 0; i <= Count; i++) { // y축 상층 회전
        float y = i * Step;
        vertices.push_back(Vector3(cos(y), height / 2.0f, sin(y)));
    }
    for (uint i = 0; i <= Count; i++) { // y축 하층 회전
        float y = i * Step; 
        vertices.push_back(Vector3(cos(y), -height / 2.0f, sin(y)));
    }
    for (uint i = 0; i <= Count; i++) { // z축 회전
        float z = i * Step;
        Vector3 vertex = Vector3(sin(z), cos(z), 0);
        vertex.y += height / ((vertex.y > 0) ? 2.0f : -2.0f); 
        vertices.push_back(vertex); 
    }

    //Indices
    for (uint i = 0; i < 4; i++) { // 각도별
        for (uint j = 0; j < Count; j++) { // 회전수 만큼
            indices.push_back(i + i * Count + j); // 0 
            indices.push_back(i + i * Count + j + 1); // 1
        }
    }
	CreateMesh();

    this->height = height;
}
CapsuleCollider::~CapsuleCollider() { }

bool CapsuleCollider::RayCollision(Ray ray, Contact* contact) {
    UpdateWorld();
    Vector3 direction = Up();
    Vector3 pa = GlobalPosition() - direction * Height() * 0.5f;
    Vector3 pb = GlobalPosition() + direction * Height() * 0.5f;

    float r = Radius();

    Vector3 ro = ray.position;
    Vector3 rd = ray.direction;

    Vector3 ba = pb - pa;
    Vector3 oa = ro - pa;

    float baba = glm::dot(ba, ba);
    float bard = glm::dot(ba, rd);
    float baoa = glm::dot(ba, oa);
    float rdoa = glm::dot(rd, oa);
    float oaoa = glm::dot(oa, oa);

    float a = baba - bard * bard;
    float b = baba * rdoa - baoa * bard;
    float c = baba * oaoa - baoa * baoa - r * r * baba;
    float h = b * b - a * c;

    if (h >= 0.0f) {
        float t = (-b - sqrt(h)) / a;
        float y = baoa + t * bard;
        if (y > 0.0f && y < baba) {
            if (contact) {
                contact->distance = t;
                contact->hitPoint = ray.position + ray.direction * t;
            }
            return true;
        }

        Vector3 oc = (y <= 0.0f) ? oa : ro - pb;
        b = glm::dot(rd, oc);
        c = glm::dot(oc, oc) - r * r;
        h = b * b - c;

        if (h > 0.0f) {
            if (contact) {
                contact->distance = t;
                contact->hitPoint = ray.position + ray.direction * t;
            }
            return true;
        }
    }

    return false;
}
bool CapsuleCollider::BoxCollision(BoxCollider* collider) { return collider->CapsuleCollision(this); }
bool CapsuleCollider::SphereCollision(SphereCollider* collider) {
    Vector3 upVector = Up();
    Vector3 A = GlobalPosition() - upVector * Height() * 0.5f;
    Vector3 B = GlobalPosition() + upVector * Height() * 0.5f;
    Vector3 P = collider->GlobalPosition();
    Vector3 pointOnLine = Calc::ClosestPointOnLine(A, B, P);
    return glm::distance(P, pointOnLine) <= (Radius() + collider->Radius());
}
bool CapsuleCollider::CapsuleCollision(CapsuleCollider* collider) {
    Vector3 aUpVector = Up();
    Vector3 aA = GlobalPosition() - aUpVector * Height() / 2.0f;
    Vector3 aB = GlobalPosition() + aUpVector * Height() / 2.0f;

    Vector3 bUpVector = collider->Up();
    Vector3 bA = collider->GlobalPosition() - bUpVector * collider->Height() / 2.0f;
    Vector3 bB = collider->GlobalPosition() + bUpVector * collider->Height() / 2.0f;

    Vector3 v0 = bA - aA;
    Vector3 v1 = bB - aA;
    Vector3 v2 = bA - aB;
    Vector3 v3 = bB - aB;

    float d0 = glm::dot(v0, v0);
    float d1 = glm::dot(v1, v1);
    float d2 = glm::dot(v2, v2);
    float d3 = glm::dot(v3, v3);

    Vector3 bestA = (d2 < d0 || d2 < d1 || d3 < d0 || d3 > d1) ? aB : aA;
    Vector3 bestB = Calc::ClosestPointOnLine(bA, bB, bestA);
    bestA = Calc::ClosestPointOnLine(aA, aB, bestB);

    return glm::distance(bestA, bestB) <= (Radius() + collider->Radius());
}
