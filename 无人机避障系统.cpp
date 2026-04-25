#include <easyx/graphics.h>
#include <stdio.h>

#pragma comment(lib,"Winmm.lib")
#pragma comment(lib,"MSIMG32.LIB")

enum GameState { MENU, SETTINGS, PLAYING, PAUSED, GAME_OVER };

struct Settings {
	bool soundOn;
	int volume;
	int difficulty;
};

GameState g_state = MENU;
Settings g_settings = { true, 100, 1 };
int g_menuSelection = 0;
bool g_exitFlag = false;

const int top = 0;
const int left = 0;
const int right = 65;
const int bottom = 65;
const int landY = 600;
const int landX = 800;
int pipeX[6]={150,150,350,350,550,550};
int pipeY[6]={-350,350,-200,500,-400,300};

void transparentimage3(IMAGE* dstimg, int x, int y, IMAGE* srcimg)
{
    HDC dstDC = GetImageHDC(dstimg);
    HDC srcDC = GetImageHDC(srcimg);
    int w = srcimg->getwidth();
    int h = srcimg->getheight();
    BLENDFUNCTION bf = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
    AlphaBlend(dstDC, x, y, w, h, srcDC, 0, 0, w, h, bf);
}

IMAGE img_bk,img_bd1,img_bd2,img_bar_up1,img_bar_up2,img_bar_down1,img_bar_down2,img_game_over;
IMAGE img_uav;
int uav_x,UAVX;
int uav_y,UAVY;

DWORD g_gameStartTime = 0;
DWORD g_pausedTime = 0;
DWORD g_pauseStartTime = 0;
DWORD g_finalTime = 0;
float g_autoMoveAccum = 0;

#define Folder "resources"

RECT g_btnStart = {250, 200, 550, 260};
RECT g_btnSettings = {250, 290, 550, 350};
RECT g_btnExit = {250, 380, 550, 440};
RECT g_btnBack = {250, 460, 550, 520};

RECT g_btnSoundToggle = {420, 175, 500, 215};
RECT g_btnVolMinus = {350, 245, 410, 285};
RECT g_btnVolPlus = {420, 245, 480, 285};
RECT g_btnDiff0 = {300, 315, 370, 360};
RECT g_btnDiff1 = {390, 315, 460, 360};
RECT g_btnDiff2 = {480, 315, 550, 360};

RECT g_btnPause = {740, 10, 790, 45};
RECT g_btnResume = {250, 230, 550, 290};
RECT g_btnMenuFromPause = {250, 330, 550, 390};

RECT g_btnRestart = {250, 330, 550, 390};
RECT g_btnMenuFromOver = {250, 420, 550, 480};

RECT g_dpadUp = {720, 430, 760, 475};
RECT g_dpadDown = {720, 515, 760, 560};
RECT g_dpadLeft = {680, 475, 720, 515};
RECT g_dpadRight = {760, 475, 800, 515};

bool isInside(RECT r, int x, int y) {
	return x >= r.left && x <= r.right && y >= r.top && y <= r.bottom;
}

void drawButton(int x, int y, int w, int h, const char* text, bool hover) {
	if (hover) {
		setfillcolor(RGB(80, 180, 255));
	} else {
		setfillcolor(RGB(60, 140, 220));
	}
	fillroundrect(x, y, x + w, y + h, 10, 10);
	settextcolor(WHITE);
	setbkmode(TRANSPARENT);
	settextstyle(24, 0, "微软雅黑");
	int tw = textwidth(text);
	int th = textheight(text);
	outtextxy(x + (w - tw) / 2, y + (h - th) / 2, text);
}

void drawButtonRect(RECT r, const char* text, bool hover) {
	drawButton(r.left, r.top, r.right - r.left, r.bottom - r.top, text, hover);
}

void playJumpSound() {
	if (!g_settings.soundOn) return;
	mciSendString("close jpmusic", NULL, 0, NULL);
	mciSendString("open Jump.mp3 alias jpmusic", NULL, 0, NULL);
	mciSendString("play jpmusic", NULL, 0, NULL);
}

void startBackgroundMusic() {
	mciSendString("close bkmusic", NULL, 0, NULL);
	if (g_settings.soundOn) {
		mciSendString("open background.mp3 alias bkmusic", NULL, 0, NULL);
		mciSendString("play bkmusic repeat", NULL, 0, NULL);
	}
}

void stopBackgroundMusic() {
	mciSendString("close bkmusic", NULL, 0, NULL);
}

