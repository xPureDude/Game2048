#include <cmath>
#include <iostream>
#include <limits>
#include <vector>
/*
// 定义二维向量结构体
struct Vector2 {
    float x;
    float y;

    Vector2(float _x = 0, float _y = 0) : x(_x), y(_y) {}

    // 向量减法
    Vector2 operator-(const Vector2& other) const {
        return Vector2(x - other.x, y - other.y);
    }

    // 向量点积
    float dot(const Vector2& other) const {
        return x * other.x + y * other.y;
    }

    // 向量归一化
    Vector2 normalize() const {
        float length = std::sqrt(x * x + y * y);
        return Vector2(x / length, y / length);
    }

    // 向量求垂直向量
    Vector2 perpendicular() const {
        return Vector2(-y, x);
    }
};

// 获取旋转矩形的四个顶点
std::vector<Vector2> getVertices(const Vector2& center, float width, float height, float angle) {
    std::vector<Vector2> vertices(4);
    float cosAngle = std::cos(angle);
    float sinAngle = std::sin(angle);

    // 计算四个顶点相对于中心点的偏移量
    Vector2 halfSize(width / 2, height / 2);
    vertices[0] = center + Vector2(-halfSize.x * cosAngle - halfSize.y * sinAngle, -halfSize.x * sinAngle + halfSize.y * cosAngle);
    vertices[1] = center + Vector2(halfSize.x * cosAngle - halfSize.y * sinAngle, halfSize.x * sinAngle + halfSize.y * cosAngle);
    vertices[2] = center + Vector2(halfSize.x * cosAngle + halfSize.y * sinAngle, halfSize.x * sinAngle - halfSize.y * cosAngle);
    vertices[3] = center + Vector2(-halfSize.x * cosAngle + halfSize.y * sinAngle, -halfSize.x * sinAngle - halfSize.y * cosAngle);

    return vertices;
}

// 将一组顶点投影到一个轴上，返回投影的最小值和最大值
void project(const std::vector<Vector2>& vertices, const Vector2& axis, float& min, float& max) {
    min = std::numeric_limits<float>::max();
    max = std::numeric_limits<float>::lowest();

    for (const auto& vertex : vertices) {
        float projection = vertex.dot(axis);
        if (projection < min) {
            min = projection;
        }
        if (projection > max) {
            max = projection;
        }
    }
}

// 分离轴定理判断两个多边形是否相交
bool separatingAxisTheorem(const std::vector<Vector2>& vertices1, const std::vector<Vector2>& vertices2) {
    int numVertices1 = vertices1.size();
    int numVertices2 = vertices2.size();

    // 遍历第一个多边形的所有边
    for (int i = 0; i < numVertices1; ++i) {
        Vector2 edge = vertices1[(i + 1) % numVertices1] - vertices1[i];
        Vector2 axis = edge.perpendicular().normalize();

        float min1, max1, min2, max2;
        project(vertices1, axis, min1, max1);
        project(vertices2, axis, min2, max2);

        if (max1 < min2 || max2 < min1) {
            return false; // 存在分离轴，不相交
        }
    }

    // 遍历第二个多边形的所有边
    for (int i = 0; i < numVertices2; ++i) {
        Vector2 edge = vertices2[(i + 1) % numVertices2] - vertices2[i];
        Vector2 axis = edge.perpendicular().normalize();

        float min1, max1, min2, max2;
        project(vertices1, axis, min1, max1);
        project(vertices2, axis, min2, max2);

        if (max1 < min2 || max2 < min1) {
            return false; // 存在分离轴，不相交
        }
    }

    return true; // 所有轴上都有重叠，相交
}

// 主函数示例
int main() {
    Vector2 center1(100, 100);
    float width1 = 50;
    float height1 = 30;
    float angle1 = 0.5; // 弧度

    Vector2 center2(120, 120);
    float width2 = 40;
    float height2 = 20;
    float angle2 = 0.3; // 弧度

    std::vector<Vector2> vertices1 = getVertices(center1, width1, height1, angle1);
    std::vector<Vector2> vertices2 = getVertices(center2, width2, height2, angle2);

    if (separatingAxisTheorem(vertices1, vertices2)) {
        std::cout << "两个旋转矩形相交！" << std::endl;
    } else {
        std::cout << "两个旋转矩形不相交！" << std::endl;
    }

    return 0;
}

*/