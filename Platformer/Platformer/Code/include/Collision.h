#pragma once
#include "Math/matrice.h"
#include <vector>
#include <sstream>
#include <algorithm>
#include "Debug.h"

class Sphere {
public:
	Sphere() = default; 
	Sphere(Vector3D _position, float _radius);
	Vector3D position;
	float radius; 
};

class Quad
{
public:
	Quad() = default;
	Quad(Vector3D point1, Vector3D point2, Vector3D point3);
	void Print();
	Vector3D a, b, c;
	Vector3D u, v;
};

class Box {
public:
	Box() = default; 
	Box(Vector3D _position, float _width, float _height, float _depth);
	Vector3D position; 
	float width, height, depth; 
	std::vector<Quad> quad; 
	void initBox();
};

class Segment
{
public:
	Segment() = default;
	Segment(Vector3D _startingPoint, Vector3D _endingPoint);
	Vector3D startingPoint;
	Vector3D endingPoint;
	Vector3D u;
};

class InfiniteCylinder
{
public:
	InfiniteCylinder() = default;
	InfiniteCylinder(Vector3D point, Vector3D _v, float radius);
	Vector3D c;
	Vector3D v;
	float r;
};

class Cylinder
{
public:
	Cylinder() = default;
	Cylinder(Vector3D point1, Vector3D point2, Vector3D _v, float radius);
	Vector3D c, d;
	Vector3D v;
	float r;
};

class Capsule
{
public:
	Capsule() = default;
	Capsule(Sphere sphere1, Sphere sphere2, Cylinder cylinder);
	Sphere s1, s2;
	Cylinder c;
};

class Plane
{
public:
	Plane() = default;
	Plane(Vector3D _point1, Vector3D _point2, Vector3D _point3);
	Vector3D point1, point2, point3;
	Vector3D normal;
	float a, b, c, d = 0.f;
	bool incorrectPlane = false;
};

bool isPointInsideBox(const Vector3D& point, const Box& box); 
bool checkCollisionBoxSphere(const Box* box, const Sphere* sphere);
bool CheckCollisionSegmentPlane(Segment segment, Plane plane, float& t0);
bool CheckCollisionSegmentQuad(Segment segment, Quad quad, float& t0);
bool CheckCollisionSegmentSphere(Segment segment, Sphere sphere, float& t0);
bool CheckCollisionSegmentInfiniteCylinder(Segment segment, InfiniteCylinder cylinder, float& t0);
bool CheckCollisionSegmentCylinder(Segment segment, Cylinder cylinder, float& t0);
bool CheckCollisionSegmentCapsule(Segment segment, Capsule capsule, float& t0);
Vector3D GetCollisionPoint(float t0, Segment segment);
void correctSpherePositionAfterCollision(Box* box, Sphere* sphere);
Box* calculateBoundingBox(const std::vector<Vector3D>& vertices); 
Sphere* calculateBoundingSphere(const std::vector<Vector3D>& vertices);
std::vector<Vector3D> loadObj(const std::string & filepath); 