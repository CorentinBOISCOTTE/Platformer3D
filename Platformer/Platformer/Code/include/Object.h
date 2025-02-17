#pragma once
#include <vector>
#include "Math/matrice.h"
#include "Mesh.h"
#include "Collision.h"

class Object
{
public:
	Object(Vector3D position, Vector3D rotation, Vector3D scale, Model* model, Texture* texture, Material* material, Box* box, Sphere* sphere, Object* parent = nullptr);

	Object* parent;
	std::vector<Object*> children;

	mat4x4 worldTransform;
	mat4x4 localTransform;

	Texture* texture;
	Model* model;
	Material* material;
	Box* box;
	Sphere* sphere; 
	Mesh mesh;

	void Update(Camera* camera);
	void Draw(Camera* camera, Shader* shader, std::vector<Light*> lights);
	void Translate(Vector3D translate) { position += translate; }
	void Rotate(Vector3D rotate) { rotation += rotate; }
	void Scale(Vector3D _scale) { scale *= _scale; }
	void SetPosition(Vector3D _position) { position = _position; }
	void SetRotation(Vector3D _rotation) { rotation = _rotation; }
	void SetScale(Vector3D _scale) { scale = _scale; }
	Vector3D GetPosition() { return position; }
	Vector3D GetRotation() { return rotation; }
	Vector3D GetScale() { return scale; }
	Vector3D GetWorldPosition() { return worldPosition; }
	Vector3D GetWorldRotation() { return worldRotation; }
	Vector3D GetWorldScale() { return worldScale; }

private:
	Vector3D position;
	Vector3D rotation;
	Vector3D scale;

	Vector3D worldPosition;
	Vector3D worldRotation;
	Vector3D worldScale;
};