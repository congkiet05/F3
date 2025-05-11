#include <graphics.h>
#include <conio.h>
#include <windows.h>
#include <sstream>
#include <string>
#include <math.h>
#include <stdlib.h>
#include <time.h> // Needed for time() function

// Ð?nh nghia các màu chu?n BGI (n?u không ch?c ch?n graphics.h dã d?nh nghia h?t)
// Các màu c?n dùng: ORANGE (thu?ng là YELLOW 14), BROWN (6), DARKGRAY (8), WHITE (15), BLACK (0), BLUE (1), LIGHTCYAN (11), RED (4), GREEN (2)
#define BLACK         0
#define BLUE          1
#define GREEN         2
#define CYAN          3
#define RED           4
#define MAGENTA       5
#define BROWN         6 // Màu nâu
#define LIGHTGRAY     7
#define DARKGRAY      8 // Màu xám d?m
#define LIGHTBLUE     9
#define LIGHTGREEN   10
#define LIGHTCYAN    11 // B?u tr?i ngày
#define LIGHTRED     12
#define LIGHTMAGENTA 13
#define YELLOW       14 // Màu vàng/cam
#define WHITE        15

// Alias ORANGE v?i YELLOW (giá tr? màu cam/vàng trong BGI)
#define ORANGE       YELLOW


#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 400
#define GROUND_Y 300
#define CACTUS_WIDTH 20 // D?nh nghia g?c
#define CACTUS_HEIGHT 40 // D?nh nghia g?c
#define JUMP_SPEED -28 // T?c d? nh?y bình thu?ng
#define HIGH_JUMP_SPEED -35 // T?c d? nh?y cao hon khi g?p 2 cây sát nhau (có th? di?u ch?nh)

// Ngu?ng kho?ng cách gi?a tâm 2 cây xuong r?ng d? coi là "g?n nhau"
// D?a trên hình v?, tam cây chinh cach mép trái kho?ng -6, mép ph?i kho?ng +6.
// Nhánh trái cách tam -17 (v?i r?ng kho?ng 6px), nhánh ph?i +17.
// U?c tính r?ng hi?u qu? c?a 1 cây kho?ng 40px (t? -20 d?n +20 so v?i x trung tâm).
// Kho?ng tr?ng gi?a 2 cây là Distance - 40. Mu?n kho?ng tr?ng <= 30px, t?c là Distance <= 70px.
#define CLOSE_CACTI_THRESHOLD 70

#define GRAVITY 3

bool checkCollision(int x1, int y1, int w1, int h1,
                    int x2, int y2, int w2, int h2) {
    return !(x1 + w1 < x2 || x2 + w2 < x1 ||
             y1 + h1 < y2 || y2 + h2 < y1);
}

