#define GL_SILENCE_DEPRECATION

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#include <GL/freeglut_ext.h>
#endif

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>

using namespace std;

// ============================================================
// 1. GLOBAL GAME STATE
// ============================================================

// Player spaceship position
float playerX = 0.0f;
float playerY = -0.75f;

// Movement amount per key press
const float playerSpeed = 0.08f;

// Game information
int score = 0;
int lives = 3;
bool gameOver = false;

// Asteroid positions
float asteroid1X = -0.55f;
float asteroid1Y = 0.90f;

float asteroid2X = 0.55f;
float asteroid2Y = 0.60f;

// Asteroid falling speeds
float asteroid1Speed = 0.015f;
float asteroid2Speed = 0.025f;


// ============================================================
// 2. BITMAP TEXT HELPER
// ============================================================

void drawBitmapString(void* font, const char* text) {
    for (const char* c = text; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}


// ============================================================
// 3. DRAW THE PLAYER SPACESHIP
// ============================================================

void drawSpaceship() {
    // Ship body
    glColor3f(0.1f, 0.5f, 1.0f);

    glBegin(GL_TRIANGLES);
        glVertex2f(playerX, playerY + 0.15f);
        glVertex2f(playerX - 0.14f, playerY - 0.12f);
        glVertex2f(playerX + 0.14f, playerY - 0.12f);
    glEnd();

    // Ship wings
    glColor3f(0.2f, 0.8f, 1.0f);

    glBegin(GL_QUADS);
        glVertex2f(playerX - 0.14f, playerY - 0.02f);
        glVertex2f(playerX - 0.28f, playerY - 0.12f);
        glVertex2f(playerX - 0.22f, playerY - 0.20f);
        glVertex2f(playerX - 0.08f, playerY - 0.10f);
    glEnd();

    glBegin(GL_QUADS);
        glVertex2f(playerX + 0.14f, playerY - 0.02f);
        glVertex2f(playerX + 0.28f, playerY - 0.12f);
        glVertex2f(playerX + 0.22f, playerY - 0.20f);
        glVertex2f(playerX + 0.08f, playerY - 0.10f);
    glEnd();

    // Ship window
    glColor3f(1.0f, 1.0f, 0.3f);

    glBegin(GL_POLYGON);
        glVertex2f(playerX, playerY + 0.07f);
        glVertex2f(playerX - 0.05f, playerY);
        glVertex2f(playerX, playerY - 0.05f);
        glVertex2f(playerX + 0.05f, playerY);
    glEnd();

    // Engine flame
    glColor3f(1.0f, 0.3f, 0.0f);

    glBegin(GL_TRIANGLES);
        glVertex2f(playerX - 0.06f, playerY - 0.13f);
        glVertex2f(playerX + 0.06f, playerY - 0.13f);
        glVertex2f(playerX, playerY - 0.25f);
    glEnd();
}


// ============================================================
// 4. DRAW AN ASTEROID
// ============================================================

void drawAsteroid(float x, float y, float size) {
    glColor3f(0.55f, 0.55f, 0.6f);

    glBegin(GL_POLYGON);
        glVertex2f(x - size, y);
        glVertex2f(x - size * 0.6f, y + size);
        glVertex2f(x + size * 0.5f, y + size * 0.8f);
        glVertex2f(x + size, y + size * 0.1f);
        glVertex2f(x + size * 0.7f, y - size * 0.8f);
        glVertex2f(x - size * 0.4f, y - size);
    glEnd();

    // Small darker crater
    glColor3f(0.25f, 0.25f, 0.3f);

    glBegin(GL_POLYGON);
        glVertex2f(x - size * 0.25f, y + size * 0.2f);
        glVertex2f(x, y + size * 0.35f);
        glVertex2f(x + size * 0.25f, y + size * 0.1f);
        glVertex2f(x + size * 0.1f, y - size * 0.15f);
        glVertex2f(x - size * 0.2f, y - size * 0.1f);
    glEnd();
}


// ============================================================
// 5. DRAW THE HUD
// ============================================================

void drawHUD() {
    char buffer[100];

    // Score display
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(-0.92f, 0.90f);

    snprintf(buffer, sizeof(buffer), "Score: %d", score);
    drawBitmapString(GLUT_BITMAP_HELVETICA_18, buffer);

    // Lives display
    glRasterPos2f(0.55f, 0.90f);

    snprintf(buffer, sizeof(buffer), "Lives: %d", lives);
    drawBitmapString(GLUT_BITMAP_HELVETICA_18, buffer);

    // Control instructions
    glColor3f(0.7f, 0.8f, 1.0f);
    glRasterPos2f(-0.92f, -0.92f);

    drawBitmapString(
        GLUT_BITMAP_HELVETICA_12,
        "WASD: Move     R: Restart     ESC: Exit"
    );
}


// ============================================================
// 6. COLLISION DETECTION
// ============================================================

bool checkCollision(float asteroidX, float asteroidY) {
    float horizontalDistance = fabs(playerX - asteroidX);
    float verticalDistance = fabs(playerY - asteroidY);

    // These values act as approximate collision boundaries
    return horizontalDistance < 0.22f &&
           verticalDistance < 0.25f;
}


// ============================================================
// 7. RESET AN ASTEROID
// ============================================================

void resetAsteroid(int asteroidNumber) {
    if (asteroidNumber == 1) {
        asteroid1X = -0.75f + static_cast<float>(rand() % 150) / 100.0f;
        asteroid1Y = 1.10f;
    }
    else {
        asteroid2X = -0.75f + static_cast<float>(rand() % 150) / 100.0f;
        asteroid2Y = 1.10f;
    }
}


// ============================================================
// 8. HANDLE ASTEROID COLLISION
// ============================================================

void handleCollision(int asteroidNumber) {
    lives--;

    cout << "Collision detected! Lives remaining: "
         << lives << endl;

    resetAsteroid(asteroidNumber);

    if (lives <= 0) {
        lives = 0;
        gameOver = true;

        cout << "GAME OVER" << endl;
    }
}


// ============================================================
// 9. TIMER ANIMATION
// ============================================================

void updateGame(int value) {
    // Stop updating the game after game over
    if (!gameOver) {
        // Move asteroids downward
        asteroid1Y -= asteroid1Speed;
        asteroid2Y -= asteroid2Speed;

        // If asteroid 1 reaches the bottom, reset it and increase score
        if (asteroid1Y < -1.1f) {
            resetAsteroid(1);
            score++;
        }

        // If asteroid 2 reaches the bottom, reset it and increase score
        if (asteroid2Y < -1.1f) {
            resetAsteroid(2);
            score++;
        }

        // Check asteroid collisions
        if (checkCollision(asteroid1X, asteroid1Y)) {
            handleCollision(1);
        }

        if (checkCollision(asteroid2X, asteroid2Y)) {
            handleCollision(2);
        }
    }

    // Request a redraw
    glutPostRedisplay();

    // Schedule the timer again
    glutTimerFunc(16, updateGame, 0);
}


// ============================================================
// 10. DISPLAY CALLBACK
// ============================================================

void display() {
    glClearColor(0.02f, 0.02f, 0.08f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    if (!gameOver) {
        // Draw the player
        drawSpaceship();

        // Draw the asteroids
        drawAsteroid(asteroid1X, asteroid1Y, 0.10f);
        drawAsteroid(asteroid2X, asteroid2Y, 0.10f);

        // Draw score, lives, and instructions
        drawHUD();
    }
    else {
        // Game-over display
        glColor3f(1.0f, 0.2f, 0.2f);
        glRasterPos2f(-0.35f, 0.20f);

        drawBitmapString(
            GLUT_BITMAP_HELVETICA_18,
            "GAME OVER"
        );

        glColor3f(1.0f, 1.0f, 1.0f);
        glRasterPos2f(-0.48f, -0.05f);

        drawBitmapString(
            GLUT_BITMAP_HELVETICA_18,
            "Press R to Restart"
        );

        glRasterPos2f(-0.35f, -0.30f);

        char buffer[50];
        snprintf(buffer, sizeof(buffer), "Final Score: %d", score);

        drawBitmapString(
            GLUT_BITMAP_HELVETICA_18,
            buffer
        );
    }

    glFlush();
}


// ============================================================
// 11. KEYBOARD CALLBACK
// ============================================================

void keyboard(unsigned char key, int x, int y) {
    if (key == 27) {
        exit(0);
    }

    // Restart the game
    if (key == 'r' || key == 'R') {
        playerX = 0.0f;
        playerY = -0.75f;

        score = 0;
        lives = 3;
        gameOver = false;

        asteroid1X = -0.55f;
        asteroid1Y = 0.90f;

        asteroid2X = 0.55f;
        asteroid2Y = 0.60f;

        cout << "Game restarted." << endl;

        glutPostRedisplay();
        return;
    }

    // Do not allow movement after game over
    if (gameOver) {
        return;
    }

    // Move the spaceship
    if (key == 'a' || key == 'A') {
        playerX -= playerSpeed;
    }
    else if (key == 'd' || key == 'D') {
        playerX += playerSpeed;
    }
    else if (key == 'w' || key == 'W') {
        playerY += playerSpeed;
    }
    else if (key == 's' || key == 'S') {
        playerY -= playerSpeed;
    }

    // Keep the spaceship inside the screen
    if (playerX < -0.75f) {
        playerX = -0.75f;
    }

    if (playerX > 0.75f) {
        playerX = 0.75f;
    }

    if (playerY < -0.75f) {
        playerY = -0.75f;
    }

    if (playerY > 0.75f) {
        playerY = 0.75f;
    }

    glutPostRedisplay();
}


// ============================================================
// 12. PROGRAM ENTRY POINT
// ============================================================

int main(int argc, char** argv) {
    glutInit(&argc, argv);

    glutInitWindowSize(800, 700);
    glutInitWindowPosition(200, 100);

    glutCreateWindow("MP01 - The Cosmic Dodger");

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);

    // Start the timer
    glutTimerFunc(16, updateGame, 0);

    glutMainLoop();

    return 0;
}