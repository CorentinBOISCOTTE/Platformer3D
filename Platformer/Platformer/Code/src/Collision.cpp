#include "Collision.h"

void Box::initBox()
{
    quad.clear();
    Vector3D a = position;

    //Top Face
    a.x -= width * 0.5f;
    a.y += height * 0.5f;
    a.z -= depth * 0.5f;
    Vector3D b = a;
    b.x += width;
    Vector3D c = a;
    c.z += depth;
    Quad topFace(a, b, c);
    quad.push_back(topFace);
    
    a = position;
    //Bottom Face
    a.x -= width * 0.5f;
    a.y -= height * 0.5f;
    a.z -= depth * 0.5f;
    b = a;
    c = a;
    b.x += width;
    c.z += depth;
    Quad bottomFace(a, b, c);
    quad.push_back(bottomFace);

    a = position;
    //Front Face
    a.x -= width * 0.5f;
    a.y -= height * 0.5f;
    a.z -= depth * 0.5f;
    b = a;
    c = a;
    b.x += width;
    c.z += height;
    Quad frontFace(a, b, c);
    quad.push_back(frontFace);

    a = position;
    //Back Face
    a.x += width * 0.5f;
    a.y -= height * 0.5f;
    a.z += depth * 0.5f;
    b = a;
    c = a;
    b.x += width;
    c.z += height;
    Quad backFace(a, b, c);
    quad.push_back(backFace);

    a = position;
    //Right Face
    a.x -= width * 0.5f;
    a.y -= height * 0.5f;
    a.z += depth * 0.5f;
    b = a;
    c = a;
    b.x -= depth;
    c.z += height;
    Quad rightFace(a, b, c);
    quad.push_back(rightFace);

    a = position;
    //Left Face
    a.x += width * 0.5f;
    a.y -= height * 0.5f;
    a.z += depth * 0.5f;
    b = a;
    c = a;
    b.x -= depth;
    c.z += height;
    Quad leftFace(a, b, c);
    quad.push_back(leftFace); 
}

bool isPointInsideBox(const Vector3D& point, const Box& box)
{
    float halfWidth = box.width / 2.0f;
    float halfHeight = box.height / 2.0f;
    float halfDepth = box.depth / 2.0f; 

    return (point.x >= box.position.x - halfWidth && point.x <= box.position.x + halfWidth) &&
        (point.y >= box.position.y - halfHeight && point.y <= box.position.y + halfHeight) &&
        (point.z >= box.position.z - halfDepth && point.z <= box.position.z + halfDepth);

    return false;
}

bool checkCollisionBoxSphere(const Box* box, const Sphere* sphere)
{
    Box expandedBox = { box->position, box->width + 2 * sphere->radius, box->height + 2 * sphere->radius, box->depth + 2 * sphere->radius }; 
    
    return isPointInsideBox(sphere->position, expandedBox);
}

bool CheckCollisionSegmentPlane(Segment segment, Plane plane, float& t0)
{
    if (plane.incorrectPlane)
        return false;
    Vector3D OA = segment.startingPoint;
    Vector3D n = plane.normal;
    Vector3D u = segment.u;
    float d = plane.d;
    if (n.dotProduct(u) == 0.f)
        return false;
    t0 = -((OA.dotProduct(n) + d) / u.dotProduct(n));
    if (t0 >= 0.f && t0 <= 1.f)
        return true;
    return false;
}

Vector3D GetCollisionPoint(float t0, Segment segment)
{
    Vector3D collisionPoint;

    collisionPoint.x = segment.startingPoint.x + (segment.u.x * t0);
    collisionPoint.y = segment.startingPoint.y + (segment.u.y * t0);
    collisionPoint.z = segment.startingPoint.z + (segment.u.z * t0);

    return collisionPoint;
}

bool CheckCollisionSegmentQuad(Segment segment, Quad quad, float& t0)
{
    Plane plane(quad.a, quad.b, quad.c);
    if (!CheckCollisionSegmentPlane(segment, plane, t0))
        return false;
    Vector3D collisionPoint = GetCollisionPoint(t0, segment);
    float checkX = (collisionPoint - quad.a).dotProduct(quad.v.normalize(quad.v));
    float checkZ = (collisionPoint - quad.a).dotProduct(quad.u.normalize(quad.u));
    float lX = quad.v.norm();
    float lZ = quad.u.norm();
    if (checkX >= 0 && checkX <= lX && checkZ >= 0 && checkZ <= lZ)
        return true;
    return false;
}

