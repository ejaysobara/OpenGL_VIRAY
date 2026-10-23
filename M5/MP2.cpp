#include <iostream>
#include <thread>
#include <mutex>
#include <cstdlib>
#include <GL/glew.h>
#include <GL/glut.h>

using namespace std;

// ---- layout data ----------------------------------------------------------
struct Rect { float x0, y0, x1, y1; };

const Rect BOUNDARY = { -0.90f, -0.90f, 0.90f, 0.90f };

// Each layout: path, playground, garden, seating
const Rect LAYOUT[2][4] = {
    { // Layout A: path runs left-right
        { -0.90f, -0.10f,  0.90f,  0.10f },
        { -0.80f,  0.25f, -0.20f,  0.80f },
        {  0.20f,  0.25f,  0.80f,  0.80f },
        { -0.30f, -0.75f,  0.30f, -0.30f } },
    { // Layout B: path runs top-bottom
        { -0.10f, -0.90f,  0.10f,  0.90f },
        { -0.80f,  0.20f, -0.25f,  0.80f },
        { -0.80f, -0.80f, -0.25f, -0.20f },
        {  0.25f, -0.30f,  0.80f,  0.30f } }
};

const char* NAMES[4]  = { "Walking path", "Playground", "Garden", "Seating" };
const char* COLORS[4] = { "sand", "orange", "purple", "blue" };
const GLfloat RGB[4][3] = {
    { 0.80f, 0.70f, 0.48f },
    { 0.95f, 0.45f, 0.15f },
    { 0.70f, 0.40f, 0.90f },
    { 0.25f, 0.60f, 0.95f }
};

// ---- buffers --------------------------------------------------------------
// Rect 0 = boundary, 1-4 = layout A, 5-8 = layout B.
// Every rect owns 4 unique vertices shared by its 2 triangles (6 indices).
const int RECTS = 9;
GLfloat  verts[RECTS * 4 * 3];
GLushort idx[RECTS * 6];
GLuint   vbo, ebo;

void addRect(int r, const Rect& q) {
    float c[4][2] = { {q.x0, q.y0}, {q.x1, q.y0}, {q.x1, q.y1}, {q.x0, q.y1} };
    for (int i = 0; i < 4; i++) {
        verts[(r*4 + i)*3 + 0] = c[i][0];
        verts[(r*4 + i)*3 + 1] = c[i][1];
        verts[(r*4 + i)*3 + 2] = 0.0f;
    }
    GLushort b = (GLushort)(r * 4);
    GLushort t[6] = { b, GLushort(b+1), GLushort(b+2), b, GLushort(b+2), GLushort(b+3) };
    for (int i = 0; i < 6; i++) idx[r*6 + i] = t[i];
}

void initBuffers() {
    addRect(0, BOUNDARY);
    for (int l = 0; l < 2; l++)
        for (int f = 0; f < 4; f++) addRect(1 + l*4 + f, LAYOUT[l][f]);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(idx), idx, GL_STATIC_DRAW);

    glClearColor(0.07f, 0.09f, 0.08f, 1.0f);
}

void cleanup() {
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
}

// ---- state (main thread only, except g_pending) ---------------------------
bool show[3]  = { true, true, true };   // playground, garden, seating
int  g_layout = 0;                      // 0 = A, 1 = B

mutex g_mutex;
int   g_pending = -1;                   // menu choice waiting for the main thread

void printLegend() {
    cout << "  Legend (Layout " << (g_layout == 0 ? 'A' : 'B') << "):\n"
         << "    Park boundary: green lawn\n"
         << "    " << NAMES[0] << ": " << COLORS[0] << " (always shown)\n";
    for (int f = 1; f < 4; f++)
        cout << "    " << NAMES[f] << ": " << COLORS[f]
             << (show[f-1] ? " [shown]\n" : " [hidden]\n");
}

// ---- display --------------------------------------------------------------
void drawRect(int r) {
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT,
                   (const void*)(r * 6 * sizeof(GLushort)));
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glEnableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glVertexPointer(3, GL_FLOAT, 0, 0);

    glColor3f(0.16f, 0.38f, 0.20f);
    drawRect(0);

    for (int f = 0; f < 4; f++) {
        if (f > 0 && !show[f-1]) continue;
        glColor3fv(RGB[f]);
        drawRect(1 + g_layout*4 + f);
    }

    glDisableClientState(GL_VERTEX_ARRAY);
    glFlush();
}

// ---- apply a validated choice (main thread only) --------------------------
void processChoice(int choice) {
    switch (choice) {
        case 1: show[0] = show[1] = show[2] = true; cout << "  -> Showing all features.\n"; break;
        case 2: case 3: case 4: {
            bool& s = show[choice - 2];
            s = !s;
            cout << "  -> " << NAMES[choice - 1] << (s ? " shown.\n" : " hidden.\n");
            break;
        }
        case 5: g_layout = 0; cout << "  -> Layout A.\n"; break;
        case 6: g_layout = 1; cout << "  -> Layout B.\n"; break;
        case 0: cleanup(); cout << "Goodbye.\n"; exit(0);
    }
    printLegend();
    glutPostRedisplay();
}

void keyboard(unsigned char key, int, int) {
    if (key >= '0' && key <= '6') processChoice(key - '0');
}

// Polls for choices typed in the console; GL calls must stay on this thread.
void pollTimer(int) {
    int c;
    { lock_guard<mutex> lk(g_mutex); c = g_pending; g_pending = -1; }
    if (c >= 0) processChoice(c);
    glutTimerFunc(30, pollTimer, 0);
}

// ---- console menu (background thread) -------------------------------------
void menuThread() {
    while (true) {
        cout << "\n=== Community Park Layout Planner ===\n"
             << "  1. Show All\n"
             << "  2. Toggle Playground\n"
             << "  3. Toggle Garden\n"
             << "  4. Toggle Seating\n"
             << "  5. Layout A\n"
             << "  6. Layout B\n"
             << "  0. Exit\n"
             << "Selection: ";

        int choice;
        if (!(cin >> choice)) {
            if (cin.eof()) return;
            cin.clear(); cin.ignore(1000, '\n');
            cout << "  [!] Enter a number 0-6.\n";
            continue;
        }
        cin.ignore(1000, '\n');
        if (choice < 0 || choice > 6) {
            cout << "  [!] Invalid selection (0-6 only).\n";
            continue;
        }
        { lock_guard<mutex> lk(g_mutex); g_pending = choice; }
    }
}

// ---- main -----------------------------------------------------------------
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Community Park Layout Planner");

    GLenum err = glewInit();
    if (err != GLEW_OK) {
        cerr << "GLEW error: " << glewGetErrorString(err) << "\n";
        return 1;
    }

    initBuffers();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(30, pollTimer, 0);

    cout << "Window keys: 0-6 work the same as the menu.\n";
    printLegend();

    thread t(menuThread);
    t.detach();

    glutMainLoop();
    return 0;
}
