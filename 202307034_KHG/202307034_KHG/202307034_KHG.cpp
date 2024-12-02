#include "202307034_KHG.h"

#define MAX_LOADSTRING 100

HINSTANCE hInst;
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];

HDC hdcBuffer = NULL;
HBITMAP hbmBuffer = NULL;
int bufferWidth, bufferHeight;

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_MY202307034KHG, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY202307034KHG));

	MSG msg;

	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY202307034KHG));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_MY202307034KHG);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance;

	RECT windowRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	AdjustWindowRect(&windowRect, WS_OVERLAPPED | WS_SYSMENU, TRUE);

	int windowWidth = windowRect.right - windowRect.left + 20;
	int windowHeight = windowRect.bottom - windowRect.top + 40;

	HWND hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPED | WS_SYSMENU,
		CW_USEDEFAULT, 0, windowWidth, windowHeight, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		KillTimer(hWnd, 1);
		KillTimer(hWnd, 2);
		if (hdcBuffer) DeleteDC(hdcBuffer);
		if (hbmBuffer) DeleteObject(hbmBuffer);
		PostQuitMessage(0);
		break;
	case WM_CREATE:
	{
		mainhWnd = hWnd;
		SetTimer(hWnd, 1, 16, NULL);  // 약 60 FPS
		SetTimer(hWnd, 2, 1000 / static_cast<int>(bulletFireRate), NULL);  // 총알 발사 타이머
		ResetGame();

		HDC hdc = GetDC(hWnd);
		bufferWidth = SCREEN_WIDTH;
		bufferHeight = SCREEN_HEIGHT;
		hdcBuffer = CreateCompatibleDC(hdc);
		hbmBuffer = CreateCompatibleBitmap(hdc, bufferWidth, bufferHeight);
		SelectObject(hdcBuffer, hbmBuffer);
		ReleaseDC(hWnd, hdc);
	}
	break;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		BitBlt(hdc, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, hdcBuffer, 0, 0, SRCCOPY);
		EndPaint(hWnd, &ps);
	}
	break;

	case WM_LBUTTONDOWN:
	{
		int mouseX = HIWORD(lParam);
		int mouseY = LOWORD(lParam);

		if (gameState == GameState::STARTSCREEN) {
			RECT clientRect;
			GetClientRect(hWnd, &clientRect);
			int centerX = (clientRect.right - clientRect.left) / 2;
			int centerY = (clientRect.bottom - clientRect.top) / 2;
			RECT startButton = { centerX - 100, centerY + 50, centerX + 100, centerY + 100 };

			if (mouseX >= startButton.left && mouseX <= startButton.right &&
				mouseY >= startButton.top && mouseY <= startButton.bottom) {
				startGame();
				gameState = GameState::COUNTDOWN;
				countdownStartTime = std::chrono::steady_clock::now();
			}
		}
		else if (gameState == GameState::GAMEOVER) {
			RECT clientRect;
			GetClientRect(hWnd, &clientRect);
			int centerX = (clientRect.right - clientRect.left) / 2;
			int centerY = (clientRect.bottom - clientRect.top) / 2;
			RECT restartButton = { centerX - 100, centerY + 50, centerX + 100, centerY + 100 };

			if (mouseX >= restartButton.left && mouseX <= restartButton.right &&
				mouseY >= restartButton.top && mouseY <= restartButton.bottom) {
				ResetGame();
				gameState = GameState::COUNTDOWN;
				countdownStartTime = std::chrono::steady_clock::now();
			}
		}
	}
	break;

	case WM_ACTIVATEAPP:
	{
		if (wParam)
			SetTimer(hWnd, 0, 50, NULL);
		else
			KillTimer(hWnd, 0);
	}
	break;
	case WM_TIMER:
		if (wParam == 0)
		{
			if (gameState == GameState::PLAY)
			{
				if (GetKeyState(VK_LEFT) & 0x8000)
				{
					double newX = max(0, player.x - player.speed);
					if (!checkCollisionWithObstacles(newX, player.y, PLAYER_SIZE, PLAYER_SIZE)) {
						player.x = newX;
					}
				}
				if (GetKeyState(VK_RIGHT) & 0x8000)
				{
					double newX = min(MAP_WIDTH - PLAYER_SIZE, player.x + player.speed);
					if (!checkCollisionWithObstacles(newX, player.y, PLAYER_SIZE, PLAYER_SIZE)) {
						player.x = newX;
					}
				}
				if (GetKeyState(VK_UP) & 0x8000)
				{
					double newY = max(0, player.y - player.speed);
					if (!checkCollisionWithObstacles(player.x, newY, PLAYER_SIZE, PLAYER_SIZE)) {
						player.y = newY;
					}
				}
				if (GetKeyState(VK_DOWN) & 0x8000)
				{
					double newY = min(MAP_HEIGHT - PLAYER_SIZE, player.y + player.speed);
					if (!checkCollisionWithObstacles(player.x, newY, PLAYER_SIZE, PLAYER_SIZE)) {
						player.y = newY;
					}
				}
				if (GetKeyState(VK_F1) & 0x8000)
				{
					bulletFireRate = MAX_FIRE_RATE;
					damage = 10000000;
					KillTimer(mainhWnd, 2);
					SetTimer(hWnd, 2, 1000 / static_cast<int>(bulletFireRate), NULL);  // 총알 발사 타이머
				}
				if (GetKeyState(VK_F2) & 0x8000)
				{
					enemySpawnMax = !enemySpawnMax;
				}

				updateCamera();
			}
		}
		if (wParam == 1) {
			if (gameState == GameState::PLAY) {
				updateGame();
			}
			drawGame(hdcBuffer);
			InvalidateRect(hWnd, NULL, FALSE);
		}
		if (wParam == 2) {
			if (gameState == GameState::PLAY) {
				shootBulletInAllDirections();
			}
		}
		break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}
