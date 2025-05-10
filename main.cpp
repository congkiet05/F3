#include <graphics.h>
#include <conio.h>
#include <windows.h>
#include <sstream>
#include <string>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 400
#define GROUND_Y 300
#define CACTUS_WIDTH 20
#define CACTUS_HEIGHT 60
#define JUMP_SPEED -22
#define GRAVITY 3

bool checkCollision(int x1, int y1, int w1, int h1,
                    int x2, int y2, int w2, int h2) {
    return !(x1 + w1 < x2 || x2 + w2 < x1 ||
             y1 + h1 < y2 || y2 + h2 < y1);
}
//ve bau troi 
void drawSky() {
    setfillstyle(SOLID_FILL, LIGHTCYAN);
    bar(0, 0, getmaxx(), 120);
}

void drawBird(int x, int y) {
    setcolor(BLACK);
    arc(x - 20, y, 0, 180, 10);
    arc(x + 0, y, 0, 180, 10);
}
void drawCloud(int x, int y) {
    setcolor(WHITE);
    setfillstyle(SOLID_FILL, WHITE);
    fillellipse(x, y, 30, 20);
    fillellipse(x + 20, y - 10, 30, 20);
    fillellipse(x + 40, y, 30, 20);
}
//ve dau chim 
void drawBirdHead(int x, int y) {
    setcolor(DARKGRAY);
    setfillstyle(SOLID_FILL, DARKGRAY);
    circle(x, y, 20);
    floodfill(x, y, DARKGRAY);

    setcolor(WHITE);
    setfillstyle(SOLID_FILL, WHITE);
    circle(x - 8, y - 5, 4);
    floodfill(x - 8, y - 5, WHITE);
    
    circle(x + 8, y - 5, 4);
    floodfill(x + 8, y - 5, WHITE);
}
//ve than chim 
void drawBirdBody(int x, int y) {
    setcolor(DARKGRAY);
    setfillstyle(SOLID_FILL, DARKGRAY);
    ellipse(x + 30, y, 0, 360, 20, 25);
    floodfill(x + 30, y, DARKGRAY);
}
//ve chan chim 
void drawBirdLeg(int x, int y) {
    setcolor(DARKGRAY);
    line(x, y + 40, x, y + 60);
    line(x + 10, y + 40, x + 10, y + 60);
}
//ve canh trai 
void drawLeftWing(int x, int y) {
    setcolor(DARKGRAY);
    setfillstyle(SOLID_FILL, DARKGRAY);
    int wing[6] = {x, y, x - 30, y - 20, x - 10, y + 10};
    fillpoly(3, wing);
}
//ve canh phai 
void drawRightWing(int x, int y) {
    setcolor(DARKGRAY);
    setfillstyle(SOLID_FILL, DARKGRAY);
    int wing[6] = {x, y, x + 30, y - 20, x + 10, y + 10};
    fillpoly(3, wing);
}
//ve chim 
void drawBirdCartoon(int x, int y) {
    drawBirdHead(x + 30, y - 5);
    drawBirdBody(x, y + 20);
    drawBirdLeg(x + 25, y);
    drawLeftWing(x + 18, y + 20);
    drawRightWing(x + 40, y + 20);
}

void bresenhamLine(int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1), dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (true) {
        putpixel(x1, y1, GREEN);
        if (x1 == x2 && y1 == y2) break;
        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x1 += sx; }
        if (e2 < dx) { err += dx; y1 += sy; }
    }
}
//ve xuong rong theo bresenham 
void drawCactus(int x, int y) {
    setcolor(GREEN);
    setfillstyle(SOLID_FILL, GREEN);
    
    bresenhamLine(x - 6, y, x + 6, y);          
    bresenhamLine(x - 6, y, x - 6, y + 60);     
    bresenhamLine(x + 6, y, x + 6, y + 60);    
    bresenhamLine(x - 6, y + 60, x + 6, y + 60);

    bresenhamLine(x - 20, y + 20, x - 14, y + 20);
    bresenhamLine(x - 20, y + 20, x - 20, y + 40);
    bresenhamLine(x - 14, y + 20, x - 14, y + 40);
    bresenhamLine(x - 20, y + 40, x - 14, y + 40);

    bresenhamLine(x + 14, y + 20, x + 20, y + 20);
    bresenhamLine(x + 14, y + 20, x + 14, y + 40);
    bresenhamLine(x + 20, y + 20, x + 20, y + 40);
    bresenhamLine(x + 14, y + 40, x + 20, y + 40);

    bresenhamLine(x - 14, y + 38, x - 8, y + 38);
    bresenhamLine(x - 14, y + 38, x - 14, y + 42);
    bresenhamLine(x - 8, y + 38, x - 8, y + 42);
    bresenhamLine(x - 14, y + 42, x - 8, y + 42);

    bresenhamLine(x + 8, y + 38, x + 14, y + 38);
    bresenhamLine(x + 8, y + 38, x + 8, y + 42);
    bresenhamLine(x + 14, y + 38, x + 14, y + 42);
    bresenhamLine(x + 8, y + 42, x + 14, y + 42);

    floodfill(x, y + 30, GREEN);
    floodfill(x - 17, y + 30, GREEN);
    floodfill(x + 17, y + 30, GREEN);
    floodfill(x - 11, y + 40, GREEN);
    floodfill(x + 11, y + 40, GREEN);
}

