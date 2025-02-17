#include "Player.h"

Player::Player(Object* object, Graph<Object*>* sceneGraph)
{
	this->object = sceneGraph->FindObject("penguin");
	this->sceneGraph = sceneGraph;
	position = object->GetPosition();
	rotation = object->GetRotation();
	scale = object->GetScale();
}

void Player::Update()
{
	PauseInput();
	if (!isPlaying)
		return;

	float deltaTime = MyTime::Get().DeltaTime();
	if (!isDashing)
	{
		translation = { 0.f, 0.f, 0.f };
		Gravity();
		Jump();
		Move();
		Rotate();
		Dash();
		translation *= deltaTime * speed;
		translation.y = velocity * deltaTime;
		object->Translate(translation);
	}
	else
	{
		translation.y = 0.f;
		if (translation.norm() <= 0.f)
		{
			isDashing = false;
			return;
		}
		translation = translation.normalize(translation);
		translation = translation * dashForce * deltaTime;
		DashTimer();
		object->Translate(translation);
		hasMoved = true;
	}
	object->SetRotation(rotate);
	position = object->GetPosition();
	rotation = object->GetRotation();
	scale = object->GetScale();
}

void Player::Move()
{
	if (InputManager::GetKey(GLFW_KEY_W))
	{
		translation -= f;
		hasMoved = true;
	}
	if (InputManager::GetKey(GLFW_KEY_S))
	{
		translation += f;
		hasMoved = true;
	}
	if (InputManager::GetKey(GLFW_KEY_D))
	{
		translation += r;
		hasMoved = true;
	}
	if (InputManager::GetKey(GLFW_KEY_A))
	{
		translation -= r;
		hasMoved = true;
	}
}

void Player::Rotate()
{
	if (translation == Vector3D(0.f, 0.f, 0.f ))
		return;
	Vector2D f2D = { f.x, f.z };
	Vector2D translation2D = { -translation.x, -translation.z };
	float angle = translation2D.angleBetween(f2D, translation2D);
	if (translation2D.x < 0.f)
		angle *= -1.f;
	angle = angle * 180.f / M_PI;
	rotate = { 0.f, angle + 180.f, 0.f };
}

void Player::Jump()
{
	if (nbJumps <= 0.f)
		return;
	if (InputManager::IsKeyPressed(GLFW_KEY_SPACE))
	{
		velocity = 0.f;
		velocity += sqrtf((jumpForce * 0.3f) * -3.f * gravity * gravityScale);
		hasMoved = true;
		nbJumps--;
	}
}

void Player::Gravity()
{
	if (isGrounded)
	{
		velocity = 0.f;
		nbJumps = 2.f;
		canDash = true;
	}
	else
	{
		velocity += gravity * gravityScale * MyTime::Get().DeltaTime();
		if (velocity < -50.f)
			velocity = -50.f;
		hasMoved = true;
	}
}

void Player::PauseInput()
{
	if (InputManager::IsKeyPressed(GLFW_KEY_ENTER))
		isPlaying = !isPlaying;
}

void Player::Dash()
{
	if (!canDash)
		return;
	if (InputManager::IsKeyPressed(GLFW_KEY_X))
		isDashing = true;
}

void Player::DashTimer()
{
	if (dashTime > dashTimer)
		dashTimer += MyTime::Get().DeltaTime();
	else
	{
		dashTimer = 0.f;
		isDashing = false;
		canDash = false;
	}
}