bool CheckCollisionSegmentSphere(Segment segment, Sphere sphere, float& t0)
{
    Vector3D CA = segment.startingPoint - sphere.position;
    float a = powf(segment.u.norm(), 2.f);
    float b = 2.f * segment.u.dotProduct(CA);
    float c = powf(CA.norm(), 2.f) - powf(sphere.radius, 2.f);
    if (a < 0.f || c < 0.f)
        return false;
    float delta = powf(b, 2.f) - (4.f * a * c);
    if (delta < 0.f)
        return false;
    float t01 = (-b - sqrtf(delta)) / (2.f * a);
    float t02 = (-b + sqrtf(delta)) / (2.f * a);
    bool ist01Good = false;
    bool ist02Good = false;
    if (t01 >= 0.f && t01 <= 1.f)
        ist01Good = true;
    if (t02 >= 0.f && t02 <= 1.f)
        ist02Good = true;

    if (ist01Good && ist02Good)
    {
        t0 = std::min(t01, t02);
        return true;
    }
    else if (ist01Good)
    {
        t0 = t01;
        return true;
    }
    else if (ist02Good)
    {
        t0 = t02;
        return true;
    }
    return false;
}

bool CheckCollisionSegmentInfiniteCylinder(Segment segment, InfiniteCylinder cylinder, float& t0)
{
    Vector3D A = segment.startingPoint;
    Vector3D C = cylinder.c;
    Vector3D u = segment.u;
    Vector3D v = cylinder.v;
    float r = cylinder.r;
    float X = A.x - C.x;
    float Y = A.y - C.y;
    float Z = A.z - C.z;
    Vector3D omega1(X * v.x, Y * v.y, Z * v.z);
    Vector3D omega2(Y * v.y, Z * v.z, X * v.x);
    Vector3D omega3(Z * v.z, X * v.x, Y * v.y);
    Vector3D omega4(X * u.x, Y * u.y, Z * u.z);
    Vector3D omega5(u.x * v.x, u.y * v.y, u.z * v.z);
    Vector3D omega6((v.y * v.y) + (v.z * v.z), (v.x * v.x) + (v.z * v.z), (v.x * v.x) + (v.y * v.y));
    Vector3D omega7(X * X, Y * Y, Z * Z);
    float a = powf(u.crossProduct(u, v).norm(), 2.f) / powf(v.norm(), 2.f);
    float b = 2.f * (omega4.dotProduct(omega6) - omega5.dotProduct(omega2 + omega3)) / powf(v.norm(), 2.f);
    float c = ((omega7.dotProduct(omega6) - (2.f * omega1.dotProduct(omega2))) / powf(v.norm(), 2.f)) - powf(r, 2.f);
    float delta = powf(b, 2.f) - (4.f * a * c);
    if (delta < 0.f)
        return false;
    float t01 = (-b - sqrtf(delta)) / (2.f * a);
    float t02 = (-b + sqrtf(delta)) / (2.f * a);
    bool ist01Good = false;
    bool ist02Good = false;
    if (t01 >= 0.f && t01 <= 1.f)
        ist01Good = true;
    if (t02 >= 0.f && t02 <= 1.f)
        ist02Good = true;

    if (ist01Good && ist02Good)
    {
        t0 = std::min(t01, t02);
        return true;
    }
    else if (ist01Good)
    {
        t0 = t01;
        return true;
    }
    else if (ist02Good)
    {
        t0 = t02;
        return true;
    }
    return false;
}

bool CheckCollisionSegmentCylinder(Segment segment, Cylinder cylinder, float& t0)
{
    InfiniteCylinder infiniteCylinder(cylinder.c, cylinder.v, cylinder.r);
    if (!CheckCollisionSegmentInfiniteCylinder(segment, infiniteCylinder, t0))
        return false;
    Vector3D A = segment.startingPoint;
    Vector3D C = cylinder.c;
    Vector3D D = cylinder.d;
    Vector3D u = segment.u;
    Vector3D CPt0(A.x + (t0 * u.x) - C.x, A.y + (t0 * u.y) - C.y, A.z + (t0 * u.z) - C.z);
    Vector3D DPt0(A.x + (t0 * u.x) - D.x, A.y + (t0 * u.y) - D.y, A.z + (t0 * u.z) - D.z);
    Vector3D CD = D - C;
    Vector3D DC = C - D;
    if (CPt0.norm() <= DPt0.norm())
    {
        if (CD.dotProduct(CPt0) >= 0.f)
            return true;
        return false;
    }
    else
    {
        if (DC.dotProduct(DPt0) >= 0.f)
            return true;
        return false;
    }
}