/**
 * @brief WM_PAINT 호출 함수
 *
 * 화면에 아이템, 플레이어, 장애물, 적, 미니맵을 그리는 함수
 *
 * @param hdc WM_PAINT device context 핸들러
 */
void drawGame(HDC hdc)
{
	// 배경 그리기
	RECT clientRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	HBRUSH backgroundBrush = CreateSolidBrush(RGB(240, 240, 240));
	FillRect(hdc, &clientRect, backgroundBrush);
	DeleteObject(backgroundBrush);
	HFONT hFont = CreateFont(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
		OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("맑은 고딕"));
	HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);
	switch (gameState) {
	case GameState::STARTSCREEN:
	{
		SetTextAlign(hdc, TA_CENTER | TA_BASELINE);
		SetBkMode(hdc, TRANSPARENT);
		int centerX = SCREEN_WIDTH / 2;
		int centerY = SCREEN_HEIGHT / 2;

		// 게임 제목
		HFONT hTitleFont = CreateFont(60, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
			OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("맑은 고딕"));
		SelectObject(hdc, hTitleFont);
		SetTextColor(hdc, RGB(50, 50, 50));
		TextOut(hdc, centerX, centerY - 50, L"유한 특공대", 6);
		DeleteObject(hTitleFont);

		// 시작 버튼
		RECT startButton = { centerX - 100, centerY + 50, centerX + 100, centerY + 100 };
		HBRUSH buttonBrush = CreateSolidBrush(RGB(200, 200, 200));
		FillRect(hdc, &startButton, buttonBrush);
		DeleteObject(buttonBrush);
		FrameRect(hdc, &startButton, (HBRUSH)GetStockObject(BLACK_BRUSH));
		SelectObject(hdc, hFont);
		SetTextColor(hdc, RGB(0, 0, 0));
		TextOut(hdc, centerX, centerY + 82, L"게임 시작", 5);
	}
	break;

	case GameState::COUNTDOWN:
	{
		SetTextAlign(hdc, TA_CENTER | TA_BASELINE);
		SetBkMode(hdc, TRANSPARENT);
		int centerX = SCREEN_WIDTH / 2;
		int centerY = SCREEN_HEIGHT / 2;
		HFONT hCountdownFont = CreateFont(100, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
			OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("맑은 고딕"));
		SelectObject(hdc, hCountdownFont);
		auto now = std::chrono::steady_clock::now();
		auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - countdownStartTime).count();
		int remainingSeconds = COUNTDOWN_SECONDS - static_cast<int>(elapsed);
		if (remainingSeconds > 0) {
			SetTextColor(hdc, RGB(50, 50, 50));
			std::wstring countdownText = std::to_wstring(remainingSeconds);
			TextOut(hdc, centerX, centerY, countdownText.c_str(), countdownText.length());
		}
		else {
			gameState = GameState::PLAY;
			startGame();
		}
		DeleteObject(hCountdownFont);
	}
	break;

	case GameState::PLAY:
	{
		// 플레이어 그리기
		HBRUSH playerBrush = CreateSolidBrush(RGB(0, 100, 200));
		if (!isPlayerInvincible() || (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - player.invincibleUntil).count() / 100) % 2 == 0) {
			RECT playerRect = {
			static_cast<int>(player.x - cameraX),
			static_cast<int>(player.y - cameraY),
			static_cast<int>(player.x - cameraX + PLAYER_SIZE),
			static_cast<int>(player.y - cameraY + PLAYER_SIZE)
			};
			FillRect(hdc, &playerRect, playerBrush);
			DeleteObject(playerBrush);

		}

		// 적 그리기
		HBRUSH enemyBrush = CreateSolidBrush(RGB(200, 50, 50));
		for (const auto& enemy : enemies) {
			if (enemy.isActive) {
				RECT enemyRect = {
					static_cast<int>(enemy.x - cameraX),
					static_cast<int>(enemy.y - cameraY),
					static_cast<int>(enemy.x - cameraX + ENEMY_SIZE),
					static_cast<int>(enemy.y - cameraY + ENEMY_SIZE)
				};
				if (enemyRect.left < SCREEN_WIDTH && enemyRect.right > 0 &&
					enemyRect.top < SCREEN_HEIGHT && enemyRect.bottom > 0) {
					FillRect(hdc, &enemyRect, enemyBrush);
					std::wstring healthText = std::to_wstring(enemy.health);
					SetTextAlign(hdc, TA_CENTER | TA_BASELINE);
					SetBkMode(hdc, TRANSPARENT);
					SetTextColor(hdc, RGB(255, 255, 255));
					TextOut(hdc,
						static_cast<int>(enemy.x - cameraX + ENEMY_SIZE / 2),
						static_cast<int>(enemy.y - cameraY + ENEMY_SIZE / 2),
						healthText.c_str(),
						healthText.length());
					SetTextAlign(hdc, TA_LEFT | TA_TOP);
				}
			}
		}
		DeleteObject(enemyBrush);

		// 총알 그리기
		HBRUSH bulletBrush = CreateSolidBrush(RGB(50, 50, 50));
		for (const auto& bullet : bullets) {
			if (bullet.isActive) {
				RECT bulletRect = {
					static_cast<int>(bullet.x - cameraX),
					static_cast<int>(bullet.y - cameraY),
					static_cast<int>(bullet.x - cameraX + BULLET_SIZE),
					static_cast<int>(bullet.y - cameraY + BULLET_SIZE)
				};
				if (bulletRect.left < SCREEN_WIDTH && bulletRect.right > 0 &&
					bulletRect.top < SCREEN_HEIGHT && bulletRect.bottom > 0) {
					FillRect(hdc, &bulletRect, bulletBrush);
				}
			}
		}
		DeleteObject(bulletBrush);

		// 장애물 그리기
		HBRUSH obstacleBrush = CreateSolidBrush(RGB(150, 150, 150));
		for (const auto& obstacle : obstacles) {
			RECT obstacleRect = {
				static_cast<int>(obstacle.x - cameraX),
				static_cast<int>(obstacle.y - cameraY),
				static_cast<int>(obstacle.x - cameraX + OBSTACLE_SIZE),
				static_cast<int>(obstacle.y - cameraY + OBSTACLE_SIZE)
			};
			if (obstacleRect.left < SCREEN_WIDTH && obstacleRect.right > 0 &&
				obstacleRect.top < SCREEN_HEIGHT && obstacleRect.bottom > 0) {
				FillRect(hdc, &obstacleRect, obstacleBrush);
			}
		}
		DeleteObject(obstacleBrush);

		// 아이템 그리기
		for (const auto& item : items) {
			if (item.isActive) {
				HBRUSH itemBrush;
				if (item.type == ItemType::DAMAGEUP) {
					itemBrush = CreateSolidBrush(RGB(255, 200, 0));
				}
				else if (item.type == ItemType::FIRERATE) {
					itemBrush = CreateSolidBrush(RGB(0, 200, 255));
				}
				else {
					itemBrush = CreateSolidBrush(RGB(0, 255, 0));
				}
				RECT itemRect = {
					static_cast<int>(item.x - cameraX),
					static_cast<int>(item.y - cameraY),
					static_cast<int>(item.x - cameraX + ITEM_SIZE),
					static_cast<int>(item.y - cameraY + ITEM_SIZE)
				};
				if (itemRect.left < SCREEN_WIDTH && itemRect.right > 0 &&
					itemRect.top < SCREEN_HEIGHT && itemRect.bottom > 0) {
					FillRect(hdc, &itemRect, itemBrush);
				}
				DeleteObject(itemBrush);
			}
		}

		// UI 정보 표시
		SetTextAlign(hdc, TA_LEFT | TA_TOP);
		SetBkMode(hdc, TRANSPARENT);
		SetTextColor(hdc, RGB(50, 50, 50));
		std::wstring scoreText = L"점수: " + std::to_wstring(score);
		TextOut(hdc, 10, 10, scoreText.c_str(), scoreText.length());
		std::wstring bulletInfoText = L"데미지: " + std::to_wstring(static_cast<int>(damage * 10) / 10.0f).substr(0, 3) +
			L" | 공격속도: " + std::to_wstring(static_cast<int>(bulletFireRate * 10) / 10.0f).substr(0, 3);
		TextOut(hdc, 10, 40, bulletInfoText.c_str(), bulletInfoText.length());

		// 체력바 그리기
		RECT healthBarBg = { 10, 70, 210, 90 };
		FillRect(hdc, &healthBarBg, (HBRUSH)GetStockObject(GRAY_BRUSH));
		RECT healthBar = { 10, 70, 10 + (playerHealth * 200 / MAX_PLAYER_HEALTH), 90 };
		HBRUSH healthBrush = CreateSolidBrush(RGB(0, 200, 0));
		FillRect(hdc, &healthBar, healthBrush);
		DeleteObject(healthBrush);
		std::wstring healthText = L"체력: " + std::to_wstring(playerHealth) + L"/" + std::to_wstring(MAX_PLAYER_HEALTH);
		SetTextAlign(hdc, TA_CENTER | TA_BASELINE);
		SetTextColor(hdc, RGB(255, 255, 255));
		TextOut(hdc,
			(healthBarBg.left + healthBarBg.right) / 2,
			(healthBarBg.top + healthBarBg.bottom) / 2 + 5,
			healthText.c_str(),
			healthText.length());
		SetTextAlign(hdc, TA_LEFT | TA_TOP);

		// 게임 시간과 난이도 표시
		auto currentTime = std::chrono::steady_clock::now();
		auto elapsedSeconds = std::chrono::duration_cast<std::chrono::seconds>(currentTime - gameStartTime).count();
		int minutes = elapsedSeconds / 60;
		int seconds = elapsedSeconds % 60;
		std::wstring timeText = L"시간: " + std::to_wstring(minutes) + L":" + (seconds < 10 ? L"0" : L"") + std::to_wstring(seconds);
		std::wstring difficultyText = L"난이도: " + std::to_wstring(static_cast<int>(difficultyMultiplier * 10) / 10.0f).substr(0, 3) + L"x";
		SetTextAlign(hdc, TA_CENTER | TA_TOP);
		SetTextColor(hdc, RGB(50, 50, 50));
		TextOut(hdc, SCREEN_WIDTH / 2, 10, timeText.c_str(), timeText.length());
		TextOut(hdc, SCREEN_WIDTH / 2, 40, difficultyText.c_str(), difficultyText.length());

		// 미니맵 그리기
		RECT minimapRect = { SCREEN_WIDTH - 210, 10, SCREEN_WIDTH - 10, 210 };
		FillRect(hdc, &minimapRect, (HBRUSH)GetStockObject(WHITE_BRUSH));
		FrameRect(hdc, &minimapRect, (HBRUSH)GetStockObject(BLACK_BRUSH));
		double minimapScale = 200.0f / MAP_WIDTH;

		// 플레이어의 화면 영역 그리기
		HPEN viewAreaPen = CreatePen(PS_SOLID, 1, RGB(100, 100, 100));
		HPEN oldPen = (HPEN)SelectObject(hdc, viewAreaPen);
		Rectangle(hdc,
			static_cast<int>(SCREEN_WIDTH - 210 + cameraX * minimapScale),
			static_cast<int>(10 + cameraY * minimapScale),
			static_cast<int>(SCREEN_WIDTH - 210 + (cameraX + SCREEN_WIDTH) * minimapScale),
			static_cast<int>(10 + (cameraY + SCREEN_HEIGHT) * minimapScale)
		);
		SelectObject(hdc, oldPen);
		DeleteObject(viewAreaPen);

		// 미니맵에 플레이어 그리기
		playerBrush = CreateSolidBrush(RGB(0, 100, 200));
		RECT minimapPlayer = {
			static_cast<int>(SCREEN_WIDTH - 210 + player.x * minimapScale),
			static_cast<int>(10 + player.y * minimapScale),
			static_cast<int>(SCREEN_WIDTH - 210 + (player.x + PLAYER_SIZE) * minimapScale),
			static_cast<int>(10 + (player.y + PLAYER_SIZE) * minimapScale)
		};
		FillRect(hdc, &minimapPlayer, playerBrush);
		DeleteObject(playerBrush);

		// 미니맵에 적 그리기
		enemyBrush = CreateSolidBrush(RGB(200, 50, 50));
		for (const auto& enemy : enemies) {
			if (enemy.isActive) {
				RECT minimapEnemy = {
					static_cast<int>(SCREEN_WIDTH - 210 + enemy.x * minimapScale),
					static_cast<int>(10 + enemy.y * minimapScale),
					static_cast<int>(SCREEN_WIDTH - 210 + (enemy.x + ENEMY_SIZE) * minimapScale),
					static_cast<int>(10 + (enemy.y + ENEMY_SIZE) * minimapScale)
				};
				FillRect(hdc, &minimapEnemy, enemyBrush);
			}
		}
		DeleteObject(enemyBrush);

		// 미니맵에 장애물 그리기
		obstacleBrush = CreateSolidBrush(RGB(150, 150, 150));
		for (const auto& obstacle : obstacles) {
			RECT minimapObstacle = {
				static_cast<int>(SCREEN_WIDTH - 210 + obstacle.x * minimapScale),
				static_cast<int>(10 + obstacle.y * minimapScale),
				static_cast<int>(SCREEN_WIDTH - 210 + (obstacle.x + OBSTACLE_SIZE) * minimapScale),
				static_cast<int>(10 + (obstacle.y + OBSTACLE_SIZE) * minimapScale)
			};
			FillRect(hdc, &minimapObstacle, obstacleBrush);
		}
		DeleteObject(obstacleBrush);

		// 미니맵에 아이템 그리기
		for (const auto& item : items) {
			if (item.isActive) {
				RECT minimapItem = {
					static_cast<int>(SCREEN_WIDTH - 210 + item.x * minimapScale),
					static_cast<int>(10 + item.y * minimapScale),
					static_cast<int>(SCREEN_WIDTH - 210 + (item.x + ITEM_SIZE) * minimapScale),
					static_cast<int>(10 + (item.y + ITEM_SIZE) * minimapScale)
				};
				HBRUSH itemBrush = CreateSolidBrush(item.type == ItemType::DAMAGEUP ? RGB(255, 200, 0) : (item.type == ItemType::FIRERATE ? RGB(0, 200, 255) : RGB(0, 255, 0)));
				FillRect(hdc, &minimapItem, itemBrush);
				DeleteObject(itemBrush);
			}
		}
	}
	break;

	case GameState::GAMEOVER:
	{
		SetTextAlign(hdc, TA_CENTER | TA_BASELINE);
		SetBkMode(hdc, TRANSPARENT);
		int centerX = SCREEN_WIDTH / 2;
		int centerY = SCREEN_HEIGHT / 2;
		HFONT hGameOverFont = CreateFont(48, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
			OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("맑은 고딕"));
		SelectObject(hdc, hGameOverFont);
		SetTextColor(hdc, RGB(50, 50, 50));
		TextOut(hdc, centerX, centerY - 50, L"Game Over", 9);
		DeleteObject(hGameOverFont);
		SelectObject(hdc, hFont);
		SetTextColor(hdc, RGB(50, 50, 50));
		std::wstring scoreText = L"최종 점수: " + std::to_wstring(score);
		TextOut(hdc, centerX, centerY, scoreText.c_str(), scoreText.length());
		RECT restartButton = { centerX - 100, centerY + 50, centerX + 100, centerY + 80 };
		HBRUSH buttonBrush = CreateSolidBrush(RGB(200, 200, 200));
		FillRect(hdc, &restartButton, buttonBrush);
		DeleteObject(buttonBrush);
		FrameRect(hdc, &restartButton, (HBRUSH)GetStockObject(BLACK_BRUSH));
		SetTextAlign(hdc, TA_CENTER | TA_TOP);
		SetTextColor(hdc, RGB(0, 0, 0));
		TextOut(hdc, centerX, centerY + 75, L"다시 시작", 5);
		SetTextAlign(hdc, TA_LEFT | TA_TOP);
	}
	break;
	}
	SelectObject(hdc, hOldFont);
	DeleteObject(hFont);
}


