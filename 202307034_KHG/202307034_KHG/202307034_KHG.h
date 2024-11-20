#pragma once


#include "framework.h"
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <chrono>

#include "resource.h"
#include "Struct.h"

/// 게임 오브젝트 선언
GameObject player;
std::vector<GameObject> enemies;				///적 생성 정보 벡터
std::vector<GameObject> bullets;				///총알 정보 벡터
std::vector<GameObject> obstacles;			///장애물 정보 벡터
std::vector<Item> items;								///필드 아이템 정보 벡터

int score = 0;

//카메라 관련 변수
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