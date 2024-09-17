#include "raylib.h"
#include <iostream>
#include <optional>
#include <cstdio>
#include <string>
#include <vector>
#include <memory>

#define BALL_SPEED 150


Vector2 screenDimensions = { 1000, 800 };

class Game {
public:
	void GameOver() {

	}


};

const char* intToConstChar(int value) {
	const int bufferSize = 12;
	char* buffer = new char[bufferSize];
	snprintf(buffer, bufferSize, "%d", value);
	return buffer;
}


class Shape {

};


struct Circle {
	float x;
	float y;
	float radius;
};

class Block : Shape {
public:
	Block(Rectangle rec) : m_Rec(rec) {};

	Rectangle m_Rec;

	bool isVisible = true;

	static std::vector<Block*> blocks;

	static void updateBlocks() {
		for (int i = 0; i < Block::blocks.size(); i++)
		{
			if (Block::blocks[i] == nullptr) {
				continue;
			}
			if (Block::blocks[i]->isVisible) {
				DrawRectangle(blocks[i]->m_Rec.x, blocks[i]->m_Rec.y, blocks[i]->m_Rec.width, blocks[i]->m_Rec.height, GREEN);
			}

		}
	}

};
std::vector<Block*> Block::blocks;


class Player {
public:
	Player(Rectangle rec) : m_Rec(rec), m_X(m_Rec.x), m_Y(m_Rec.y) {}

	Rectangle m_Rec;
	float& m_X;
	float& m_Y;


	std::optional<bool> goingRight;
	bool canMove = false;


	void update() {

		handleMovement();

		draw();
	}

	void handleMovement() {

		if (canMove) {
			if (IsKeyDown(KEY_A)) {
				goingRight = false;
			}
			if (IsKeyDown(KEY_D)) {
				goingRight = true;
			}

			if (!goingRight.has_value()) {
				return;
			}
			if (goingRight.value() == true) {
				if (m_X + m_Rec.width + 400 * GetFrameTime() < 1000)
					m_X += 400 * GetFrameTime();
			}
			if (goingRight.value() == false) {
				if (m_X - 400 * GetFrameTime() > 0)
					m_X -= 400 * GetFrameTime();
			}
		}
	}

	void draw() {
		DrawRectangleRec(this->m_Rec, RED);

	}

};



class Ball {
public:
	Ball(Circle circle) : m_Circle(circle), m_X(m_Circle.x), m_Y(m_Circle.y) {};


	void handleMovement() {
		if (canMove) {

			if (movingRight != true && movingRight != false)
			{
				std::srand(time(0));
				int randomValue = rand() % 2;
				movingRight = randomValue;
				movingUp = false;
			}
			if (movingRight && !movingUp) {
				m_X += BALL_SPEED * GetFrameTime();
				m_Y += BALL_SPEED * GetFrameTime();
			}
			if (!movingRight && !movingUp) {
				m_X -= BALL_SPEED * GetFrameTime();
				m_Y += BALL_SPEED * GetFrameTime();
			}
			if (movingRight && movingUp) {
				m_X += BALL_SPEED * GetFrameTime();
				m_Y -= BALL_SPEED * GetFrameTime();
			}
			if (!movingRight && movingUp) {
				m_X -= BALL_SPEED * GetFrameTime();
				m_Y -= BALL_SPEED * GetFrameTime();
			}

			if (m_X + m_Circle.radius >= screenDimensions.x) {
				movingRight = false;
			}
			if (m_X - m_Circle.radius <= 0) {
				movingRight = true;
			}
			if (m_Y - m_Circle.radius <= 0) {
				resetPosition();
				movingUp = false;
			}

			for (Block* block : Block::blocks) {
				if (block->isVisible && m_Y - m_Circle.radius <= block->m_Rec.y + block->m_Rec.height && m_Y >= block->m_Rec.y && m_X + m_Circle.radius <= block->m_Rec.x + block->m_Rec.width && m_X + m_Circle.radius >= block->m_Rec.x) {
					//bottom side
					block->isVisible = false;
					movingUp = false;
				}
				if (block->isVisible && m_Y + m_Circle.radius >= block->m_Rec.y && m_Y + m_Circle.radius <= block->m_Rec.y && m_X + m_Circle.radius >= block->m_Rec.x && m_X <= block->m_Rec.x + block->m_Rec.width)
				{
					//left side
					movingRight = false;
				}
			}

		}
	}

	void resetPosition() {
		m_X = screenDimensions.x / 2;
		m_Y = screenDimensions.y - 300;
	}

	void update() {

		handleMovement();
		draw();
	}

	void draw() {
		DrawCircle(m_X, m_Y, m_Circle.radius, BLUE);

	}


	bool movingUp;
	bool movingRight;
	bool canMove = false;
	Circle m_Circle;
	float& m_X;
	float& m_Y;

};


void updateRelations(Player& player, Ball& ball) {
	if (ball.m_Y + ball.m_Circle.radius >= player.m_Y && ball.m_Y - ball.m_Circle.radius <= player.m_Y + player.m_Rec.height && ball.m_X <= player.m_X + player.m_Rec.width && ball.m_X >= player.m_X) {
		if (player.goingRight.value()) {
			ball.movingRight = true;
			ball.movingUp = true;
		}
		if (!player.goingRight.value()) {
			ball.movingRight = false;
			ball.movingUp = true;
		}
	}
}



int main() {

	for (int y = 0; y < 5; y++) {
		for (int x = 0; x < 10; x++) {
			Rectangle rec = { x == 0 ? 50 : (90 * x) + 50 , y == 0 ? 30 : (40 * y) + 30, 60, 20 };
			Block* block = new Block(rec);
			Block::blocks.push_back(block);
			std::cout << "initializing <<  " << y << std::endl;
		}
	}



	bool m_Started = false;
	int score = 0;








	InitWindow(screenDimensions.x, screenDimensions.y, "odk");
	SetTargetFPS(60);






	Player player({ (screenDimensions.x / 2) - 50, (screenDimensions.y - 100), 100, 30 });
	Ball ball({ (screenDimensions.x / 2), screenDimensions.y - 300, 10 });




	while (!WindowShouldClose()) {
		BeginDrawing();

		ClearBackground(WHITE);

		DrawFPS(screenDimensions.x - 80, 10);
		Block::updateBlocks();

		player.update();
		ball.update();
		updateRelations(player, ball);


		if (!m_Started) {
			DrawRectangle(0, 0, 1000, 800, { 0, 0, 0, 230 });
			DrawText("Press space to start playing...", (screenDimensions.x / 2) - 300, screenDimensions.y / 2, 40, WHITE);
			if (IsKeyDown(KEY_SPACE)) {
				m_Started = !m_Started;
				player.canMove = true;
				ball.canMove = true;
			}
		}

		if (player.m_X + player.m_Rec.width >= screenDimensions.x || player.m_X <= 0) {
			player.goingRight.reset();
		}



		EndDrawing();
	}
}