/**
 * @brief 모든 방향으로 총알 발사
 *
 * 플레이어 주변 8방향으로 총알 동시 발사
 * 각 총알의 방향과 초기 위치 설정
 */
void shootBulletInAllDirections() {
	const int bulletCount = 8;
	const float angleStep = 2 * 3.14159f / bulletCount;

	for (int i = 0; i < bulletCount; ++i) {
		float angle = i * angleStep;
		double bulletX = player.x + PLAYER_SIZE / 2 - BULLET_SIZE / 2;
		double bulletY = player.y + PLAYER_SIZE / 2 - BULLET_SIZE / 2;

		bullets.push_back({
			bulletX,
			bulletY,
			BULLET_SPEED,
			true,
			1,
			angle,
			std::chrono::steady_clock::now(),
			false
			});
	}
}

/**
 * @brief 게임 상태 업데이트
 *
 * 매 프레임마다 호출
 * 1. 난이도 업데이트
 * 2. 적 및 아이템 생성
 * 3. 게임 오브젝트(적, 총알, 아이템) 위치 업데이트
 * 4. 충돌 감지 및 처리
 * 5. 카메라 위치 업데이트
 */
void updateGame() {
	auto currentTime = std::chrono::steady_clock::now();
	auto elapsedSeconds = std::chrono::duration_cast<std::chrono::seconds>(currentTime - gameStartTime).count();

	updateDifficulty();

	// 적 생성
	if (!enemySpawnMax) {
		if (rand() % 100 < (5 + static_cast<int>(difficultyMultiplier * 0.5))) {
			spawnEnemy();
		}
	} else
	{
		spawnEnemy();
	}


	// 아이템 생성
	if (rand() % 1000 < max(1, static_cast<int>(10 - difficultyMultiplier * 0.5))) {
		spawnItem();
	}

	// 적 업데이트 및 충돌 처리
	for (auto& enemy : enemies) {
		if (enemy.isActive) {
			updateEnemyPosition(enemy);
			handleEnemyCollisions(enemy);
		}
	}

	// 총알 업데이트 및 충돌 처리
	updateBullets();

	// 아이템 충돌 체크
	for (auto& item : items) {
		if (item.isActive && checkCollision(player.x, player.y, PLAYER_SIZE, PLAYER_SIZE, item.x, item.y, ITEM_SIZE, ITEM_SIZE)) {
			applyItemEffect(item.type);
			item.isActive = false;
		}
	}

	// 플레이어와 적 충돌 체크
	checkPlayerEnemyCollision();

	updateCamera();
}

