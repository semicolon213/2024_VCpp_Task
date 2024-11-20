#pragma once


#include "framework.h"
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <chrono>

#include "resource.h"
#include "Struct.h"

/// ���� ������Ʈ ����
GameObject player;
std::vector<GameObject> enemies;				///�� ���� ���� ����
std::vector<GameObject> bullets;				///�Ѿ� ���� ����
std::vector<GameObject> obstacles;			///��ֹ� ���� ����
std::vector<Item> items;								///�ʵ� ������ ���� ����

int score = 0;

//ī�޶� ���� ����
float cameraX, cameraY;

GameState gameState = GameState::STARTSCREEN;
std::chrono::steady_clock::time_point gameStartTime;
int damage = 1;
int bulletCnt = 1;
int playerHealth = 10;
int startCntSec = 3;
std::chrono::steady_clock::time_point countdownStartTime;

void updateGame();
void spawnEnemy();
void shootBullet();
void updateCamera();
void initializeObstacles();
void spawnItem();
bool checkCollision(double, double, LONG, LONG, double, double, LONG, LONG);
void applyItemEffect(ItemType type);
void startGame();
void ResetGame();