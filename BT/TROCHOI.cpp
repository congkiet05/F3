#include <graphics.h>
#include <conio.h>
#include <windows.h> // C?n cho Beep
#include <sstream>
#include <string>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
// Define BGI colors (Gi? nguyên)
#define BLACK         0
#define BLUE          1
#define GREEN         2
#define CYAN          3
#define RED           4
#define MAGENTA       5
#define BROWN         6
#define LIGHTGRAY     7
#define DARKGRAY      8
#define LIGHTBLUE     9
#define LIGHTGREEN   10
#define LIGHTCYAN    11
#define LIGHTRED     12
#define LIGHTMAGENTA 13
#define YELLOW       14
#define WHITE        15

#define ORANGE       YELLOW

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 400
#define GROUND_Y 300
#define JUMP_SPEED -28
#define HIGH_JUMP_SPEED -35
#define CLOSE_CACTI_THRESHOLD 70 // Ngu?ng kho?ng cách d? kích ho?t nh?y cao
#define GRAVITY 3

#ifdef __cplusplus
extern "C" {
#endif
int textwidth(char *textstring); // Ho?c int __stdcall textwidth(char far *textstring); tùy phiên b?n
#ifdef __cplusplus
}
#endif
// Hàm ki?m tra va ch?m (Gi? nguyên)
bool checkCollision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2) {
    return !(x1 + w1 < x2 || x2 + w2 < x1 || y1 + h1 < y2 || y2 + h2 < y1);
}

// Hàm v? chim don gi?n (Gi? nguyên)
void drawBird(int x, int y, int color) {
    setcolor(color);
    arc(x - 20, y, 0, 180, 10);
    arc(x + 0, y, 0, 180, 10);
}

// Hàm v? mây (Gi? nguyên)
void drawCloud(int x, int y) {
    setcolor(WHITE);
    setfillstyle(SOLID_FILL, WHITE);
    fillellipse(x, y, 30, 20);
    fillellipse(x + 20, y - 10, 30, 20);
    fillellipse(x + 40, y, 30, 20);
}

// Hàm v? du?ng th?ng Bresenham (Gi? nguyên)
void bresenhamLine(int x1, int y1, int x2, int y2, int color) {
    setcolor(color);
    int dx = abs(x2 - x1), dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (true) {
        putpixel(x1, y1, color);
        if (x1 == x2 && y1 == y2) break;
        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x1 += sx; }
        if (e2 < dx) { err += dx; y1 += sy; }
    }
}

// Hàm v? d?u chim (Gi? nguyên)
void drawBirdHead(int x, int y, bool isGameOver) {
    setcolor(ORANGE);
    setfillstyle(SOLID_FILL, ORANGE);
    circle(x + 18, y - 3, 12);
    floodfill(x + 18, y - 3, ORANGE);

    setcolor(BROWN);
    setfillstyle(SOLID_FILL, BROWN);
    int beak_points[6] = {x + 18 - 3, y - 0, x + 18 + 3, y - 0, x + 18, y + 4};
    fillpoly(3, beak_points);

    if (!isGameOver) {
        setcolor(BLACK);
        setfillstyle(SOLID_FILL, BLACK);
        circle(x + 18 - 5, y - 3 - 3, 2);
        floodfill(x + 18 - 5, y - 3 - 3, BLACK);
        circle(x + 18 + 5, y - 3 - 3, 2);
        floodfill(x + 18 + 5, y - 3 - 3, BLACK);
    } else {
        setcolor(BLACK);
        bresenhamLine(x + 18 - 5 - 2, y - 3 - 3 - 2, x + 18 - 5 + 2, y - 3 - 3 + 2, BLACK);
        bresenhamLine(x + 18 - 5 - 2, y - 3 - 3 + 2, x + 18 - 5 + 2, y - 3 - 3 - 2, BLACK);
        bresenhamLine(x + 18 + 5 - 2, y - 3 - 3 - 2, x + 18 + 5 + 2, y - 3 - 3 + 2, BLACK);
        bresenhamLine(x + 18 + 5 - 2, y - 3 - 3 + 2, x + 18 + 5 + 2, y - 3 - 3 - 2, BLACK);
    }
}

// Hàm v? thân chim (Gi? nguyên)
void drawBirdBody(int x, int y) {
    setcolor(ORANGE);
    setfillstyle(SOLID_FILL, ORANGE);
    ellipse(x + 18, y + 12, 0, 360, 12, 15);
    floodfill(x + 18, y + 12, ORANGE);
}

// Hàm v? chân chim (Gi? nguyên)
void drawBirdLeg(int x, int y) {
    setcolor(BROWN);
    line(x + 15, y + 24, x + 15, y + 36);
    line(x + 15 + 6, y + 24, x + 15 + 6, y + 36);
}