/**
 * @brief 적 생성
 *
 * 맵의 랜덤한 위치에 적 생성
 * 생성 위치 조건:
 * 1. 플레이어의 현재 화면 밖
 * 2. 다른 적이나 장애물과 겹치지 않음
 *
 * 적의 속도와 체력은 현재 게임 시간과 난이도에 따라 조정
 */
void spawnEnemy() {
	double x, y;
	bool validPosition;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> disX(0, MAP_WIDTH - ENEMY_SIZE);
	std::uniform_int_distribution<> disY(0, MAP_HEIGHT - ENEMY_SIZE);
	std::uniform_int_distribution<> disSize(50, 150);
	do {
		validPosition = true;
		x = disX(gen);
		y = disY(gen);

		// 플레이어의 화면 영역 체크
		if (x >= cameraX && x <= cameraX + SCREEN_WIDTH &&
			y >= cameraY && y <= cameraY + SCREEN_HEIGHT) {
			validPosition = false;
			continue;
		}

		// 다른 적과의 충돌 체크
		for (const auto& otherEnemy : enemies) {
			if (otherEnemy.isActive && checkCollision(x, y, ENEMY_SIZE, ENEMY_SIZE, otherEnemy.x, otherEnemy.y, ENEMY_SIZE, ENEMY_SIZE)) {
				validPosition = false;
				break;
			}
		}

		// 장애물과의 충돌 체크
		if (validPosition) {
			for (const auto& obstacle : obstacles) {
				if (checkCollision(x, y, ENEMY_SIZE, ENEMY_SIZE, obstacle.x, obstacle.y, OBSTACLE_SIZE, OBSTACLE_SIZE)) {
					validPosition = false;
					break;
				}
			}
		}
	} while (!validPosition);

	auto currentTime = std::chrono::steady_clock::now();
	auto elapsedSeconds = std::chrono::duration_cast<std::chrono::seconds>(currentTime - gameStartTime).count();
	double enemySpeed = 1.5 * difficultyMultiplier * (1 + elapsedSeconds / 250.0);
	// 적의 체력 증가 속도를 높입니다
	enemyHealth = max(1, static_cast<int>((elapsedSeconds / 15) * (difficultyMultiplier * 0.8)) + 1);
	
	enemies.push_back({ x, y, enemySpeed, true, enemyHealth, 0, std::chrono::steady_clock::now(), false });
}