bool CheckCollisionSegmentCapsule(Segment segment, Capsule capsule, float& t0)
{
    if (CheckCollisionSegmentCylinder(segment, capsule.c, t0))
        return true;
    else if (CheckCollisionSegmentSphere(segment, capsule.s1, t0))
        return true;
    else if (CheckCollisionSegmentSphere(segment, capsule.s2, t0))
        return true;
    return false;
}

void correctSpherePositionAfterCollision(Box* box, Sphere* sphere) {
    if (!checkCollisionBoxSphere(box, sphere))
        return;

    float halfWidth = box->width / 2.0f;
    float halfHeight = box->height / 2.0f;
    float halfDepth = box->depth / 2.0f;

    Vector3D minBox = { box->position.x - halfWidth, box->position.y - halfHeight, box->position.z - halfDepth };
    Vector3D maxBox = { box->position.x + halfWidth, box->position.y + halfHeight, box->position.z + halfDepth };

    Vector3D closestPoint = sphere->position;
    closestPoint.x = std::max(minBox.x, std::min(sphere->position.x, maxBox.x));
    closestPoint.y = std::max(minBox.y, std::min(sphere->position.y, maxBox.y));
    closestPoint.z = std::max(minBox.z, std::min(sphere->position.z, maxBox.z));

    float distanceSquared = (closestPoint.x - sphere->position.x) * (closestPoint.x - sphere->position.x) +
        (closestPoint.y - sphere->position.y) * (closestPoint.y - sphere->position.y) +
        (closestPoint.z - sphere->position.z) * (closestPoint.z - sphere->position.z);

    if (distanceSquared <= sphere->radius * sphere->radius) {
        float distance = std::sqrt(distanceSquared);

        if (distance > 0.0f) {
            float penetrationDepth = sphere->radius - distance;
            Vector3D collisionNormal = {
                (sphere->position.x - closestPoint.x) / distance,
                (sphere->position.y - closestPoint.y) / distance,
                (sphere->position.z - closestPoint.z) / distance
            };
            sphere->position.x += collisionNormal.x * penetrationDepth;
            sphere->position.y += collisionNormal.y * penetrationDepth;
            sphere->position.z += collisionNormal.z * penetrationDepth;
        }
        else {
            if (sphere->position.x < box->position.x) sphere->position.x -= sphere->radius;
            else sphere->position.x += sphere->radius;
            if (sphere->position.y < box->position.y) sphere->position.y -= sphere->radius;
            else sphere->position.y += sphere->radius;
            if (sphere->position.z < box->position.z) sphere->position.z -= sphere->radius;
            else sphere->position.z += sphere->radius;
        }
    }
}


Box* calculateBoundingBox(const std::vector<Vector3D>& vertices) {
    ASSERT(!vertices.empty(), "The vertex list is empty.");

    Vector3D minVertex = vertices[0];
    Vector3D maxVertex = vertices[0];

    for (const auto& vertex : vertices) {
        minVertex.x = std::min(minVertex.x, vertex.x);
        minVertex.y = std::min(minVertex.y, vertex.y);
        minVertex.z = std::min(minVertex.z, vertex.z);

        maxVertex.x = std::max(maxVertex.x, vertex.x);
        maxVertex.y = std::max(maxVertex.y, vertex.y);
        maxVertex.z = std::max(maxVertex.z, vertex.z);
    }

    Vector3D center;
    center.x = (minVertex.x + maxVertex.x) / 2.0f;
    center.y = (minVertex.y + maxVertex.y) / 2.0f;
    center.z = (minVertex.z + maxVertex.z) / 2.0f;

    float width = maxVertex.x - minVertex.x;
    float height = maxVertex.y - minVertex.y;
    float depth = maxVertex.z - minVertex.z;

    Box* box = new Box(center, width, height, depth);

    return box;
}

