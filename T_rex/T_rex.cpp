#include <sstream>
#include <graphics.h>
#include <iostream>
#include <string>
#include <windows.h> // Thêm thu vi?n Windows

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 400
#define GROUND_Y 300
#define DINO_WIDTH 40
#define DINO_HEIGHT 40
#define CACTUS_WIDTH 20
#define CACTUS_HEIGHT 40
#define JUMP_SPEED -22
#define GRAVITY 3

bool checkCollision(int x1, int y1, int w1, int h1,
                    int x2, int y2, int w2, int h2) {
    return !(x1 + w1 < x2 || x2 + w2 < x1 ||
             y1 + h1 < y2 || y2 + h2 < y1);
}

int main() {
    initwindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Dino Game - Part 5", 0, 0, false, true);

    int dinoX = 50;
    int dinoY = GROUND_Y - DINO_HEIGHT;
    int velocityY = 0;
    bool isJumping = false;

    int cactusX = SCREEN_WIDTH;
    int cactusY = GROUND_Y - CACTUS_HEIGHT;

    bool gameOver = false;

    int score = 0;
    int cactusSpeed = 10;

    // Double buffering: T?o hai trang d? v?
    int currentPage = 0;
    int totalPages = 2;
    setactivepage(currentPage);
    setvisualpage(1 - currentPage);  // Trang hi?n th?

    while (true) {
        cleardevice(); // Xóa màn hình c?a trang dang s? d?ng

        // V? m?t d?t
        setcolor(GREEN);
        line(0, GROUND_Y, SCREEN_WIDTH, GROUND_Y);

        // X? lý phím b?ng GetAsyncKeyState
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) break; // Nh?n ESC d? thoát
        if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
            std::cout << "Space pressed!" << std::endl;
            if (!isJumping && !gameOver) {
                velocityY = JUMP_SPEED;
                isJumping = true;
                std::cout << "Jump triggered! velocityY: " << velocityY << std::endl;
            }
        }
        // Thêm phím 'R' d? kh?i d?ng l?i trò choi
        if (GetAsyncKeyState('R') & 0x8000 || GetAsyncKeyState('r') & 0x8000) {
            if (gameOver) {
                gameOver = false;
                dinoY = GROUND_Y - DINO_HEIGHT;
                velocityY = 0;
                isJumping = false;
                cactusX = SCREEN_WIDTH;
                score = 0;
                cactusSpeed = 10;
                std::cout << "Game restarted!" << std::endl;
            }
        }

        // X? lý logic nh?y
        if (!gameOver) {
            dinoY += velocityY;
            velocityY += GRAVITY;
            std::cout << "dinoY: " << dinoY << ", velocityY: " << velocityY << std::endl;

            if (dinoY >= GROUND_Y - DINO_HEIGHT) {
                dinoY = GROUND_Y - DINO_HEIGHT;
                velocityY = 0;
                isJumping = false;
            }

            cactusX -= cactusSpeed;
            if (cactusX < -CACTUS_WIDTH) {
                cactusX = SCREEN_WIDTH + rand() % 200;
                score += 10;
                if (score % 50 == 0 && cactusSpeed < 30) cactusSpeed++;
            }

            if (checkCollision(dinoX, dinoY, DINO_WIDTH, DINO_HEIGHT,
                               cactusX, cactusY, CACTUS_WIDTH, CACTUS_HEIGHT)) {
                gameOver = true;
            }
        }

        // V? kh?ng long
        setfillstyle(SOLID_FILL, DARKGRAY);
        bar(dinoX, dinoY, dinoX + DINO_WIDTH, dinoY + DINO_HEIGHT);

        // V? xuong r?ng
        setfillstyle(SOLID_FILL, GREEN);
        bar(cactusX, cactusY, cactusX + CACTUS_WIDTH, cactusY + CACTUS_HEIGHT);

        // Hi?n th? di?m s?
        settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 1);
        setcolor(WHITE);
        std::ostringstream oss;
        oss << "Score: " << score;
        std::string scoreStr = oss.str();
        outtextxy(SCREEN_WIDTH - 150, 10, const_cast<char*>(scoreStr.c_str()));

        // Hi?n th? "Game Over" và hu?ng d?n kh?i d?ng l?i
        if (gameOver) {
            settextstyle(BOLD_FONT, HORIZ_DIR, 3);
            setcolor(RED);
            outtextxy(SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2, "GAME OVER!");
            settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 1);
            setcolor(WHITE);
            outtextxy(SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 + 40, "Press R to Restart");
        }

        // Hoán d?i trang (buffer)
        currentPage = 1 - currentPage;
        setactivepage(currentPage);
        setvisualpage(1 - currentPage);

        delay(30);
    }

    closegraph();
    return 0;
}