void resetGame() {
	uav_x = 0;
	uav_y = 500;
	UAVX = 0;
	UAVY = 500;
	g_gameStartTime = GetTickCount();
	g_autoMoveAccum = 0;
}

void startup()
{
	initgraph(800, 600);
	loadimage(&img_game_over, "./" Folder "/game_over.jpg");
	loadimage(&img_bk, "./" Folder "/background.jpg");
	loadimage(&img_uav, "./" Folder "/UAV12.png");
	loadimage(&img_bar_up1, "./" Folder "/bar_up1.gif");
	loadimage(&img_bar_up2, "./" Folder "/bar_up2.gif");
	loadimage(&img_bar_down1, "./" Folder "/bar_down1.gif");
	loadimage(&img_bar_down2, "./" Folder "/bar_down2.gif");
	loadimage(&img_bd1, "./" Folder "/UAV1.png");
	loadimage(&img_bd2, "./" Folder "/UAV12.png");
	uav_x = 0;
	uav_y = 500;
	BeginBatchDraw();
}

void renderMenu() {
	putimage(0, 0, &img_bk);

	settextcolor(RGB(0, 180, 255));
	settextstyle(48, 0, "微软雅黑");
	setbkmode(TRANSPARENT);
	RECT tr = {0, 60, 800, 140};
	drawtext("无人机避障系统", &tr, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	int mx = -1, my = -1;
	ExMessage m;
	while (peekmessage(&m, EM_MOUSE)) { if (m.message == WM_MOUSEMOVE) { mx = m.x; my = m.y; } }

	bool hoverStart = isInside(g_btnStart, mx, my);
	bool hoverSettings = isInside(g_btnSettings, mx, my);
	bool hoverExit = isInside(g_btnExit, mx, my);

	drawButtonRect(g_btnStart, "开始游戏", hoverStart);
	drawButtonRect(g_btnSettings, "设置", hoverSettings);
	drawButtonRect(g_btnExit, "退出", hoverExit);

	FlushBatchDraw();
	Sleep(20);
}

void handleMenuInput(ExMessage* msg) {
	if (msg->message == WM_LBUTTONDOWN) {
		if (isInside(g_btnStart, msg->x, msg->y)) {
			resetGame();
			startBackgroundMusic();
			g_state = PLAYING;
		} else if (isInside(g_btnSettings, msg->x, msg->y)) {
			g_state = SETTINGS;
		} else if (isInside(g_btnExit, msg->x, msg->y)) {
			g_exitFlag = true;
		}
	}
}

void renderSettings() {
	putimage(0, 0, &img_bk);

	settextcolor(RGB(0, 180, 255));
	settextstyle(40, 0, "微软雅黑");
	setbkmode(TRANSPARENT);
	RECT tr = {0, 40, 800, 110};
	drawtext("设置", &tr, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	int mx = -1, my = -1;
	ExMessage m;
	while (peekmessage(&m, EM_MOUSE)) { if (m.message == WM_MOUSEMOVE) { mx = m.x; my = m.y; } }

	settextcolor(WHITE);
	settextstyle(24, 0, "微软雅黑");
	setbkmode(TRANSPARENT);

	char buf[64];
	sprintf(buf, "音效: %s", g_settings.soundOn ? "开" : "关");
	RECT sr = {200, 175, 410, 215};
	drawtext(buf, &sr, DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
	drawButtonRect(g_btnSoundToggle, g_settings.soundOn ? "ON" : "OFF", isInside(g_btnSoundToggle, mx, my));

	sprintf(buf, "音量: %d%%", g_settings.volume);
	RECT vr = {200, 245, 340, 285};
	drawtext(buf, &vr, DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
	drawButtonRect(g_btnVolMinus, "-", isInside(g_btnVolMinus, mx, my));
	drawButtonRect(g_btnVolPlus, "+", isInside(g_btnVolPlus, mx, my));

	settextcolor(WHITE);
	RECT dr = {200, 315, 290, 360};
	drawtext("难度:", &dr, DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
	drawButtonRect(g_btnDiff0, "低", g_settings.difficulty == 0 || isInside(g_btnDiff0, mx, my));
	drawButtonRect(g_btnDiff1, "中", g_settings.difficulty == 1 || isInside(g_btnDiff1, mx, my));
	drawButtonRect(g_btnDiff2, "高", g_settings.difficulty == 2 || isInside(g_btnDiff2, mx, my));

	drawButtonRect(g_btnBack, "返回", isInside(g_btnBack, mx, my));

	FlushBatchDraw();
	Sleep(20);
}

void handleSettingsInput(ExMessage* msg) {
	if (msg->message != WM_LBUTTONDOWN) return;
	if (isInside(g_btnSoundToggle, msg->x, msg->y)) {
		g_settings.soundOn = !g_settings.soundOn;
	} else if (isInside(g_btnVolMinus, msg->x, msg->y)) {
		if (g_settings.volume > 0) g_settings.volume -= 10;
	} else if (isInside(g_btnVolPlus, msg->x, msg->y)) {
		if (g_settings.volume < 100) g_settings.volume += 10;
	} else if (isInside(g_btnDiff0, msg->x, msg->y)) {
		g_settings.difficulty = 0;
	} else if (isInside(g_btnDiff1, msg->x, msg->y)) {
		g_settings.difficulty = 1;
	} else if (isInside(g_btnDiff2, msg->x, msg->y)) {
		g_settings.difficulty = 2;
	} else if (isInside(g_btnBack, msg->x, msg->y)) {
		g_state = MENU;
	}
}

void renderHUD() {
	setfillcolor(RGB(0, 0, 0));
	setbkmode(TRANSPARENT);

	COLORREF c = RGB(0, 255, 100);
	settextcolor(c);
	settextstyle(18, 0, "Consolas");

	DWORD elapsed;
	if (g_state == PAUSED) {
		elapsed = g_pausedTime;
	} else {
		elapsed = (GetTickCount() - g_gameStartTime) / 1000;
	}

	char buf[128];
	const char* diffNames[] = {"低", "中", "高"};
	sprintf(buf, "高度:%d  水平:%d  时间:%lus  难度:%s",
		landY - uav_y, uav_x, elapsed, diffNames[g_settings.difficulty]);

	RECT hudRect = {10, 10, 580, 36};
	setfillcolor(RGB(0, 40, 0));
	solidroundrect(10, 10, 580, 36, 5, 5);
	drawtext(buf, &hudRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
}

void renderDpad() {
	setfillcolor(RGB(50, 50, 50));
	solidroundrect(680, 430, 800, 560, 8, 8);

	setfillcolor(RGB(100, 100, 100));
	fillroundrect(720, 430, 760, 475, 5, 5);
	fillroundrect(720, 515, 760, 560, 5, 5);
	fillroundrect(680, 475, 720, 515, 5, 5);
	fillroundrect(760, 475, 800, 515, 5, 5);

	settextcolor(WHITE);
	setbkmode(TRANSPARENT);
	settextstyle(18, 0, "微软雅黑");

	RECT tr;
	tr = {720, 430, 760, 475}; drawtext("▲", &tr, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	tr = {720, 515, 760, 560}; drawtext("▼", &tr, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	tr = {680, 475, 720, 515}; drawtext("◄", &tr, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	tr = {760, 475, 800, 515}; drawtext("►", &tr, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	setfillcolor(RGB(60, 140, 220));
	fillroundrect(740, 10, 790, 45, 5, 5);
	settextcolor(WHITE);
	settextstyle(16, 0, "微软雅黑");
	RECT pr = {740, 10, 790, 45};
	drawtext("暂停", &pr, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

void handleDpadClick(int x, int y) {
	if (isInside(g_dpadUp, x, y) && uav_y > 21) {
		uav_y -= 20;
		playJumpSound();
	} else if (isInside(g_dpadDown, x, y) && uav_y < 581) {
		uav_y += 20;
		playJumpSound();
	} else if (isInside(g_dpadLeft, x, y) && uav_x > 21) {
		uav_x -= 20;
		playJumpSound();
	} else if (isInside(g_dpadRight, x, y) && uav_x < 781) {
		uav_x += 20;
		playJumpSound();
	}
}

void renderPauseMenu() {
	putimage(0, 0, &img_bk);

	setfillcolor(RGB(0, 0, 0));
	COLORREF c = RGB(0, 255, 100);
	settextcolor(c);
	settextstyle(18, 0, "Consolas");
	RECT hr = {10, 10, 580, 36};
	char buf[128];
	const char* diffNames[] = {"低", "中", "高"};
	sprintf(buf, "高度:%d  水平:%d  时间:%lus  难度:%s [暂停]",
		landY - uav_y, uav_x, g_pausedTime, diffNames[g_settings.difficulty]);
	setfillcolor(RGB(0, 40, 0));
	solidroundrect(10, 10, 580, 36, 5, 5);
	drawtext(buf, &hr, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

	setfillcolor(RGB(0, 0, 0));
	solidroundrect(200, 150, 600, 450, 12, 12);

	settextcolor(RGB(255, 200, 50));
	settextstyle(40, 0, "微软雅黑");
	setbkmode(TRANSPARENT);
	RECT tr = {200, 155, 600, 220};
	drawtext("游戏暂停", &tr, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	int mx = -1, my = -1;
	ExMessage m;
	while (peekmessage(&m, EM_MOUSE)) { if (m.message == WM_MOUSEMOVE) { mx = m.x; my = m.y; } }

	drawButtonRect(g_btnResume, "继续游戏", isInside(g_btnResume, mx, my));
	drawButtonRect(g_btnMenuFromPause, "返回主菜单", isInside(g_btnMenuFromPause, mx, my));

	FlushBatchDraw();
	Sleep(20);
}

void handlePauseInput(ExMessage* msg) {
	if (msg->message != WM_LBUTTONDOWN) return;
	if (isInside(g_btnResume, msg->x, msg->y)) {
		DWORD now = GetTickCount();
		g_gameStartTime += (now - g_pauseStartTime);
		g_state = PLAYING;
	} else if (isInside(g_btnMenuFromPause, msg->x, msg->y)) {
		stopBackgroundMusic();
		g_state = MENU;
	}
}

void renderGameOver() {
	putimage(0, 0, &img_bk);
	putimage(100, 60, &img_game_over);

	setfillcolor(RGB(0, 0, 0));
	solidroundrect(200, 280, 600, 350, 10, 10);
	settextcolor(RGB(255, 200, 50));
	settextstyle(24, 0, "微软雅黑");
	setbkmode(TRANSPARENT);
	char buf[64];
	sprintf(buf, "存活时间: %lu 秒  难度: %s",
		g_finalTime, (const char*[]){ "低", "中", "高" }[g_settings.difficulty]);
	RECT tr = {205, 285, 595, 345};
	drawtext(buf, &tr, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	int mx = -1, my = -1;
	ExMessage m;
	while (peekmessage(&m, EM_MOUSE)) { if (m.message == WM_MOUSEMOVE) { mx = m.x; my = m.y; } }

	drawButtonRect(g_btnRestart, "重新开始", isInside(g_btnRestart, mx, my));
	drawButtonRect(g_btnMenuFromOver, "返回主菜单", isInside(g_btnMenuFromOver, mx, my));

	FlushBatchDraw();
	Sleep(20);
}

void handleGameOverInput(ExMessage* msg) {
	if (msg->message != WM_LBUTTONDOWN) return;
	if (isInside(g_btnRestart, msg->x, msg->y)) {
		resetGame();
		startBackgroundMusic();
		g_state = PLAYING;
	} else if (isInside(g_btnMenuFromOver, msg->x, msg->y)) {
		g_state = MENU;
	}
}

void show()
{
	putimage(0, 0, &img_bk);
	putimage(pipeX[0], pipeY[0], &img_bar_up1,NOTSRCERASE);
	putimage(pipeX[0], pipeY[0], &img_bar_up2,SRCINVERT);
	putimage(pipeX[1], pipeY[1], &img_bar_down1,NOTSRCERASE);
	putimage(pipeX[1], pipeY[1], &img_bar_down2,SRCINVERT);

	putimage(pipeX[2], pipeY[2], &img_bar_up1,NOTSRCERASE);
	putimage(pipeX[2], pipeY[2], &img_bar_up2,SRCINVERT);
	putimage(pipeX[3], pipeY[3], &img_bar_down1,NOTSRCERASE);
	putimage(pipeX[3], pipeY[3], &img_bar_down2,SRCINVERT);
	
	putimage(pipeX[4], pipeY[4],&img_bar_up1,NOTSRCERASE);
	putimage(pipeX[4], pipeY[4], &img_bar_up2,SRCINVERT);
	putimage(pipeX[5], pipeY[5], &img_bar_down1,NOTSRCERASE);
	putimage(pipeX[5], pipeY[5], &img_bar_down2,SRCINVERT);

	transparentimage3(NULL, uav_x, uav_y, &img_uav);
	UAVX=uav_x;
	UAVY=uav_y;

	renderHUD();
	renderDpad();

	FlushBatchDraw();
	Sleep(60);
}

void updateWithoutInput()
{
	float speed;
	switch (g_settings.difficulty) {
		case 0: speed = 0.5f; break;
		case 2: speed = 1.5f; break;
		default: speed = 1.0f; break;
	}
	g_autoMoveAccum += speed;
	while (g_autoMoveAccum >= 1.0f) {
		if (uav_y > 0) uav_y = uav_y - 1;
		g_autoMoveAccum -= 1.0f;
	}
}

void handlePlayingKeyboard(WORD vkcode) {
	switch (vkcode) {
	case 'W':
		if (uav_y > 21) { uav_y -= 20; playJumpSound(); }
		break;
	case 'S':
		if (uav_y < 581) { uav_y += 20; playJumpSound(); }
		break;
	case 'A':
		if (uav_x > 21) { uav_x -= 20; playJumpSound(); }
		break;
	case 'D':
		if (uav_x < 781) { uav_x += 20; playJumpSound(); }
		break;
	case VK_ESCAPE:
		g_pausedTime = (GetTickCount() - g_gameStartTime) / 1000;
		g_pauseStartTime = GetTickCount();
		g_state = PAUSED;
		break;
	}
}

bool isDie()
{
	if (uav_y ==0)
		return true;
	if (uav_y + bottom > landY)
		return true;
	if (uav_x + right > landX)
		return true;
	if (uav_x + right > pipeX[0] && uav_x + left < pipeX[0] + 72)
	{
		if (uav_y + top < pipeY[0] + 589 || uav_y + bottom > pipeY[0] + 716)
			return true;
	}

	if (uav_x + right > pipeX[1] && uav_x + left < pipeX[1] + 72)
	{
		if (uav_y + top < pipeY[1] -111 || uav_y + bottom > pipeY[1] + 16)
			return true;
	}

	if (uav_x + right > pipeX[2] && uav_x + left < pipeX[2] + 72)
	{
		if (uav_y + top < pipeY[2] + 589 || uav_y + bottom > pipeY[2] + 716)
			return true;
	}

	if (uav_x + right > pipeX[3] && uav_x + left < pipeX[3] + 72)
	{
		if (uav_y + top < pipeY[3] -111 || uav_y + bottom > pipeY[3] + 16)
			return true;
	}
	if (uav_x + right > pipeX[4] && uav_x + left < pipeX[4] + 72)
	{
		if (uav_y + top < pipeY[4] + 589 || uav_y + bottom > pipeY[4] + 716)
			return true;
	}

	if (uav_x + right > pipeX[5] && uav_x + left < pipeX[5] + 72)
	{
		if (uav_y + top < pipeY[5] -111 || uav_y + bottom > pipeY[5] + 16)
			return true;
	}
	return false;
}

int main()
{
	startup();

	while (!g_exitFlag)
	{
		ExMessage msg;
		switch (g_state)
		{
		case MENU:
			renderMenu();
			while (peekmessage(&msg, EM_MOUSE | EM_KEY)) {
				handleMenuInput(&msg);
			}
			break;

		case SETTINGS:
			renderSettings();
			while (peekmessage(&msg, EM_MOUSE | EM_KEY)) {
				handleSettingsInput(&msg);
			}
			break;

		case PLAYING:
			updateWithoutInput();
			show();
			while (peekmessage(&msg, EM_MOUSE | EM_KEY)) {
				if (msg.message == WM_KEYDOWN) {
					handlePlayingKeyboard(msg.vkcode);
				} else if (msg.message == WM_LBUTTONDOWN) {
					if (isInside(g_btnPause, msg.x, msg.y)) {
						g_pausedTime = (GetTickCount() - g_gameStartTime) / 1000;
						g_pauseStartTime = GetTickCount();
						g_state = PAUSED;
					} else {
						handleDpadClick(msg.x, msg.y);
					}
				}
			}
			if (isDie()) {
				g_finalTime = (GetTickCount() - g_gameStartTime) / 1000;
				stopBackgroundMusic();
				mciSendString("close jpmusic", NULL, 0, NULL);
				mciSendString("open explode.mp3 alias expmusic", NULL, 0, NULL);
				mciSendString("play expmusic", NULL, 0, NULL);
				g_state = GAME_OVER;
			}
			break;

		case PAUSED:
			renderPauseMenu();
			while (peekmessage(&msg, EM_MOUSE | EM_KEY)) {
				handlePauseInput(&msg);
			}
			break;

		case GAME_OVER:
			renderGameOver();
			while (peekmessage(&msg, EM_MOUSE | EM_KEY)) {
				handleGameOverInput(&msg);
			}
			break;
		}
	}

	mciSendString("close jpmusic", NULL, 0, NULL);
	mciSendString("close bkmusic", NULL, 0, NULL);
	mciSendString("close expmusic", NULL, 0, NULL);
	EndBatchDraw();
	closegraph();
	return 0;
}