Sphere* calculateBoundingSphere(const std::vector<Vector3D>& vertices) {
    ASSERT(!vertices.empty(), "The vertex list is empty.");

    Vector3D minVertex = vertices[0];
    Vector3D maxVertex = vertices[0];

    for (const auto& vertex : vertices) {
        minVertex.x = std::min(minVertex.x, vertex.x);
        minVertex.y = std::min(minVertex.y, vertex.y);
        minVertex.z = std::min(minVertex.z, vertex.z);

        maxVertex.x = std::max(maxVertex.x, vertex.x);
        maxVertex.y = std::max(maxVertex.y, vertex.y);
        maxVertex.z = std::max(maxVertex.z, vertex.z);
    }

    float xdiff = maxVertex.x - minVertex.x;
    float ydiff = maxVertex.y - minVertex.y;
    float zdiff = maxVertex.z - minVertex.z;

    Vector3D center;
    center.x = (minVertex.x + maxVertex.x) / 2.0f;
    center.y = (minVertex.y + maxVertex.y) / 2.0f;
    center.z = (minVertex.z + maxVertex.z) / 2.0f;

    float diameter = std::max(xdiff, std::max(ydiff, zdiff)); 
    float radius = diameter / 2;    

    Sphere* sphere = new Sphere(center, radius);

    return sphere; 
}

std::vector<Vector3D> loadObj(const std::string& filepath) {
    std::vector<Vector3D> vertices;
    std::ifstream file(filepath); 

    if (!file.is_open()) {
        throw std::invalid_argument("Could not open file: " + filepath);
    }

    std::string line;
    while (getline(file, line)) {
        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;

        if (prefix == "v") {
            Vector3D vertex;
            iss >> vertex.x >> vertex.y >> vertex.z;
            vertices.push_back(vertex);
        }
    }

    file.close();
    return vertices; 
}

Plane::Plane(Vector3D _point1, Vector3D _point2, Vector3D _point3)
{
    point1 = _point1;
    point2 = _point2;
    point3 = _point3;
    Vector3D AB = point2 - point1;
    Vector3D AC = point3 - point1;
    if ((AB.x / AC.x) == (AB.y / AC.y) && (AB.x / AC.x) == (AB.z / AC.z))
    {
        std::cout << "The 3 points are aligned, cannot define a plane" << std::endl;
        incorrectPlane = true;
        return;
    }
    normal = AB.crossProduct(AB, AC);
    a = normal.x;
    b = normal.y;
    c = normal.z;
    d = -(a * point1.x) - (b * point1.y) - (c * point1.z);
}

Segment::Segment(Vector3D _startingPoint, Vector3D _endingPoint)
{
    startingPoint = _startingPoint;
    endingPoint = _endingPoint;
    u = { endingPoint.x - startingPoint.x, endingPoint.y - startingPoint.y, endingPoint.z - startingPoint.z };
}

Quad::Quad(Vector3D point1, Vector3D point2, Vector3D point3)
{
    a = point1;
    b = point2;
    c = point3;
    u = b - a;
    v = c - a;
}

void Quad::Print()
{
    std::cout << "Quad : a = " << a.x << "; " << a.y << "; " << a.z;
    std::cout << "; b = " << b.x << "; " << b.y << "; " << b.z;
    std::cout << "; c = " << c.x << "; " << c.y << "; " << c.z << std::endl;
}

InfiniteCylinder::InfiniteCylinder(Vector3D point, Vector3D _v, float radius)
{
    c = point;
    v = _v;
    r = radius;
}

Cylinder::Cylinder(Vector3D point1, Vector3D point2, Vector3D _v, float radius)
{
    c = point1;
    d = point2;
    v = _v;
    r = radius;
}

Capsule::Capsule(Sphere sphere1, Sphere sphere2, Cylinder cylinder)
{
    s1 = sphere1;
    s2 = sphere2;
    c = cylinder;
}

Sphere::Sphere(Vector3D _position, float _radius)
{
    position = _position; 
    radius = _radius;
}

Box::Box(Vector3D _position, float _width, float _height, float _depth)
{
    position = _position;
    width = _width;
    height = _height;
    depth = _depth;
    initBox();
}
