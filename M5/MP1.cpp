#include <iostream>
#include <thread>
#include <mutex>
#include <cmath>
#include <GL/glew.h>
#include <GL/glut.h>

using namespace std;

// ---------- state ----------------------------------------------------------
enum Signal { ALL_OFF = 0, RED, YELLOW, GREEN };
Signal g_signal       = ALL_OFF;
bool   g_redraw        = false;
bool   g_exitRequested = false;  // set by menu thread; acted on by main thread
mutex  g_mutex;

// ---------- geometry constants ---------------------------------------------
static const int   SEG = 24;
static const float PI  = 3.14159265f;
static const float CR  = 0.18f;

static const float CX    = 0.0f;
static const float CY[3] = { 0.55f, 0.0f, -0.55f }; // red, yellow, green

static GLfloat housingV[] = {
    -0.28f, -0.85f, 0.0f,
     0.28f, -0.85f, 0.0f,
     0.28f,  0.85f, 0.0f,
    -0.28f, -0.85f, 0.0f,
     0.28f,  0.85f, 0.0f,
    -0.28f,  0.85f, 0.0f
};

static GLfloat circleV[3][SEG * 3 * 3];

GLuint vboHousing;
GLuint vboCircle[3];

// ---------- build circle geometry -----------------------------------------
static void buildCircle(int idx) {
    float cx = CX, cy = CY[idx];
    for (int i = 0; i < SEG; i++) {
        float a1 = 2*PI*i/SEG, a2 = 2*PI*(i+1)/SEG;
        int b = i * 9;
        circleV[idx][b+0]=cx;             circleV[idx][b+1]=cy;             circleV[idx][b+2]=0;
        circleV[idx][b+3]=cx+CR*cosf(a1); circleV[idx][b+4]=cy+CR*sinf(a1); circleV[idx][b+5]=0;
        circleV[idx][b+6]=cx+CR*cosf(a2); circleV[idx][b+7]=cy+CR*sinf(a2); circleV[idx][b+8]=0;
    }
}

// ---------- upload geometry to GPU once -----------------------------------
static void initBuffers() {
    for (int i = 0; i < 3; i++) buildCircle(i);

    glGenBuffers(1, &vboHousing);
    glBindBuffer(GL_ARRAY_BUFFER, vboHousing);
    glBufferData(GL_ARRAY_BUFFER, sizeof(housingV), housingV, GL_STATIC_DRAW);

    glGenBuffers(3, vboCircle);
    for (int i = 0; i < 3; i++) {
        glBindBuffer(GL_ARRAY_BUFFER, vboCircle[i]);
        glBufferData(GL_ARRAY_BUFFER, SEG*9*sizeof(GLfloat), circleV[i], GL_STATIC_DRAW);
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glClearColor(0.12f, 0.12f, 0.12f, 1.0f);
}

// ---------- free GPU buffers (must be called from the main GL thread) -----
void cleanup() {
    glDeleteBuffers(1, &vboHousing);
    glDeleteBuffers(3, vboCircle);
}

// ---------- set color for each light based on active signal ---------------
static void setLightColor(int lightIdx) {
    Signal s;
    { lock_guard<mutex> lk(g_mutex); s = g_signal; }

    if (s == ALL_OFF) { glColor3f(0.12f, 0.12f, 0.12f); return; }

    bool on = (s == (Signal)(lightIdx + 1));
    if (!on) { glColor3f(0.15f, 0.15f, 0.15f); return; }

    switch (lightIdx) {
        case 0: glColor3f(1.0f,  0.10f, 0.10f); break; // red
        case 1: glColor3f(1.0f,  0.85f, 0.05f); break; // yellow
        case 2: glColor3f(0.10f, 0.90f, 0.15f); break; // green
    }
}

// ---------- GLUT display --------------------------------------------------
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glEnableClientState(GL_VERTEX_ARRAY);

    glColor3f(0.22f, 0.22f, 0.22f);
    glBindBuffer(GL_ARRAY_BUFFER, vboHousing);
    glVertexPointer(3, GL_FLOAT, 0, 0);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    for (int i = 0; i < 3; i++) {
        setLightColor(i);
        glBindBuffer(GL_ARRAY_BUFFER, vboCircle[i]);
        glVertexPointer(3, GL_FLOAT, 0, 0);
        glDrawArrays(GL_TRIANGLES, 0, SEG * 3);
    }

    glDisableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glFlush();
}

// ---------- shared: apply a valid menu choice -----------------------------
static void applyChoice(int choice) {
    const char* msg = nullptr;
    {
        lock_guard<mutex> lk(g_mutex);
        switch (choice) {
            case 1: g_signal = RED;     msg = "Red / Stop";      break;
            case 2: g_signal = YELLOW;  msg = "Yellow / Caution"; break;
            case 3: g_signal = GREEN;   msg = "Green / Go";       break;
            case 4: g_signal = ALL_OFF; msg = "All Off";          break;
            case 5: g_signal = ALL_OFF; msg = "Reset";            break;
            case 0: g_exitRequested = true; return; // main thread handles cleanup
            default: cout << "  [!] Invalid (0-5 only).\n"; return;
        }
        g_redraw = true;
    }
    cout << "  -> " << msg << "\n";
}

// ---------- GLUT keyboard: press 0-5 in the window -----------------------
void keyboard(unsigned char key, int /*x*/, int /*y*/) {
    if (key >= '0' && key <= '5') applyChoice(key - '0');
}

// ---------- GLUT idle: reads flags written by menu thread -----------------
void idle() {
    bool need, quit;
    { lock_guard<mutex> lk(g_mutex); need = g_redraw; g_redraw = false; quit = g_exitRequested; }
    if (quit) { cleanup(); cout << "Goodbye.\n"; exit(0); } // GL on main thread
    if (need) glutPostRedisplay();
}

// ---------- console menu (background thread) ------------------------------
void menuThread() {
    while (true) {
        cout << "\n=== Traffic Signal Visualizer ===\n"
             << "  1. Red    / Stop\n"
             << "  2. Yellow / Caution\n"
             << "  3. Green  / Go\n"
             << "  4. All Off\n"
             << "  5. Reset\n"
             << "  0. Exit\n"
             << "Selection: ";

        int choice;
        if (!(cin >> choice)) {
            cin.clear(); cin.ignore(1000, '\n');
            cout << "  [!] Enter a number 0-5.\n";
            continue;
        }
        applyChoice(choice);
    }
}

// ---------- main ----------------------------------------------------------
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(300, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Campus Traffic Signal Visualizer");

    GLenum err = glewInit();
    if (err != GLEW_OK) {
        cerr << "GLEW error: " << glewGetErrorString(err) << "\n";
        return 1;
    }

    initBuffers();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle);

    thread t(menuThread);
    t.detach();

    glutMainLoop();
    return 0;
}