/**
 * @brief 카메라 위치 업데이트
 *
 * 플레이어의 현재 위치 기반으로 카메라 위치 조정
 * 카메라는 플레이어를 중심으로 이동하며, 맵의 경계를 벗어나지 않도록 함
 */
void updateCamera() {
	cameraX = player.x + PLAYER_SIZE / 2 - SCREEN_WIDTH / 2.0f;
	cameraY = player.y + PLAYER_SIZE / 2 - SCREEN_HEIGHT / 2.0f;

	cameraX = max(0.0f, min(cameraX, MAP_WIDTH - SCREEN_WIDTH));
	cameraY = max(0.0f, min(cameraY, MAP_HEIGHT - SCREEN_HEIGHT));
}

/**
 * @brief 장애물 초기화
 *
 * 맵 내에 무작위로 장애물 배치
 * 장애물은 플레이어의 시작 위치와 겹치지 않도록 배치
 */
void initializeObstacles() {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> disX(0, MAP_WIDTH - OBSTACLE_SIZE);
	std::uniform_int_distribution<> disY(0, MAP_HEIGHT - OBSTACLE_SIZE);
	std::uniform_int_distribution<> disSize(50, 150);

	for (int i = 0; i < OBSTACLE_COUNT; ++i) {
		int width = disSize(gen);
		int height = disSize(gen);
		double x = disX(gen);
		double y = disY(gen);

		if (x < player.x + 100 && x + width > player.x - 100 &&
			y < player.y + 100 && y + height > player.y - 100) {
			continue;
		}

		obstacles.push_back({ x, y, 0, true, 0, 0, std::chrono::steady_clock::now(), false });
	}
}