// Hàm v? cánh trái (Gi? nguyên)
void drawLeftWing(int x, int y) {
    setcolor(ORANGE);
    setfillstyle(SOLID_FILL, ORANGE);
    int wing[6] = {x + 10, y + 12, x + 10 - 18, y + 12 - 12, x + 10 - 6, y + 12 + 6};
    fillpoly(3, wing);
}

// Hàm v? cánh ph?i (Gi? nguyên)
void drawRightWing(int x, int y) {
    setcolor(ORANGE);
    setfillstyle(SOLID_FILL, ORANGE);
    int wing[6] = {x + 24, y + 12, x + 24 + 18, y + 12 - 12, x + 24 + 6, y + 12 + 6};
    fillpoly(3, wing);
}

// Hàm v? toàn b? chim (Gi? nguyên)
void drawBirdCartoon(int x, int y, bool isGameOver) {
    drawBirdHead(x, y, isGameOver);
    drawBirdBody(x, y);
    drawBirdLeg(x, y);
    drawLeftWing(x, y);
    drawRightWing(x, y);
}

// Hàm v? cây xuong r?ng (Gi? nguyên)
void drawCactus(int x, int y, int color) {
    setcolor(color);
    setfillstyle(SOLID_FILL, color);

    bresenhamLine(x - 6, y, x + 6, y, color);
    bresenhamLine(x - 6, y, x - 6, y + 60, color);
    bresenhamLine(x + 6, y, x + 6, y + 60, color);
    bresenhamLine(x - 6, y + 60, x + 6, y + 60, color);

    bresenhamLine(x - 20, y + 20, x - 14, y + 20, color);
    bresenhamLine(x - 20, y + 20, x - 20, y + 40, color);
    bresenhamLine(x - 14, y + 20, x - 14, y + 40, color);
    bresenhamLine(x - 20, y + 40, x - 14, y + 40, color);

    bresenhamLine(x + 14, y + 20, x + 20, y + 20, color);
    bresenhamLine(x + 14, y + 20, x + 14, y + 40, color);
    bresenhamLine(x + 20, y + 20, x + 20, y + 40, color);
    bresenhamLine(x + 14, y + 40, x + 20, y + 40, color);

    bresenhamLine(x - 14, y + 38, x - 8, y + 38, color);
    bresenhamLine(x - 14, y + 38, x - 14, y + 42, color);
    bresenhamLine(x - 8, y + 38, x - 8, y + 42, color);
    bresenhamLine(x - 14, y + 42, x - 8, y + 42, color);

    bresenhamLine(x + 8, y + 38, x + 14, y + 38, color);
    bresenhamLine(x + 8, y + 38, x + 8, y + 42, color);
    bresenhamLine(x + 14, y + 38, x + 14, y + 42, color);
    bresenhamLine(x + 8, y + 42, x + 14, y + 42, color);

    floodfill(x, y + 30, color);
    floodfill(x - 17, y + 30, color);
    floodfill(x + 17, y + 30, color);
    floodfill(x - 11, y + 40, color);
    floodfill(x + 11, y + 40, color);
}

