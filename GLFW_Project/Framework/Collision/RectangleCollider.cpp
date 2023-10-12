#include "../../Framework.h"

RectangleCollider::RectangleCollider(Vector2 pos, Vector2 size)
    : pos(pos), size(size)
{
    tag = "Rect_Collider";
    Init(L"Shaders/Basic/Collider.hlsl");

    left = Left();
    right = Right();
    top = Top();
    bottom = Bottom();
}

RectangleCollider::~RectangleCollider()
{
    SAFE_DELETE(mesh);
    SAFE_DELETE(wBuffer);
}


void RectangleCollider::Init(wstring shader_path)
{
    SetShader(shader_path);

    vertices.emplace_back(left, top, 0.0f);
    vertices.emplace_back(right, top, 0.0f);
    vertices.emplace_back(left, bottom, 0.0f);
    vertices.emplace_back(right, bottom, 0.0f);
    indices = { 0, 1, 1, 3, 3, 2, 2, 0 };

    wBuffer = new MatrixBuffer();
    world = XMMatrixIdentity();
    mesh = new Mesh(
        vertices.data(), sizeof(Vertex), static_cast<UINT>(vertices.size()),
        indices.data(), static_cast<UINT>(indices.size()));
}

void RectangleCollider::SetShader(wstring file)
{
    material->SetShader(file);
}

bool RectangleCollider::IsCollision(const Vector2& point)
{
    if (!isActive) return false;

    if (point.x > Left() && point.x < Right())
    {
        if (point.y > Top() && point.y < Bottom())
            return true;
    }

    return false;
}

bool RectangleCollider::IsCollision(const RectangleCollider* rect)
{
    if (!isActive) return false;
    if (!rect->isActive) return false;

    if (rect->Left() < Right() && rect->Right() > Left())
    {
        if (rect->Top() < Bottom() && rect->Bottom() > Top())
            return true;
    }

    return false;
}

bool RectangleCollider::IsCollision(IN const RectangleCollider* rect, OUT Vector2& overlapSize)
{
    if (!isActive) return false;
    if (!rect->isActive) return false;

    float left = max(rect->Left(), Left());
    float right = min(rect->Right(), Right());

    float top = max(rect->Top(), Top());
    float bottom = min(rect->Bottom(), Bottom());

    overlapSize.x = right - left;
    overlapSize.y = bottom - top;

    if (overlapSize.x > 0 && overlapSize.y > 0)
        return true;

    return false;
}

void RectangleCollider::Render()
{
    if (!Active()) return;

    wBuffer->Set(world);
    wBuffer->SetVSBuffer(0);

    mesh->IASet(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

    material->Set();

    DC->DrawIndexed(indices.size(), 0, 0);
    //DC->Draw(4, 0);
}