/**
 * @brief 아이템 생성
 *
 * 맵 내 무작위 위치에 아이템 생성
 * 아이템은 장애물과 겹치지 않으며, 플레이어가 접근 가능한 위치에 생성
 */
void spawnItem() {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> disX(0, MAP_WIDTH - ITEM_SIZE);
	std::uniform_int_distribution<> disY(0, MAP_HEIGHT - ITEM_SIZE);
	std::uniform_int_distribution<> disType(0, 2);

	double x, y;
	bool validPosition;
	do {
		validPosition = true;
		x = disX(gen);
		y = disY(gen);

		// 장애물과 겹치지 않는지 확인
		for (const auto& obstacle : obstacles) {
			if (checkCollision(x, y, ITEM_SIZE, ITEM_SIZE, obstacle.x, obstacle.y, OBSTACLE_SIZE, OBSTACLE_SIZE)) {
				validPosition = false;
				break;
			}
		}

		// 플레이어가 접근 가능한 위치인지 확인
		if (validPosition) {
			bool accessibleFromAnyDirection = false;
			for (int dx = -1; dx <= 1; dx++) {
				for (int dy = -1; dy <= 1; dy++) {
					if (dx == 0 && dy == 0) continue;
					double checkX = x + dx * 50;
					double checkY = y + dy * 50;
					bool blocked = false;
					for (const auto& obstacle : obstacles) {
						if (checkCollision(checkX, checkY, PLAYER_SIZE, PLAYER_SIZE, obstacle.x, obstacle.y, OBSTACLE_SIZE, OBSTACLE_SIZE)) {
							blocked = true;
							break;
						}
					}
					if (!blocked) {
						accessibleFromAnyDirection = true;
						break;
					}
				}
				if (accessibleFromAnyDirection) break;
			}
			validPosition = accessibleFromAnyDirection;
		}
	} while (!validPosition);

	ItemType type;
	do {
		type = static_cast<ItemType>(disType(gen));
	} while (type == ItemType::FIRERATE && bulletFireRate >= MAX_FIRE_RATE);

	items.push_back({ x, y, true, type });
}

/**
 * @brief 두 객체 간의 충돌 검사
 *
 * @param x1 첫 번째 객체의 x 좌표
 * @param y1 첫 번째 객체의 y 좌표
 * @param w1 첫 번째 객체의 너비
 * @param h1 첫 번째 객체의 높이
 * @param x2 두 번째 객체의 x 좌표
 * @param y2 두 번째 객체의 y 좌표
 * @param w2 두 번째 객체의 너비
 * @param h2 두 번째 객체의 높이
 * @return 충돌 여부 (true: 충돌, false: 비충돌)
 */
