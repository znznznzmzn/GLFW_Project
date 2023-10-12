#pragma once
class RectangleCollider : public Transform
{
private:
    Material* material = nullptr;
    MatrixBuffer* wBuffer = nullptr;
protected:
    Mesh* mesh;
    vector<Vertex> vertices;
    vector<UINT> indices;

public:
    RectangleCollider(Vector2 pos, Vector2 size);
    ~RectangleCollider();

    void Init(wstring shader_path);
    void SetShader(wstring file);

    bool IsCollision(const Vector2& point);
    bool IsCollision(const Rectangle* rect);
    bool IsCollision(IN const Rectangle* rect, OUT Vector2& overlapSize);

    void SetColor(float4 color) { material->GetData().diffuse = color; }
    void SetColor(float r, float g, float b) { SetColor({ r, g, b, 1.0f }); }


    void Render();

    float Left() const { return pos.x - size.x * 0.5f; }
    float Right() const { return pos.x + size.x * 0.5f; }
    float Top() const { return pos.y - size.y * 0.5f; }
    float Bottom() const { return pos.y + size.y * 0.5f; }

    Vector2 Half() const { return size * 0.5f; }

    Vector2& Pos() { return pos; }
    Vector2& Size() { return size; }
    bool& Active() { return isActive; }

protected:
    bool isActive = true;

    Vector2 pos;
    Vector2 size;

    float left;
    float right;
    float top;
    float bottom;
};