int main() {
    initwindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Dinosaur Game");

    srand(time(0));

    int dinoX = 50;
    int dinoY = GROUND_Y - 60;
    int velocityY = 0;
    bool isJumping = false;

    int cactusX[2] = {SCREEN_WIDTH, SCREEN_WIDTH + 400}; // V? trí ban d?u c?a 2 xuong r?ng
    int cactusY[2] = {GROUND_Y - 60, GROUND_Y - 60};
    int bird2X = 420, bird2Y = 30;
    int bird3X = 470, bird3Y = 40;
    bool hasTwoCacti = false;

    int score = 0;
    int cactusSpeed = 10;
    bool gameOver = false;

    int cloudX[3] = {200, 500, 750};
    int cloudY[3] = {50, 70, 90};

    time_t startTime = time(0);

    int page = 0; // Bi?n d? chuy?n d?i gi?a các trang cho double buffering

    while (true) {
        // ----- B?T Ð?U V? LÊN TRANG ?N -----
        setactivepage(page);

        // Xác d?nh màu s?c d?a trên ngày/dêm
        time_t currentTime = time(0);
        int elapsedTime = difftime(currentTime, startTime);
        bool currentlyIsDay = ((elapsedTime / 8) % 2 == 0);

        int bgColor, groundColor, skyColor, simpleBirdColor, scoreColor, cactusColor;
        if (currentlyIsDay) {
            bgColor = WHITE;
            groundColor = BLACK;
            skyColor = LIGHTCYAN;
            simpleBirdColor = BLACK;
            scoreColor = RED;
            cactusColor = GREEN;
        } else {
            bgColor = DARKGRAY;
            groundColor = LIGHTGRAY;
            skyColor = BLACK;
            simpleBirdColor = WHITE;
            scoreColor = WHITE;
            cactusColor = LIGHTGREEN;
        }

        setbkcolor(bgColor);
        cleardevice();

        setfillstyle(SOLID_FILL, skyColor);
        bar(0, 0, getmaxx(), 120);

        setcolor(groundColor);
        line(0, GROUND_Y, SCREEN_WIDTH, GROUND_Y);

        for (int i = 0; i < 3; i++) {
            drawCloud(cloudX[i], cloudY[i]);
            if (!gameOver) {
                 cloudX[i] -= 2;
            }
            if (cloudX[i] < -50) {
                cloudX[i] = SCREEN_WIDTH + rand() % 100;
                cloudY[i] = 50 + rand() % 60;
            }
        }

        drawBird(bird2X, bird2Y, simpleBirdColor);
        drawBird(bird3X, bird3Y, simpleBirdColor);
        if (!gameOver) {
            bird2X -= 4;
            bird3X -= 3;
        }
        if (bird2X < -20) {
            bird2X = SCREEN_WIDTH + rand() % 100;
            bird2Y = 30 + rand() % 40;
        }
        if (bird3X < -20) {
            bird3X = SCREEN_WIDTH + rand() % 150;
            bird3Y = 30 + rand() % 40;
        }

        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) break;

        if (!gameOver) {
            if ((GetAsyncKeyState(VK_SPACE) & 0x8000) && !isJumping) {
                Beep(800, 50);
                bool triggerHighJump = false;
                // Logic nh?y cao: ki?m tra n?u kh?ng long ? g?n m?t c?m 2 xuong r?ng g?n nhau
                if (hasTwoCacti) {
                    // dinoX là v? trí bên trái c?a kh?ng long, chi?u r?ng kh?ng long kho?ng 40-50px
                    // cactusX[i] là v? trí bên trái c?a xuong r?ng, chi?u r?ng xuong r?ng 40px
                    int dino_front_x = dinoX + 20 + 15; // U?c lu?ng v? trí phía tru?c c?a kh?ng long
                    for(int k=0; k<2; ++k) {
                        if (cactusX[k] > dino_front_x && cactusX[k] < dino_front_x + 150) { // Xuong r?ng ? phía tru?c
                            int other_cactus_idx = 1 - k;
                            // Ki?m tra xem xuong r?ng còn l?i có g?n xuong r?ng này không
                            if (abs(cactusX[k] - cactusX[other_cactus_idx]) <= CLOSE_CACTI_THRESHOLD + 20 && // Kho?ng cách gi?a 2 xuong r?ng
                                abs(cactusX[k] - cactusX[other_cactus_idx]) > 20) { // Và không quá g?n (tránh l?i)
                                triggerHighJump = true;
                                break;
                            }
                        }
                    }
                }
                velocityY = triggerHighJump ? HIGH_JUMP_SPEED : JUMP_SPEED;
                isJumping = true;
            }
        } else { // N?u gameOver
            if (GetAsyncKeyState('R') & 0x8000) {
                dinoY = GROUND_Y - 60;
                velocityY = 0;
                isJumping = false;
                cactusX[0] = SCREEN_WIDTH;
                cactusX[1] = SCREEN_WIDTH + 400 + rand() % 100; // Thêm chút ng?u nhiên cho xuong r?ng th? 2
                cactusY[0] = GROUND_Y - 60;
                cactusY[1] = GROUND_Y - 60;
                score = 0;
                cactusSpeed = 10;
                hasTwoCacti = false;
                gameOver = false;
                startTime = time(0);
                cloudX[0] = 200; cloudY[0] = 50;
                cloudX[1] = 500; cloudY[1] = 70;
                cloudX[2] = 750; cloudY[2] = 90;
                bird2X = 420; bird2Y = 30;
                bird3X = 470; bird3Y = 40;
            }
        }


        if (!gameOver) {
            dinoY += velocityY;
            velocityY += GRAVITY;
            if (dinoY >= GROUND_Y - 60) {
                dinoY = GROUND_Y - 60;
                velocityY = 0;
                isJumping = false;
            }

            int numCactiToUpdate = hasTwoCacti ? 2 : 1;
            for (int i = 0; i < numCactiToUpdate; i++) {
                cactusX[i] -= cactusSpeed;
                 if (cactusX[i] < -40) { // Xuong r?ng dã ra kh?i màn hình
                    if (numCactiToUpdate == 1) { // Ch? có 1 xuong r?ng
                        cactusX[i] = SCREEN_WIDTH + 150 + rand() % 250;
                    } else { // Có 2 xuong r?ng
                        // Ð?t xuong r?ng này sau xuong r?ng còn l?i m?t kho?ng cách h?p lý
                        int otherCactusX = cactusX[1 - i]; // V? trí c?a xuong r?ng kia
                        // Ð?m b?o xuong r?ng m?i xu?t hi?n ? bên ph?i màn hình và sau xuong r?ng còn l?i
                        cactusX[i] = SCREEN_WIDTH + rand() % 100; // V? trí co b?n
                        if (otherCactusX > -40) { // N?u xuong r?ng kia v?n còn trên màn hình ho?c v?a m?i qua
                           // S?A ? ÐÂY: dùng std::max
                           cactusX[i] = std::max(cactusX[i], otherCactusX + 200 + rand() % 200);
                        }
                        // Ð?m b?o nó không xu?t hi?n quá g?n l? trái ngay
                        if (cactusX[i] < SCREEN_WIDTH) cactusX[i] = SCREEN_WIDTH + rand()%50;
                    }
                    cactusY[i] = GROUND_Y - 60;
                    score += 10;
                    if (score >= 50 && !hasTwoCacti) hasTwoCacti = true;
                    if (score > 0 && score % 50 == 0 && cactusSpeed < 20) cactusSpeed++;
                }
            
            }

            // Ki?m tra va ch?m (Gi? nguyên hitbox t? phiên b?n g?c c?a b?n)
            int dino_collision_x = dinoX + 20;
            int dino_collision_y = dinoY + 25;
            int dino_collision_w = 30;
            int dino_collision_h = 60;

            int cactus_collision_offset_x = -20;
            int cactus_collision_offset_y = 0;
            int cactus_effective_w = 40;
            int cactus_effective_h = 60;

            int numCactiToCheck = hasTwoCacti ? 2 : 1;
            for (int i = 0; i < numCactiToCheck; i++) {
                if (checkCollision(dino_collision_x, dino_collision_y, dino_collision_w, dino_collision_h,
                                  cactusX[i] + cactus_collision_offset_x, cactusY[i] + cactus_collision_offset_y,
                                  cactus_effective_w, cactus_effective_h)) {
                    gameOver = true;
                    Beep(400, 300);
                    break;
                }
            }
        }

        drawBirdCartoon(dinoX + 20, dinoY + 25, gameOver);

        int numCactiToDraw = hasTwoCacti ? 2 : 1;
        for (int i = 0; i < numCactiToDraw; i++) {
            drawCactus(cactusX[i], cactusY[i], cactusColor);
        }

        if (!gameOver) {
            settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 1);
            setcolor(scoreColor);
            std::ostringstream oss;
            oss << "Score: " << score;
            outtextxy(SCREEN_WIDTH - 150, 10, const_cast<char*>(oss.str().c_str()));
        } else {
            setcolor(RED);
            settextstyle(DEFAULT_FONT, HORIZ_DIR, 4);
            const char* gameOverText = "GAME OVER";
           int gameOverTextWidth = textwidth(const_cast<char*>(gameOverText));
            outtextxy((SCREEN_WIDTH - gameOverTextWidth) / 2, SCREEN_HEIGHT / 2 - 60, const_cast<char*>(gameOverText));

            setcolor(groundColor);
            if(!currentlyIsDay) setcolor(WHITE);
            settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 2);
            std::ostringstream final_score_oss;
            final_score_oss << "Final Score: " << score;
            const char* finalScoreText = final_score_oss.str().c_str();
            int finalScoreTextWidth = textwidth(const_cast<char*>(finalScoreText));
            outtextxy((SCREEN_WIDTH - finalScoreTextWidth) / 2, SCREEN_HEIGHT / 2 - 10, const_cast<char*>(finalScoreText));

            setcolor(DARKGRAY);
            if(!currentlyIsDay) setcolor(LIGHTGRAY);
            settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 1);
            const char* restartText = "Press R to restart";
            int restartTextWidth = textwidth(const_cast<char*>(restartText));
            outtextxy((SCREEN_WIDTH - restartTextWidth) / 2, SCREEN_HEIGHT / 2 + 30, const_cast<char*>(restartText));
        }

        // ----- K?T THÚC V? LÊN TRANG ?N -----

        // Hi?n th? trang dã v? (trang active) lên màn hình
        setvisualpage(page);

        // Chuy?n sang trang làm vi?c khác cho khung hình ti?p theo
        page = 1 - page;

        delay(1000 / 60); // Aim for 60 FPS
    }

    closegraph();
    return 0;
}