bool checkCollision(double x1, double y1, double w1, double h1, double x2, double y2, double w2, double h2) {
	return (x1 < x2 + w2 && x1 + w1 > x2 && y1 < y2 + h2 && y1 + h1 > y2);
}

/**
 * @brief 아이템 효과 적용 함수
 */
void applyItemEffect(ItemType type) {
	switch (type) {
	case ItemType::DAMAGEUP:
		damage += 0.3;
		break;
	case ItemType::FIRERATE:
		bulletFireRate = min(bulletFireRate + 0.2f, MAX_FIRE_RATE);
		SetTimer(mainhWnd, 2, static_cast<int>(1000.0f / bulletFireRate), NULL);
		break;
	case ItemType::HEAL:
		playerHealth = min(playerHealth + 1, MAX_PLAYER_HEALTH);
		break;
	}
}

/**
 * @brief 게임 시작
 *
 * 플레이어 초기화, 장애물 생성, 게임 시작 시간 설정 등 수행
 */
void startGame() {
	player.x = MAP_WIDTH / 2 - PLAYER_SIZE / 2;
	player.y = MAP_HEIGHT - PLAYER_SIZE - 50;
	player.speed = INITIAL_PLAYER_SPEED;
	player.isActive = true;
	player.health = INITIAL_PLAYER_HEALTH;
	player.direction = 0;
	player.invincibleUntil = std::chrono::steady_clock::now();
	player.isInvincible = false;

	initializeObstacles();
	gameStartTime = std::chrono::steady_clock::now();
	difficultyMultiplier = 1.0f;
}

/**
 * @brief 게임 리셋
 *
 * 모든 게임 상태 초기화 및 새 게임 시작 준비
 */
void ResetGame() {
	score = 0;
	damage = INITIAL_DAMAGE;
	bulletFireRate = INITIAL_FIRE_RATE;
	playerHealth = INITIAL_PLAYER_HEALTH;

	enemies.clear();
	bullets.clear();
	items.clear();
	obstacles.clear();

	player.x = MAP_WIDTH / 2 - PLAYER_SIZE / 2;
	player.y = MAP_HEIGHT - PLAYER_SIZE - 50;
	player.speed = INITIAL_PLAYER_SPEED;
	player.isActive = true;
	player.health = INITIAL_PLAYER_HEALTH;
	player.direction = 0;
	player.invincibleUntil = std::chrono::steady_clock::now();
	player.isInvincible = false;

	updateCamera();

	// 총알 발사 타이머 재설정
	SetTimer(mainhWnd, 2, 1000 / static_cast<int>(bulletFireRate), NULL);
}

/**
 * @brief 플레이어 무적 상태로 만들기
 *
 * 플레이어가 데미지를 입은 후 일시적으로 무적 상태가 됨
 * 무적 시간은 현재 난이도에 따라 조정
 */
void makePlayerInvincible() {
	player.isInvincible = true;
	int invincibleTime = max(1, static_cast<int>(3 - difficultyMultiplier * 0.3));
	player.invincibleUntil = std::chrono::steady_clock::now() + std::chrono::seconds(invincibleTime);
}

/**
 * @brief 플레이어 무적 상태 확인 함수
 * @return 플레이어의 현재 무적 상태
 */
bool isPlayerInvincible() {
	if (player.isInvincible) {
		if (std::chrono::steady_clock::now() > player.invincibleUntil) {
			player.isInvincible = false;
			return false;
		}
		return true;
	}
	return false;
}

/**
 * @brief 게임 난이도 업데이트
 *
 * 게임 시간에 따라 난이도 점진적 증가
 * 난이도 증가에 따라 적의 속도와 체력 조정
 */
void updateDifficulty() {
	auto currentTime = std::chrono::steady_clock::now();
	auto elapsedSeconds = std::chrono::duration_cast<std::chrono::seconds>(currentTime - gameStartTime).count();

	// 난이도 증가 속도 증가
	difficultyMultiplier = 1.0f + (elapsedSeconds / 60.0f) * 0.15f;

	for (auto& enemy : enemies) {
		// 적의 속도 증가를 더 빠르게
		enemy.speed = 1.5 * difficultyMultiplier * (1 + elapsedSeconds / 200.0);
	}
}

/**
 * @brief 장애물과의 충돌 검사
 *
 * @param x 검사할 객체의 x 좌표
 * @param y 검사할 객체의 y 좌표
 * @param width 검사할 객체의 너비
 * @param height 검사할 객체의 높이
 * @return 충돌 여부 (true: 충돌, false: 비충돌)
 */
bool checkCollisionWithObstacles(double x, double y, double width, double height) {
	for (const auto& obstacle : obstacles) {
		if (checkCollision(x, y, width, height, obstacle.x, obstacle.y, OBSTACLE_SIZE, OBSTACLE_SIZE)) {
			return true;
		}
	}
	return false;
}

/**
 * @brief 적의 위치를 업데이트하는 함수
 *
 * @param enemy 업데이트할 적 객체
 */
