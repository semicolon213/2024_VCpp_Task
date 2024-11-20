/**
 * @brief               2024년 2학기 VC++ 기말 대체 과제를 위한 프로젝트
 * @details            뱀파이어 서바이벌에서 착안한 게임 프로젝트
 * @author            202407034 김형균(Git : semicolon213)
 * @date                2024-11-06
 */



 // 202307034_KHG.cpp : 애플리케이션에 대한 진입점을 정의합니다.
 //

#include "202307034_KHG.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
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

	// TODO: 여기에 코드를 입력합니다.

	// 전역 문자열을 초기화합니다.
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_MY202307034KHG, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 애플리케이션 초기화를 수행합니다:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY202307034KHG));

	MSG msg;

	// 기본 메시지 루프입니다:
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



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
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

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

	HWND hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPED | WS_SYSMENU,
		CW_USEDEFAULT, 0, SCREEN_WIDTH, SCREEN_HEIGHT, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//

int left = 0, top = 0, right = 50, bottom = 50;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_CREATE:
	{

		// 게임 루프를 위한 타이머 설정
		SetTimer(hWnd, 1, 16, NULL);  // 약 60 FPS
		ResetGame();
	}
	break;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		// 배경 지우기
		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

		switch (gameState) {
		case GameState::STARTSCREEN:
		{
			// 시작 화면 그리기
			SetTextAlign(hdc, TA_CENTER | TA_BASELINE);
			SetBkMode(hdc, TRANSPARENT);

			RECT clientRect;
			GetClientRect(hWnd, &clientRect);
			int centerX = (clientRect.right - clientRect.left) / 2;
			int centerY = (clientRect.bottom - clientRect.top) / 2;

			// 게임 제목
			SelectObject(hdc, CreateFont(48, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
				CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Arial")));
			TextOut(hdc, centerX, centerY - 50, L"유한 특공대", 6);

			// 시작 버튼
			RECT startButton = { centerX - 50, centerY + 50, centerX + 50, centerY + 80 };
			FillRect(hdc, &startButton, (HBRUSH)(COLOR_BTNFACE + 1));
			FrameRect(hdc, &startButton, (HBRUSH)GetStockObject(BLACK_BRUSH));

			SelectObject(hdc, CreateFont(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
				CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Arial")));
			TextOut(hdc, centerX, centerY + 70, L"Start", 5);
		}
		break;

		case GameState::COUNTDOWN:
		{
			// 카운트다운 표시
			SetTextAlign(hdc, TA_CENTER | TA_BASELINE);
			SetBkMode(hdc, TRANSPARENT);

			RECT clientRect;
			GetClientRect(hWnd, &clientRect);
			int centerX = (clientRect.right - clientRect.left) / 2;
			int centerY = (clientRect.bottom - clientRect.top) / 2;

			SelectObject(hdc, CreateFont(72, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
				CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Arial")));

			auto now = std::chrono::steady_clock::now();
			auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - countdownStartTime).count();
			int remainingSeconds = startCntSec - static_cast<int>(elapsed);

			if (remainingSeconds > 0) {
				std::wstring countdownText = std::to_wstring(remainingSeconds);
				TextOut(hdc, centerX, centerY, countdownText.c_str(), countdownText.length());
			}
			else {
				gameState = GameState::PLAY;
				startGame();
			}
		}
		break;

		case GameState::PLAY:
		{
			// 플레이어 그리기
			HBRUSH playerBrush = CreateSolidBrush(RGB(0, 0, 255));
			RECT playerRect = {
				static_cast<int>(player.x - cameraX),
				static_cast<int>(player.y - cameraY),
				static_cast<int>(player.x - cameraX + player.objectSize.cx),
				static_cast<int>(player.y - cameraY + player.objectSize.cy)
			};
			FillRect(hdc, &playerRect, playerBrush);
			DeleteObject(playerBrush);

			// 적 그리기
			HBRUSH enemyBrush = CreateSolidBrush(RGB(255, 0, 0));
			for (const auto& enemy : enemies) {
				if (enemy.isActive) {
					RECT enemyRect = {
						static_cast<int>(enemy.x - cameraX),
						static_cast<int>(enemy.y - cameraY),
						static_cast<int>(enemy.x - cameraX + enemy.objectSize.cx),
						static_cast<int>(enemy.y - cameraY + enemy.objectSize.cy)
					};
					if (enemyRect.left < SCREEN_WIDTH && enemyRect.right > 0 &&
						enemyRect.top < SCREEN_HEIGHT && enemyRect.bottom > 0) {
						FillRect(hdc, &enemyRect, enemyBrush);

						// 적의 체력 표시
						std::wstring healthText = std::to_wstring(enemy.health);
						SetBkMode(hdc, TRANSPARENT);
						SetTextColor(hdc, RGB(255, 255, 255));
						DrawText(hdc, healthText.c_str(), -1, &enemyRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
					}
				}
			}
			DeleteObject(enemyBrush);

			// 총알 그리기
			HBRUSH bulletBrush = CreateSolidBrush(RGB(0, 255, 0));
			for (const auto& bullet : bullets) {
				if (bullet.isActive) {
					RECT bulletRect = {
						static_cast<int>(bullet.x - cameraX),
						static_cast<int>(bullet.y - cameraY),
						static_cast<int>(bullet.x - cameraX + bullet.objectSize.cx),
						static_cast<int>(bullet.y - cameraY + bullet.objectSize.cy)
					};
					if (bulletRect.left < SCREEN_WIDTH && bulletRect.right > 0 &&
						bulletRect.top < SCREEN_HEIGHT && bulletRect.bottom > 0) {
						FillRect(hdc, &bulletRect, bulletBrush);
					}
				}
			}
			DeleteObject(bulletBrush);

			// 장애물 그리기
			HBRUSH obstacleBrush = CreateSolidBrush(RGB(128, 128, 128));
			for (const auto& obstacle : obstacles) {
				RECT obstacleRect = {
					static_cast<int>(obstacle.x - cameraX),
					static_cast<int>(obstacle.y - cameraY),
					static_cast<int>(obstacle.x - cameraX + obstacle.objectSize.cx),
					static_cast<int>(obstacle.y - cameraY + obstacle.objectSize.cy)
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
						itemBrush = CreateSolidBrush(RGB(255, 255, 0));  // 노란색
					}
					else if (item.type == ItemType::MULTISHOT) {
						itemBrush = CreateSolidBrush(RGB(255, 0, 255));  // 보라색
					}
					else {
						itemBrush = CreateSolidBrush(RGB(0, 255, 0));  // 초록색 (체력 회복)
					}
					RECT itemRect = {
						static_cast<int>(item.x - cameraX),
						static_cast<int>(item.y - cameraY),
						static_cast<int>(item.x - cameraX + item.itemSize.cx),
						static_cast<int>(item.y - cameraY + item.itemSize.cy)
					};
					if (itemRect.left < SCREEN_WIDTH && itemRect.right > 0 &&
						itemRect.top < SCREEN_HEIGHT && itemRect.bottom > 0) {
						FillRect(hdc, &itemRect, itemBrush);
					}
					DeleteObject(itemBrush);
				}
			}

			// 점수 표시
			std::wstring scoreText = L"Score: " + std::to_wstring(score);
			TextOut(hdc, 10, 10, scoreText.c_str(), scoreText.length());

			// 총알 데미지와 발사 개수 표시
			std::wstring bulletInfoText = L"Damage: " + std::to_wstring(damage) + L" | Count: " + std::to_wstring(bulletCnt);
			TextOut(hdc, 10, 30, bulletInfoText.c_str(), bulletInfoText.length());

			// 플레이어 체력 표시
			std::wstring healthText = L"Health: " + std::to_wstring(playerHealth);
			TextOut(hdc, SCREEN_WIDTH - 100, 10, healthText.c_str(), healthText.length());

			// 미니맵 그리기
			RECT minimapRect = { SCREEN_WIDTH - 210, 10, SCREEN_WIDTH - 10, 210 };
			FillRect(hdc, &minimapRect, (HBRUSH)GetStockObject(WHITE_BRUSH));
			FrameRect(hdc, &minimapRect, (HBRUSH)GetStockObject(BLACK_BRUSH));

			double minimapScale = 200.0f / MAP_WIDTH;

			// 미니맵에 플레이어 표시
			RECT minimapPlayer = {
				static_cast<int>(SCREEN_WIDTH - 210 + player.x * minimapScale),
				static_cast<int>(10 + player.y * minimapScale),
				static_cast<int>(SCREEN_WIDTH - 210 + (player.x + player.objectSize.cx) * minimapScale),
				static_cast<int>(10 + (player.y + player.objectSize.cy) * minimapScale)
			};
			FillRect(hdc, &minimapPlayer, playerBrush);

			// 미니맵에 적 표시
			for (const auto& enemy : enemies) {
				if (enemy.isActive) {
					RECT minimapEnemy = {
						static_cast<int>(SCREEN_WIDTH - 210 + enemy.x * minimapScale),
						static_cast<int>(10 + enemy.y * minimapScale),
						static_cast<int>(SCREEN_WIDTH - 210 + (enemy.x + enemy.objectSize.cx) * minimapScale),
						static_cast<int>(10 + (enemy.y + enemy.objectSize.cy) * minimapScale)
					};
					FillRect(hdc, &minimapEnemy, enemyBrush);
				}
			}

			// 미니맵에 장애물 표시
			for (const auto& obstacle : obstacles) {
				RECT minimapObstacle = {
					static_cast<int>(SCREEN_WIDTH - 210 + obstacle.x * minimapScale),
					static_cast<int>(10 + obstacle.y * minimapScale),
					static_cast<int>(SCREEN_WIDTH - 210 + (obstacle.x + obstacle.objectSize.cx) * minimapScale),
					static_cast<int>(10 + (obstacle.y + obstacle.objectSize.cy) * minimapScale)
				};
				FillRect(hdc, &minimapObstacle, obstacleBrush);
			}

			// 미니맵에 아이템 표시
			for (const auto& item : items) {
				if (item.isActive) {
					RECT minimapItem = {
						static_cast<int>(SCREEN_WIDTH - 210 + item.x * minimapScale),
						static_cast<int>(10 + item.y * minimapScale),
						static_cast<int>(SCREEN_WIDTH - 210 + (item.x + item.itemSize.cx) * minimapScale),
						static_cast<int>(10 + (item.y + item.itemSize.cy) * minimapScale)
					};
					HBRUSH itemBrush = CreateSolidBrush(item.type == ItemType::DAMAGEUP ? RGB(255, 255, 0) : (item.type == ItemType::MULTISHOT ? RGB(255, 0, 255) : RGB(0, 255, 0)));
					FillRect(hdc, &minimapItem, itemBrush);
					DeleteObject(itemBrush);
				}
			}
		}
		break;

		case GameState::GAMEOVER:
		{
			// 게임 오버 메시지
			SetTextAlign(hdc, TA_CENTER | TA_BASELINE);
			SetBkMode(hdc, TRANSPARENT);

			RECT clientRect;
			GetClientRect(hWnd, &clientRect);
			int centerX = (clientRect.right - clientRect.left) / 2;
			int centerY = (clientRect.bottom - clientRect.top) / 2;

			SelectObject(hdc, CreateFont(48, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
				CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Arial")));
			TextOut(hdc, centerX, centerY - 50, L"Game Over", 9);

			SelectObject(hdc, CreateFont(24, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
				CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Arial")));
			std::wstring scoreText = L"Final Score: " + std::to_wstring(score);
			TextOut(hdc, centerX, centerY, scoreText.c_str(), scoreText.length());

			// 재시작 버튼
			RECT restartButton = { centerX - 50, centerY + 50, centerX + 50, centerY + 80 };
			FillRect(hdc, &restartButton, (HBRUSH)(COLOR_BTNFACE + 1));
			FrameRect(hdc, &restartButton, (HBRUSH)GetStockObject(BLACK_BRUSH));

			SelectObject(hdc, CreateFont(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
				CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Arial")));
			TextOut(hdc, centerX, centerY + 70, L"Restart", 7);
		}
		break;
		}

		EndPaint(hWnd, &ps);
	}
	return 0;

	case WM_LBUTTONDOWN:
	{
		int mouseX = LOWORD(lParam);
		int mouseY = HIWORD(lParam);

		if (gameState == GameState::STARTSCREEN) {
			// 시작 버튼 클릭 확인
			RECT clientRect;
			GetClientRect(hWnd, &clientRect);
			int centerX = (clientRect.right - clientRect.left) / 2;
			int centerY = (clientRect.bottom - clientRect.top) / 2;
			RECT startButton = { centerX - 50, centerY + 50, centerX + 50, centerY + 80 };

			if (mouseX >= startButton.left && mouseX <= startButton.right &&
				mouseY >= startButton.top && mouseY <= startButton.bottom) {
				startGame();
				gameState = GameState::COUNTDOWN;
				countdownStartTime = std::chrono::steady_clock::now();
				InvalidateRect(hWnd, NULL, TRUE);
			}
		}
		else if (gameState == GameState::GAMEOVER) {
			// 재시작 버튼 클릭 확인
			RECT clientRect;
			GetClientRect(hWnd, &clientRect);
			int centerX = (clientRect.right - clientRect.left) / 2;
			int centerY = (clientRect.bottom - clientRect.top) / 2;
			RECT restartButton = { centerX - 50, centerY + 50, centerX + 50, centerY + 80 };

			if (mouseX >= restartButton.left && mouseX <= restartButton.right &&
				mouseY >= restartButton.top && mouseY <= restartButton.bottom) {
				ResetGame();
				gameState = GameState::COUNTDOWN;
				countdownStartTime = std::chrono::steady_clock::now();
				InvalidateRect(hWnd, NULL, TRUE);
			}
		}
	}
	break;
	/*case WM_KEYDOWN:
		if (gameState == GameState::PLAY) {
			double newX = player.x;
			double newY = player.y;
			switch (wParam) {
			case VK_LEFT:
				newX = max((0.0f), (player.x - player.speed));
				break;
			case VK_RIGHT:
				newX = min((static_cast<double>(MAP_WIDTH - player.objectSize.cx)), (player.x + player.speed));
				break;
			case VK_UP:
				newY = max(0.0f, (player.y - player.speed));
				break;
			case VK_DOWN:
				newY = min((static_cast<double>(MAP_HEIGHT - player.objectSize.cy)), (player.y + player.speed));
				break;
			case VK_SPACE:
				shootBullet();
				break;
			}

			// 장애물과의 충돌 검사
			bool canMove = true;
			for (const auto& obstacle : obstacles) {
				if (checkCollision(newX, newY, player.objectSize.cx, player.objectSize.cy, obstacle.x, obstacle.y, obstacle.objectSize.cx, obstacle.objectSize.cy)) {
					canMove = false;
					break;
				}
			}

			if (canMove) {
				player.x = newX;
				player.y = newY;
			}

			updateCamera();
		}
		break;*/

	case WM_ACTIVATEAPP:                   ///윈도우 활성화 시 키 폴링, 비활성화 시 폴링 타이머 제거
	{
		if (wParam)
			SetTimer(hWnd, 0, 50, NULL);
		else
			KillTimer(hWnd, 0);
	}
	break;
	case WM_TIMER:
		if (wParam == 1) {
			if (gameState == GameState::PLAY) {
				updateGame();
			}
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		}
		if (wParam == 0)
		{
			if (gameState == GameState::PLAY) 
			{
				double newX = player.x;
				double newY = player.y;
				///0x8000 : GetKeyState의 반환 값의 최상위 비트 - 설정되어있을 시 해당 키가 눌려져 있음
				if (GetKeyState(VK_LEFT) & 0x8000)
				{
					newX = max((0.0f), (player.x - player.speed));
				}
				if (GetKeyState(VK_RIGHT) & 0x8000)
				{
					newX = min((static_cast<double>(MAP_WIDTH - player.objectSize.cx)), (player.x + player.speed));
				}
				if (GetKeyState(VK_UP) & 0x8000)
				{
					newY = max(0.0f, (player.y - player.speed));
				}
				if (GetKeyState(VK_DOWN) & 0x8000)
				{
					newY = min((static_cast<double>(MAP_HEIGHT - player.objectSize.cy)), (player.y + player.speed));
				}
				if (GetKeyState(VK_SPACE) & 0x8000)
				{
					shootBullet();
				}
				if (GetKeyState(VK_ESCAPE) & 0x8000)
				{

				}

				// 장애물과의 충돌 검사
				bool canMove = true;
				for (const auto& obstacle : obstacles) {
					if (checkCollision(newX, newY, player.objectSize.cx, player.objectSize.cy, obstacle.x, obstacle.y, obstacle.objectSize.cx, obstacle.objectSize.cy)) {
						canMove = false;
						break;
					}
				}
				if (canMove) {
					player.x = newX;
					player.y = newY;
					updateCamera();
				}
				break;
			}
		}
		break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}


void updateGame() {
	// 게임 시간 계산
	auto currentTime = std::chrono::steady_clock::now();
	auto elapsedSeconds = std::chrono::duration_cast<std::chrono::seconds>(currentTime - gameStartTime).count();

	// 적 생성
	if (rand() % 100 < 5) {  // 5% 확률로 적 생성
		spawnEnemy();
	}

	// 아이템 생성
	if (rand() % 1000 < 5) {  // 0.5% 확률로 아이템 생성
		spawnItem();
	}

	// 적 이동 및 체력 증가
	for (auto& enemy : enemies) {
		if (enemy.isActive) {
			// 플레이어 방향으로 이동
			double dx = player.x - enemy.x;
			double dy = player.y - enemy.y;
			double distance = std::sqrt(dx * dx + dy * dy);
			if (distance > 0) {
				enemy.x += (dx / distance) * enemy.speed;
				enemy.y += (dy / distance) * enemy.speed;
			}

			// 다른 적들과의 충돌 검사 및 밀어내기
			for (auto& otherEnemy : enemies) {
				if (&enemy != &otherEnemy && otherEnemy.isActive) {
					if (checkCollision(enemy.x, enemy.y, enemy.objectSize.cx, enemy.objectSize.cy,
						otherEnemy.x, otherEnemy.y, otherEnemy.objectSize.cx, otherEnemy.objectSize.cy)) {
						// 충돌 시 서로 반대 방향으로 밀어냄
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

			if (enemy.y > MAP_HEIGHT) {
				enemy.isActive = false;
				gameState = GameState::GAMEOVER;
			}

			// 시간에 따른 체력 증가 (매 30초마다 1씩 증가)
			enemy.health = max(1, static_cast<int>(elapsedSeconds / 30) + 1);

			// 장애물과의 충돌 검사
			for (const auto& obstacle : obstacles) {
				if (checkCollision(enemy.x, enemy.y, enemy.objectSize.cx, enemy.objectSize.cy,
					obstacle.x, obstacle.y, obstacle.objectSize.cx, obstacle.objectSize.cy)) {
					// 충돌 시 장애물 반대 방향으로 밀어냄
					double pushX = enemy.x - (obstacle.x + obstacle.objectSize.cx / 2);
					double pushY = enemy.y - (obstacle.y + obstacle.objectSize.cy / 2);
					double pushDistance = std::sqrt(pushX * pushX + pushY * pushY);
					if (pushDistance > 0) {
						enemy.x += (pushX / pushDistance) * 1.0f;
						enemy.y += (pushY / pushDistance) * 1.0f;
					}
				}
			}
		}
	}

	// 총알 이동
	for (auto& bullet : bullets) {
		if (bullet.isActive) {
			bullet.y -= bullet.speed;
			if (bullet.y < 0) {
				bullet.isActive = false;
			}

			// 장애물과의 충돌 검사
			for (const auto& obstacle : obstacles) {
				if (checkCollision(bullet.x, bullet.y, bullet.objectSize.cx, bullet.objectSize.cy, obstacle.x, obstacle.y, obstacle.objectSize.cx, obstacle.objectSize.cy)) {
					bullet.isActive = false;
					break;
				}
			}
		}
	}

	// 충돌 감지 (총알과 적)
	for (auto& bullet : bullets) {
		if (bullet.isActive) {
			for (auto& enemy : enemies) {
				if (enemy.isActive && checkCollision(bullet.x, bullet.y, bullet.objectSize.cx, bullet.objectSize.cy, enemy.x, enemy.y, enemy.objectSize.cx, enemy.objectSize.cy)) {
					enemy.health -= damage;
					if (enemy.health <= 0) {
						enemy.isActive = false;
						score += 10;
					}
					bullet.isActive = false;
					break;
				}
			}
		}
	}

	// 아이템 충돌 감지
	for (auto& item : items) {
		if (item.isActive && checkCollision(player.x, player.y, player.objectSize.cx, player.objectSize.cy, item.x, item.y, item.itemSize.cx, item.itemSize.cy)) {
			applyItemEffect(item.type);
			item.isActive = false;
		}
	}

	// 플레이어와 적의 충돌 검사
	for (const auto& enemy : enemies) {
		if (enemy.isActive && checkCollision(player.x, player.y, player.objectSize.cx, player.objectSize.cy, enemy.x, enemy.y, enemy.objectSize.cx, enemy.objectSize.cy)) {
			playerHealth--;
			if (playerHealth <= 0) {
				gameState = GameState::GAMEOVER;
			}
			break;
		}
	}

	updateCamera();
}

void spawnEnemy() {
	double x, y;
	bool validPosition;
	do {
		validPosition = true;
		x = static_cast<double>(rand() % (MAP_WIDTH - 50));
		y = static_cast<double>(rand() % (MAP_HEIGHT - 50));

		// 다른 적들과의 충돌 검사
		for (const auto& otherEnemy : enemies) {
			if (otherEnemy.isActive && checkCollision(x, y, 50L, 50L,
				otherEnemy.x, otherEnemy.y, otherEnemy.objectSize.cx, otherEnemy.objectSize.cy)) {
				validPosition = false;
				break;
			}
		}

		// 장애물과의 충돌 검사
		if (validPosition) {
			for (const auto& obstacle : obstacles) {
				if (checkCollision(x, y, 50L, 50L,
					obstacle.x, obstacle.y, obstacle.objectSize.cx, obstacle.objectSize.cy)) {
					validPosition = false;
					break;
				}
			}
		}
	} while (!validPosition);

	enemies.push_back({ x, y, 50, 50, 2.0, true, 1 });  // 초기 체력 1
}

void shootBullet() {
	int bulletWidth = 10;
	int bulletHeight = 20;
	double bulletSpeed = 10.0f;

	for (int i = 0; i < bulletCnt; ++i) {
		double offset = (bulletCnt - 1) * bulletWidth * 0.5f - i * bulletWidth;
		bullets.push_back({ player.x + player.objectSize.cx / 2.0f - bulletWidth / 2.0f + offset, player.y, bulletWidth, bulletHeight, bulletSpeed, true, 1 });
	}
}

void updateCamera() {
	cameraX = player.x - SCREEN_WIDTH / 2.0f + player.objectSize.cx / 2.0f;
	cameraY = player.y - SCREEN_HEIGHT / 2.0f + player.objectSize.cy / 2.0f;

	// 카메라가 월드 경계를 벗어나지 않도록 조정
	cameraX = max(0.0f, min(cameraX, static_cast<double>(MAP_WIDTH - SCREEN_WIDTH)));
	cameraY = max(0.0f, min(cameraY, static_cast<double>(MAP_HEIGHT - SCREEN_HEIGHT)));
}

void initializeObstacles() {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> disX(0, MAP_WIDTH - 100);
	std::uniform_int_distribution<> disY(0, MAP_HEIGHT - 100);
	std::uniform_int_distribution<> disSize(50, 150);

	for (int i = 0; i < 20; ++i) {
		int width = disSize(gen);
		int height = disSize(gen);
		double x = static_cast<double>(disX(gen));
		double y = static_cast<double>(disY(gen));

		// 플레이어 시작 위치와 겹치지 않도록
		if (x < player.x + 100 && x + width > player.x - 100 &&
			y < player.y + 100 && y + height > player.y - 100) {
			continue;
		}

		obstacles.push_back({ x, y, width, height, 0, true, 0 });
	}
}

void spawnItem() {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> disX(0, MAP_WIDTH - 30);
	std::uniform_int_distribution<> disY(0, MAP_HEIGHT - 30);
	std::uniform_int_distribution<> disType(0, 2); // 0~2 범위로 변경

	float x = static_cast<float>(disX(gen));
	float y = static_cast<float>(disY(gen));
	ItemType type = static_cast<ItemType>(disType(gen)); // ItemType으로 형변환

	items.push_back({ x, y, 30, 30, true, type });
}

bool checkCollision(double x1, double y1, LONG w1, LONG h1, double x2, double y2, LONG w2, LONG h2) {
	return x1 < x2 + w2 && x1 + w1 > x2 && y1 < y2 + h2 && y1 + h1 > y2;
}

void applyItemEffect(ItemType type) {
	switch (type) {
	case ItemType::DAMAGEUP:
		damage++;
		break;
	case ItemType::MULTISHOT:
		bulletCnt = min(bulletCnt + 1, 5);  // 최대 5발까지 발사 가능
		break;
	case ItemType::HEAL:
		playerHealth = min(playerHealth + 3, 10);  // 3만큼 회복, 최대 10
		break;
	}
}

void startGame() {
	// 플레이어 초기화
	player = { MAP_WIDTH / 2.0f, MAP_HEIGHT - 100.0f, 50, 50, 5.0f, true, 100 };

	// 장애물 초기화
	initializeObstacles();

	// 게임 시작 시간 기록
	gameStartTime = std::chrono::steady_clock::now();
}

void ResetGame() {
	// 게임 상태 초기화
	score = 0;
	damage = 1;
	bulletCnt = 1;
	playerHealth = 10;

	// 적, 총알, 아이템 초기화
	enemies.clear();
	bullets.clear();
	items.clear();

	// 플레이어 위치 초기화
	player.x = MAP_WIDTH / 2.0f;
	player.y = MAP_HEIGHT - 100.0f;

	// 카메라 위치 초기화
	updateCamera();
}