int main() {
    initwindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Dinosaur Game", 0, 0, false, true);
    srand(time(0));

    setbkcolor(WHITE);
    cleardevice();

    int dinoX = 50;
    int dinoY = GROUND_Y - 60;
    int velocityY = 0;
    bool isJumping = false;

    int cactusX[2] = {SCREEN_WIDTH, SCREEN_WIDTH + 200};
    int cactusY[2] = {GROUND_Y - 60, GROUND_Y - 60};
    int bird2X = 420, bird2Y = 30; 
    int bird3X = 470, bird3Y = 40;
    bool hasTwoCacti = false;

    int score = 0;
    int cactusSpeed = 10;
    bool gameOver = false;
    int currentPage = 0;

    int cloudX[3] = {200, 500, 750};
    int cloudY[3] = {50, 70, 90};

    while (true) {
        setactivepage(currentPage);
        setvisualpage(1 - currentPage);
        cleardevice();
		//ve mat dat 
        setcolor(BLACK);
        line(0, GROUND_Y, SCREEN_WIDTH, GROUND_Y);
		
		drawSky();
        for (int i = 0; i < 3; i++) {
        	drawCloud(cloudX[i], cloudY[i]);
            cloudX[i] -= 2;  // T?c ð? mây ch?m hõn chý?ng ng?i v?t
            if (cloudX[i] < -50) {
                cloudX[i] = SCREEN_WIDTH + rand() % 100;
                cloudY[i] = 30 + rand() % 60;
            }
        }
		bird2X -= 4; 
		bird3X -= 3;  
		if (bird2X < -20) {
		    bird2X = SCREEN_WIDTH + rand() % 100; 
		    bird2Y = 30 + rand() % 40;  
		}
		if (bird3X < -20) {
		    bird3X = SCREEN_WIDTH + rand() % 150;  
		    bird3Y = 30 + rand() % 40;  
		}
		drawBird(bird2X, bird2Y); 
		drawBird(bird3X, bird3Y);

        // Xu ly phim  
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) break;
        if ((GetAsyncKeyState(VK_SPACE) & 0x8000) && !isJumping && !gameOver) {
        	Beep(800, 100); 
            velocityY = JUMP_SPEED;
            isJumping = true;
        }
        if ((GetAsyncKeyState('R') & 0x8000) && gameOver) {
            dinoY = GROUND_Y - 60;
            velocityY = 0;
            isJumping = false;
            cactusX[0] = SCREEN_WIDTH;
            cactusX[1] = SCREEN_WIDTH + 300;
            score = 0;
            cactusSpeed = 10;
            hasTwoCacti = false;
            gameOver = false;
        }

        // Xu ly nhay 
        if (!gameOver) {
            dinoY += velocityY;
            velocityY += GRAVITY;
            if (dinoY >= GROUND_Y - 60) {
                dinoY = GROUND_Y - 60;
                velocityY = 0;
                isJumping = false;
            }

            for (int i = 0; i < (hasTwoCacti ? 2 : 1); i++) {
                cactusX[i] -= cactusSpeed;
                if (cactusX[i] < -CACTUS_WIDTH) {
                    cactusX[i] = SCREEN_WIDTH + rand() % 200;
                    cactusY[i] = GROUND_Y - 100;
                    score += 10;
                    if (score % 50 == 0 && cactusSpeed < 30) cactusSpeed++;
                    if (score >= 50) hasTwoCacti = true;
                }
            }

            if (checkCollision(dinoX, dinoY, 50, 60,
                               cactusX[0], cactusY[0], CACTUS_WIDTH, CACTUS_HEIGHT) ||
                (hasTwoCacti && checkCollision(dinoX, dinoY, 50, 60,
                                               cactusX[1], cactusY[1], CACTUS_WIDTH, CACTUS_HEIGHT))) {
                gameOver = true;
            }
			
        }
		//ve chim 
        drawBirdCartoon(dinoX + 20, dinoY);
		//ve xuong rong 
        for (int i = 0; i < (hasTwoCacti ? 2 : 1); i++) {
            drawCactus(cactusX[i], cactusY[i]);
        }

        // Ve diem 
        settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 1);
        setcolor(RED);
        std::ostringstream oss;
        oss << "Score: " << score;
        outtextxy(SCREEN_WIDTH - 150, 10, const_cast<char*>(oss.str().c_str()));

        // Game Over
        if (gameOver) {
            setcolor(RED);
            outtextxy(SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2, "GAME OVER");
            outtextxy(SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 + 20, "Press R to restart");
        }

        currentPage = 1 - currentPage;
        delay(1000 / 60);  
    }

    closegraph();
    return 0;
}

