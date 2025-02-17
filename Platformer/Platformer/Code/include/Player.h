#pragma once
#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "Debug.h"
#include "Graph.h"
#include "Collision.h"
#include "InputManager.h"

class Player
{
public:
	Player() = default;
	Player(Object* object, Graph<Object*>* sceneGraph);
	~Player() = default;
	void Update();

	Vector3D position, rotation, scale;
	Vector3D translation;
	Graph<Object*>* sceneGraph;
	float speed = 10.f;
	float gravityScale = 1.0f;
	float jumpForce = 1.f;
	float dashForce = 50.f;
	bool hasMoved = true;
	Object* object;
	bool isGrounded = false;
	bool isPlaying = true;

private:
	void Move();
	void Rotate();
	void Jump();
	void Gravity();
	void PauseInput();
	void Dash();
	void DashTimer();

	Vector3D f = { 0.f, 0.f, 1.f };
	Vector3D r = { 1.f, 0.f, 0.f };

	Vector3D rotate;

	float gravity = -9.81f;
	float velocity = 0.f;
	float nbJumps = 2.f;
	float dashTime = 0.125f;
	float dashTimer = 0.f;

	bool isDashing = false;
	bool canDash = false;
};