// Function to draw simple birds (arcs) - Màu v?n ph? thu?c vào th?i gian ngày/dêm
void drawBird(int x, int y, int color) {
    setcolor(color);
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


void drawBirdHead(int x, int y) { // x, y là v? trí g?c c?a drawBirdCartoon

    // --- V? và tô màu Ð?U (cam) ---
    setcolor(ORANGE); // Ð?u màu cam
    setfillstyle(SOLID_FILL, ORANGE); // Tô màu cam
    // Ð?u du?c v? tâm t?i (x + 18, y - 3) so v?i v? trí g?c
    circle(x + 18, y - 3, 12);
    floodfill(x + 18, y - 3, ORANGE); // Tô màu cam

    // --- V? và tô màu M? (nâu) - Ðã làm nh? l?i ---
    setcolor(BROWN); // M? màu nâu
    setfillstyle(SOLID_FILL, BROWN);
    // T?a d? hình tam giác v? m?, relative d?n v? trí g?c (x, y) c?a drawBirdCartoon
    // Ði?u ch?nh t?a d? d? m? nh? hon và n?m du?i 2 m?t
    int beak_points[6] = {
        x + 18 - 3, y - 0,  // Ði?m góc trên bên trái (gi?m chi?u r?ng)
        x + 18 + 3, y - 0,  // Ði?m góc trên bên ph?i (gi?m chi?u r?ng)
        x + 18,     y + 4   // Ði?m nh?n phía du?i (gi?m chi?u cao)
    };
    fillpoly(3, beak_points); // V? và tô màu tam giác

    // --- V? và tô màu M?T (den) --- // ÐÃ THAY Ð?I MÀU T?I ÐÂY
    setcolor(BLACK); // M?t màu DEN
    setfillstyle(SOLID_FILL, BLACK); // Tô màu DEN
    // V? trí m?t relative d?n tâm d?u (x+18, y-3)
    circle(x + 18 - 5, y - 3 - 3, 2); // M?t trái
    floodfill(x + 18 - 5, y - 3 - 3, BLACK); // Tô màu DEN

    circle(x + 18 + 5, y - 3 - 3, 2); // M?t ph?i
    floodfill(x + 18 + 5, y - 3 - 3, BLACK); // Tô màu DEN
}

// Hàm v? thân
void drawBirdBody(int x, int y) { // x, y là v? trí g?c
    setcolor(ORANGE); // Thân màu cam
    setfillstyle(SOLID_FILL, ORANGE); // Tô màu cam
    // Thân du?c v? tâm t?i (x + 18, y + 12) so v?i v? trí g?c
    ellipse(x + 18, y + 12, 0, 360, 12, 15);
    floodfill(x + 18, y + 12, ORANGE); // Tô màu cam
}

// Hàm v? chân
void drawBirdLeg(int x, int y) { // x, y là v? trí g?c
    setcolor(BROWN); // Chân màu nâu
    // Chân du?c v? t? v? trí kho?ng (x + 15, y + 24) so v?i v? trí g?c
    line(x + 15, y + 24, x + 15, y + 36);
    line(x + 15 + 6, y + 24, x + 15 + 6, y + 36);
}

// Hàm v? cánh trái
void drawLeftWing(int x, int y) { // x, y là v? trí g?c
    setcolor(ORANGE); // Cánh trái màu cam
    setfillstyle(SOLID_FILL, ORANGE); // Tô màu cam
    // T?a d? hình tam giác v? cánh trái, relative d?n v? trí g?c
    int wing[6] = {x + 10, y + 12, // Ði?m neo cánh
                   x + 10 - 18, y + 12 - 12, // Ði?m trên xa
                   x + 10 - 6, y + 12 + 6};  // Ði?m du?i g?n
    fillpoly(3, wing); // V? và tô màu tam giác
}

// Hàm v? cánh ph?i
void drawRightWing(int x, int y) { // x, y là v? trí g?c
    setcolor(ORANGE); // Cánh ph?i màu cam
    setfillstyle(SOLID_FILL, ORANGE); // Tô màu cam
    // T?a d? hình tam giác v? cánh ph?i, relative d?n v? trí g?c
    int wing[6] = {x + 24, y + 12, // Ði?m neo cánh
                   x + 24 + 18, y + 12 - 12, // Ði?m trên xa
                   x + 24 + 6, y + 12 + 6};  // Ði?m du?i g?n
    fillpoly(3, wing); // V? và tô màu tam giác
}

// Hàm chính v? toàn b? con chim ho?t hình
void drawBirdCartoon(int x, int y) { // x, y là v? trí g?c c?a con chim trong game (dinoX + 20, dinoY + 25)
    // G?i các hàm v? b? ph?n, truy?n cùng v? trí g?c x, y
    drawBirdHead(x, y); // drawBirdHead gi? dã bao g?m v? m?
    drawBirdBody(x, y);
    drawBirdLeg(x, y);
    drawLeftWing(x, y);
    drawRightWing(x, y);
}
// --------------------------------------------------------------------------

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

void drawCactus(int x, int y, int color) {
    setcolor(color);
    setfillstyle(SOLID_FILL, color);

    // Redraw cactus using Bresenham lines with specified color
    // Gi? s? x, y là tam c?a thân cây chính
    bresenhamLine(x - 6, y, x + 6, y, color);
    bresenhamLine(x - 6, y, x - 6, y + 60, color);
    bresenhamLine(x + 6, y, x + 6, y + 60, color);
    bresenhamLine(x - 6, y + 60, x + 6, y + 60, color);

    // V? các nhánh
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


    // Floodfill using the specified color and a pixel of that color within the shape
    // Ði?m floodfill c?n n?m trong vùng c?n tô màu
    floodfill(x, y + 30, color); // Tâm thân cây chính
    floodfill(x - 17, y + 30, color); // Nhánh trái trên
    floodfill(x + 17, y + 30, color); // Nhánh ph?i trên
    floodfill(x - 11, y + 40, color); // Nhánh trái du?i
    floodfill(x + 11, y + 40, color); // Nhánh ph?i du?i
}


int main() {
    initwindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Dinosaur Game", 0, 0, false, true);
    srand(time(0)); // Seed random number generator

    int dinoX = 50;
    int dinoY = GROUND_Y - 60; // Dino base position relative to ground
    int velocityY = 0;
    bool isJumping = false;

    // V? trí X c?a 2 cây xuong r?ng (luôn là tam c?a thân chính)
    int cactusX[2] = {SCREEN_WIDTH, SCREEN_WIDTH + 200}; // Kh?i t?o cách xa nhau
    // V? trí Y c?a 2 cây xuong r?ng (luôn là d?nh c?a thân chính)
    int cactusY[2] = {GROUND_Y - 60, GROUND_Y - 60}; // V? trí ban d?u (base) - gi? nguyên nhu code cu
    int bird2X = 420, bird2Y = 30; // Simple birds
    int bird3X = 470, bird3Y = 40;
    bool hasTwoCacti = false;

    int score = 0;
    int cactusSpeed = 10;
    bool gameOver = false;
    int currentPage = 0;

    int cloudX[3] = {200, 500, 750};
    int cloudY[3] = {50, 70, 90};

    time_t startTime = time(0); // Record start time

    while (true) {
        setactivepage(currentPage);
        setvisualpage(1 - currentPage);

        // --- Time and Background Logic ---
        time_t currentTime = time(0);
        int elapsedTime = currentTime - startTime;

        // Tính toán tr?ng thái ngày/dêm luân phiên m?i 20 giây
        // Chu k?: 20s ngày, 20s dêm
        bool currentlyIsDay = ((elapsedTime / 10) % 2 == 0); // <--- CH?NH S? ? ÐÂY (20s)

        // Set background color and ground line color based on the current time state
        int bgColor, groundColor, skyColor, simpleBirdColor, scoreColor, cactusColor;
        if (currentlyIsDay) { // Tr?ng thái NGÀY
            bgColor = WHITE;
            groundColor = BLACK;
            skyColor = LIGHTCYAN; // B?u tr?i sáng
            simpleBirdColor = BLACK; // Chim màu den
            scoreColor = RED; // Ði?m màu d?
            cactusColor = GREEN; // Xuong r?ng màu xanh
        } else { // Tr?ng thái ÐÊM
            bgColor = LIGHTGRAY; // N?n den
            groundColor = DARKGRAY; // Ðu?ng d?t màu xám d?m
            skyColor = BLACK; // B?u tr?i màu xanh d?m (dêm) - S? d?ng BLUE chu?n BGI
            simpleBirdColor = WHITE; // Chim don gi?n màu tr?ng cho d? th?y
            scoreColor = WHITE; // Ði?m màu tr?ng cho d? th?y
            cactusColor = GREEN; // Xuong r?ng v?n xanh
        }

        setbkcolor(bgColor);
        cleardevice(); // Clear with the chosen background color

        // Draw the distinct sky bar
        setfillstyle(SOLID_FILL, skyColor);
        bar(0, 0, getmaxx(), 120);

        // Draw ground line with the chosen color
        setcolor(groundColor);
        line(0, GROUND_Y, SCREEN_WIDTH, GROUND_Y);

        // --- Drawing movable objects ---
        // Clouds (keep white) - B?n có th? mu?n làm m? ho?c ?n mây vào ban dêm
        for (int i = 0; i < 3; i++) {
            drawCloud(cloudX[i], cloudY[i]);
            cloudX[i] -= 2;
            if (cloudX[i] < -50) {
                cloudX[i] = SCREEN_WIDTH + rand() % 100;
                cloudY[i] = 30 + rand() % 60;
            }
        }

        // Simple Birds (adjust color based on time)
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
        drawBird(bird2X, bird2Y, simpleBirdColor);
        drawBird(bird3X, bird3Y, simpleBirdColor);


        // --- Input Handling ---
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) break;
        // X? lý nh?y - Thêm logic nh?y cao khi 2 cây g?n nhau
        if ((GetAsyncKeyState(VK_SPACE) & 0x8000) && !isJumping && !gameOver) {
            Beep(800, 100); // Beep on jump

            bool triggerHighJump = false;
            // Ki?m tra xem có 2 cây xuong r?ng ang hi?n th? và chúng có g?n nhau không
            // hasTwoCacti ch? true khi di?m >= 50
            if (hasTwoCacti) {
                // Tính kho?ng cách gi?a tam 2 cây xuong r?ng d?a trên v? trí X c?a chúng
                // Luu ý: cactusX[0] và cactusX[1] là t?a d? X c?a tam thân cây chính
                int distanceBetweenCactiCenters = abs(cactusX[0] - cactusX[1]);

                // N?u kho?ng cách gi?a tâm 2 cây nh? hon ho?c b?ng ngu?ng
                if (distanceBetweenCactiCenters <= CLOSE_CACTI_THRESHOLD) {
                    triggerHighJump = true;
                }
            }

            // Thi?t l?p t?c d? nh?y d?a vào vi?c có c?n nh?y cao hay không
            if (triggerHighJump) {
                velocityY = HIGH_JUMP_SPEED; // Nh?y cao
            } else {
                velocityY = JUMP_SPEED; // Nh?y bình thu?ng
            }
            isJumping = true; // B?t d?u nh?y
        }

        // X? lý khoi d?ng l?i
        if ((GetAsyncKeyState('R') & 0x8000) && gameOver) { // Restart game
            dinoY = GROUND_Y - 60;
            velocityY = 0;
            isJumping = false;
            // Reset v? trí xuong r?ng v? tr?ng thái ban d?u
            cactusX[0] = SCREEN_WIDTH;
            cactusX[1] = SCREEN_WIDTH + 200; // Reset v? trí ban d?u (xa nhau)
            cactusY[0] = GROUND_Y - 60; // Reset Y ban d?u
            cactusY[1] = GROUND_Y - 60; // Reset Y ban d?u
            score = 0;
            cactusSpeed = 10;
            hasTwoCacti = false;
            gameOver = false;
            // Reset time on restart
            startTime = time(0);
        }

        // --- Game Logic ---
        if (!gameOver) {
            // Update Dinosaur position
            dinoY += velocityY;
            velocityY += GRAVITY;
            if (dinoY >= GROUND_Y - 60) {
                dinoY = GROUND_Y - 60;
                velocityY = 0;
                isJumping = false;
            }

            // Update Cactus positions
            // Chúng ta ph?i c?p nh?t c? 2 cây n?u hasTwoCacti là true
            int numCactiToUpdate = hasTwoCacti ? 2 : 1;
            for (int i = 0; i < numCactiToUpdate; i++) {
                cactusX[i] -= cactusSpeed;
                // Ki?m tra và reset cactus n?u di chuy?n ra ngoài màn hình bên trái
                if (cactusX[i] < -40) { // Kho?ng -40 là r?i hoàn toàn ra ngoài (ví d? r?ng 40)
                    // Reset cactus position
                    cactusX[i] = SCREEN_WIDTH + rand() % 200; // V? trí xu?t hi?n ng?u nhiên sau màn hình
                    cactusY[i] = GROUND_Y - 60; // Reset Y ban d?u
                    score += 10; // Tang di?m khi vu?t qua 1 cây

                    // Sau khi vu?t qua cây d?u tiên và di?m >= 50, b?t d?u xu?t hi?n cây th? 2
                    if (score >= 50 && !hasTwoCacti) {
                         hasTwoCacti = true;
                         // Khi b?t d?u có cây th? 2, nên dam b?o nó xu?t hi?n cách cây d?u tiên m?t kho?ng h?p lý
                         // (ho?c d?n gi? nó xu?t hi?n ng?u nhiên nhu code hi?n t?i).
                         // Logic hi?n t?i reset c? 2 d?n SCREEN_WIDTH + rand()%200, v?n d?m b?o kho?ng cách ng?u nhiên.
                    }

                    // Tang t?c d? khi dat moc di?m
                    if (score % 50 == 0 && cactusSpeed < 30) {
                        cactusSpeed++;
                    }
                }
            }

            // Check for collision
            // Ki?m tra va ch?m v?i t?t c? các cây xuong r?ng ang hi?n th?
             int dino_collision_x = dinoX + 10; // V? trí x kho?ng mép trái thân chim
             int dino_collision_y = dinoY + 25; // V? trí y kho?ng d?nh d?u chim
             int dino_collision_w = 40; // Chi?u r?ng va ch?m (u?c tính)
             int dino_collision_h = 60; // Chi?u cao va ch?m (u?c tính)

            int cactus_effective_w = 40; // Chi?u r?ng hi?u qu? c?a cây xuong r?ng cho va ch?m
            int cactus_effective_h = 60; // Chi?u cao hi?u qu? c?a cây xuong r?ng (t? y d?n y+60)
            int cactus_collision_offset_x = -20; // Offset t? cactusX (tam) d?n mép trái
            int cactus_collision_offset_y = 0; // Offset t? cactusY (d?nh) d?n d?nh vùng va ch?m


            for (int i = 0; i < numCactiToUpdate; i++) {
                 // Vùng va ch?m c?a cactus: t? (cactusX[i] - 20, cactusY[i]) r?ng 40, cao 60
                 if (checkCollision(dino_collision_x, dino_collision_y, dino_collision_w, dino_collision_h,
                                   cactusX[i] + cactus_collision_offset_x, cactusY[i] + cactus_collision_offset_y, cactus_effective_w, cactus_effective_h)) {
                     gameOver = true;
                     break; // Thoát vòng l?p ki?m tra va ch?m ngay khi phát hi?n va ch?m
                 }
            }
        }

        // --- Drawing Game Objects ---
        // Cartoon Bird (Dino) - drawn at dinoX + 20, dinoY + 25 relative to its top-left corner
        // The drawBirdCartoon function uses these coordinates as its base (x,y)
        drawBirdCartoon(dinoX + 20, dinoY+25);

        // Cacti (draw with chosen color - stays green)
        int numCactiToDraw = hasTwoCacti ? 2 : 1;
        for (int i = 0; i < numCactiToDraw; i++) {
            drawCactus(cactusX[i], cactusY[i], cactusColor); // cactusY là d?nh c?a thân chính
        }

        // --- Draw Score ---
        settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 1);
        setcolor(scoreColor); // Use chosen score color
        std::ostringstream oss;
        oss << "Score: " << score;
        outtextxy(SCREEN_WIDTH - 150, 10, const_cast<char*>(oss.str().c_str()));

        // --- Game Over Message ---
        if (gameOver) {
            setcolor(RED);
            outtextxy(SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2, "GAME OVER");
            outtextxy(SCREEN_WIDTH / 2 - 65, SCREEN_HEIGHT / 2 + 20, "Press R to restart");
        }

        // --- End of Frame ---
        currentPage = 1 - currentPage; // Switch pages
        delay(1000 / 60);  // Aim for 60 frames per second
    }

    closegraph();
    return 0;
}