void updateEnemyPosition(GameObject& enemy) {
	double dx = player.x - enemy.x;
	double dy = player.y - enemy.y;
	double distance = std::sqrt(dx * dx + dy * dy);

	if (distance > 0) {
		double moveX = (dx / distance) * enemy.speed;
		double moveY = (dy / distance) * enemy.speed;

		double newX = enemy.x + moveX;
		double newY = enemy.y + moveY;

		if (!checkCollisionWithObstacles(newX, newY, ENEMY_SIZE, ENEMY_SIZE)) {
			enemy.x = newX;
			enemy.y = newY;
		}
		else {
			// 장애물과 충돌 시 우회 로직
			double angleStep = 3.14159 / 8;  // 45도
			for (int i = 1; i <= 4; ++i) {  // 왼쪽과 오른쪽으로 45도씩 최대 180도까지 시도
				double leftAngle = atan2(dy, dx) - angleStep * i;
				double rightAngle = atan2(dy, dx) + angleStep * i;

				newX = enemy.x + cos(leftAngle) * enemy.speed;
				newY = enemy.y + sin(leftAngle) * enemy.speed;
				if (!checkCollisionWithObstacles(newX, newY, ENEMY_SIZE, ENEMY_SIZE)) {
					enemy.x = newX;
					enemy.y = newY;
					break;
				}

				newX = enemy.x + cos(rightAngle) * enemy.speed;
				newY = enemy.y + sin(rightAngle) * enemy.speed;
				if (!checkCollisionWithObstacles(newX, newY, ENEMY_SIZE, ENEMY_SIZE)) {
					enemy.x = newX;
					enemy.y = newY;
					break;
				}
			}
		}
	}

	if (enemy.y > MAP_HEIGHT) {
		enemy.isActive = false;  // 맵 밖으로 나간 적은 비활성화
	}
}

/**
 * @brief 적들 간의 충돌을 처리하는 함수
 *
 * @param enemy 충돌을 확인할 적 객체
 */
void handleEnemyCollisions(GameObject& enemy) {
	for (auto& otherEnemy : enemies) {
		if (&enemy != &otherEnemy && otherEnemy.isActive) {
			if (checkCollision(enemy.x, enemy.y, ENEMY_SIZE, ENEMY_SIZE, otherEnemy.x, otherEnemy.y, ENEMY_SIZE, ENEMY_SIZE)) {
				double pushX = enemy.x - otherEnemy.x;
				double pushY = enemy.y - otherEnemy.y;
				double pushDistance = std::sqrt(pushX * pushX + pushY * pushY);
				if (pushDistance > 0) {
					enemy.x += (pushX / pushDistance) * 1.0f;
					enemy.y += (pushY / pushDistance) * 1.0f;
				}
			}
		}
	}
}

/**
 * @brief 총알을 업데이트하고 충돌을 처리하는 함수
 */
void updateBullets() {
	for (auto& bullet : bullets) {
		if (bullet.isActive) {
			bullet.x += std::cos(bullet.direction) * bullet.speed;
			bullet.y += std::sin(bullet.direction) * bullet.speed;

			// 총알 사거리 제한
			double distanceTraveled = std::sqrt(std::pow(bullet.x - player.x, 2) + std::pow(bullet.y - player.y, 2));
			if (distanceTraveled > BULLET_MAX_DISTANCE) {
				bullet.isActive = false;
				continue;
			}

			// 맵 경계 체크
			if (bullet.x < 0 || bullet.x > MAP_WIDTH || bullet.y < 0 || bullet.y > MAP_HEIGHT) {
				bullet.isActive = false;
				continue;
			}

			// 장애물 충돌 체크
			for (const auto& obstacle : obstacles) {
				if (checkCollision(bullet.x, bullet.y, BULLET_SIZE, BULLET_SIZE, obstacle.x, obstacle.y, OBSTACLE_SIZE, OBSTACLE_SIZE)) {
					bullet.isActive = false;
					break;
				}
			}

			// 적과의 충돌 체크
			for (auto& enemy : enemies) {
				if (enemy.isActive && checkCollision(bullet.x, bullet.y, BULLET_SIZE, BULLET_SIZE, enemy.x, enemy.y, ENEMY_SIZE, ENEMY_SIZE)) {
					int currentDamage = max(1, static_cast<int>(damage / (difficultyMultiplier * 0.7)));
					enemy.health -= currentDamage;
					if (enemy.health <= 0) {
						enemy.isActive = false;
						score += static_cast<int>(3 * difficultyMultiplier);
					}
					bullet.isActive = false;
					break;
				}
			}
		}
	}
}

/**
 * @brief 플레이어와 적의 충돌을 체크하는 함수
 */
void checkPlayerEnemyCollision() {
	for (const auto& enemy : enemies) {
		if (enemy.isActive && checkCollision(player.x, player.y, PLAYER_SIZE, PLAYER_SIZE, enemy.x, enemy.y, ENEMY_SIZE, ENEMY_SIZE)) {
			if (!isPlayerInvincible()) {
				playerHealth--;
				if (playerHealth <= 0) {
					gameState = GameState::GAMEOVER;
				}
				else {
					makePlayerInvincible();
				}
			}
			break;
		}
	}
}
