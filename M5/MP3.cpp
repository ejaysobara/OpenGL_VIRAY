// MP3 - Warehouse Robot Route Simulator
// CS0045 | Dynamic VBO + glMapBuffer, console menu + keyboard, GLEW + GLUT
// Build: g++ MP3.cpp -o MP3 -lglew32 -lfreeglut -lopengl32

#include <iostream>
#include <thread>
#include <mutex>
#include <cmath>
#include <GL/glew.h>
#include <GL/glut.h>

using namespace std;

// ---- state ----------------------------------------------------------------
mutex g_mutex;
bool  g_exitRequested = false;

float g_step   = 0.05f;
bool  g_autoOn = false;   // toggled by choice 7; read+written on main thread only

// ---- warehouse bounds (NDC) -----------------------------------------------
const float WL = -0.88f, WR = 0.88f, WB = -0.88f, WT = 0.88f;
const float ROBOT_MARGIN = 0.08f; // half-size of robot triangle

// ---- robot base shape (relative offsets from centre) ----------------------
const float RBX[3] = {  0.00f, -0.06f,  0.06f };
const float RBY[3] = {  0.09f, -0.06f, -0.06f };

// current robot centre (tracked so we can enforce boundary)
float g_rx = 0.0f, g_ry = 0.0f;

// ---- VBOs -----------------------------------------------------------------
GLuint vboWarehouse; // 4-corner rectangle for GL_LINE_LOOP
GLuint vboZones;     // 3 zones × 2 triangles = 18 verts
GLuint vboRobot;     // 3 verts, GL_DYNAMIC_DRAW

// Warehouse outline
GLfloat warehouseV[] = {
    WL, WB, 0,  WR, WB, 0,  WR, WT, 0,  WL, WT, 0
};

// Destination zones (2 triangles each)
// Zone A (CYAN)    top-left
// Zone B (MAGENTA) top-right
// Zone C (YELLOW)  bottom-center
GLfloat zoneV[] = {
    // Zone A
    -0.88f, 0.55f,0, -0.55f, 0.55f,0, -0.55f, 0.88f,0,
    -0.88f, 0.55f,0, -0.55f, 0.88f,0, -0.88f, 0.88f,0,
    // Zone B
     0.55f, 0.55f,0,  0.88f, 0.55f,0,  0.88f, 0.88f,0,
     0.55f, 0.55f,0,  0.88f, 0.88f,0,  0.55f, 0.88f,0,
    // Zone C
    -0.18f,-0.88f,0,  0.18f,-0.88f,0,  0.18f,-0.55f,0,
    -0.18f,-0.88f,0,  0.18f,-0.55f,0, -0.18f,-0.55f,0
};

// Robot triangle (initial position = origin + base offsets)
GLfloat robotV[] = {
    RBX[0],RBY[0],0,
    RBX[1],RBY[1],0,
    RBX[2],RBY[2],0
};

// ---- pending action from menu thread -------------------------------------
struct Action { bool valid=false; int choice=0; float stepSize=0; };
Action g_pending;

// ---- forward declaration --------------------------------------------------
void cleanup();

// ---- init -----------------------------------------------------------------
void initBuffers() {
    // Warehouse (static, never changes)
    glGenBuffers(1, &vboWarehouse);
    glBindBuffer(GL_ARRAY_BUFFER, vboWarehouse);
    glBufferData(GL_ARRAY_BUFFER, sizeof(warehouseV), warehouseV, GL_STATIC_DRAW);

    // Zones (static)
    glGenBuffers(1, &vboZones);
    glBindBuffer(GL_ARRAY_BUFFER, vboZones);
    glBufferData(GL_ARRAY_BUFFER, sizeof(zoneV), zoneV, GL_STATIC_DRAW);

    // Robot (dynamic — updated every move via glMapBuffer)
    glGenBuffers(1, &vboRobot);
    glBindBuffer(GL_ARRAY_BUFFER, vboRobot);
    glBufferData(GL_ARRAY_BUFFER, sizeof(robotV), robotV, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glClearColor(0.08f, 0.10f, 0.12f, 1.0f);

    cout << "Zones: [A] Cyan=top-left  [B] Magenta=top-right  [C] Yellow=bottom-center\n";
    cout << "Window keys: 1/2/3/4 = move | 6 = reset | 7 = demo | 0 = exit\n";
    cout << "To set step size, type 5 in the terminal and enter a value.\n";
}

// ---- move robot: update VBO via glMapBuffer (main thread only) -----------
void moveRobot(float dx, float dy) {
    // Clamp new centre to warehouse interior
    float nx = fmaxf(WL + ROBOT_MARGIN, fminf(WR - ROBOT_MARGIN, g_rx + dx));
    float ny = fmaxf(WB + ROBOT_MARGIN, fminf(WT - ROBOT_MARGIN, g_ry + dy));
    float adx = nx - g_rx, ady = ny - g_ry;
    g_rx = nx; g_ry = ny;
    if (adx == 0 && ady == 0) return;

    glBindBuffer(GL_ARRAY_BUFFER, vboRobot);
    GLfloat* v = (GLfloat*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);
    if (v) {
        // Apply the same displacement to all three vertices (preserves shape)
        v[0] += adx; v[1] += ady;   // vertex 0: x, y
        v[3] += adx; v[4] += ady;   // vertex 1: x, y
        v[6] += adx; v[7] += ady;   // vertex 2: x, y
        glUnmapBuffer(GL_ARRAY_BUFFER);
    } else {
        cout << "  [!] glMapBuffer returned NULL; move skipped.\n";
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// ---- reset robot to origin via glMapBuffer --------------------------------
void resetRobot() {
    g_rx = g_ry = 0.0f;
    glBindBuffer(GL_ARRAY_BUFFER, vboRobot);
    GLfloat* v = (GLfloat*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    if (v) {
        for (int i = 0; i < 3; i++) { v[i*3]=RBX[i]; v[i*3+1]=RBY[i]; v[i*3+2]=0; }
        glUnmapBuffer(GL_ARRAY_BUFFER);
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    cout << "  -> Returned to start.\n";
}

// ---- display --------------------------------------------------------------
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glEnableClientState(GL_VERTEX_ARRAY);

    // Warehouse outline
    glColor3f(0.75f, 0.75f, 0.75f);
    glBindBuffer(GL_ARRAY_BUFFER, vboWarehouse);
    glVertexPointer(3, GL_FLOAT, 0, 0);
    glDrawArrays(GL_LINE_LOOP, 0, 4);

    // Destination zones
    glBindBuffer(GL_ARRAY_BUFFER, vboZones);
    glVertexPointer(3, GL_FLOAT, 0, 0);
    glColor3f(0.0f, 0.70f, 0.70f); glDrawArrays(GL_TRIANGLES,  0, 6); // A cyan
    glColor3f(0.70f, 0.0f, 0.70f); glDrawArrays(GL_TRIANGLES,  6, 6); // B magenta
    glColor3f(0.70f, 0.70f, 0.0f); glDrawArrays(GL_TRIANGLES, 12, 6); // C yellow

    // Robot (bright orange triangle)
    glColor3f(1.0f, 0.50f, 0.0f);
    glBindBuffer(GL_ARRAY_BUFFER, vboRobot);
    glVertexPointer(3, GL_FLOAT, 0, 0);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glDisableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glFlush();
}

// ---- auto-move demo: timer schedules itself, NOT called from display() ----
static int s_demoDir = 0, s_demoTick = 0;
static const float DX[] = { 0.04f,  0.0f, -0.04f,  0.0f };
static const float DY[] = { 0.0f,   0.04f,  0.0f, -0.04f };

void autoMoveTimer(int) {
    if (!g_autoOn) return; // demo turned off; stop rescheduling
    moveRobot(DX[s_demoDir], DY[s_demoDir]);
    if (++s_demoTick >= 18) { s_demoTick = 0; s_demoDir = (s_demoDir + 1) % 4; }
    glutPostRedisplay();
    glutTimerFunc(60, autoMoveTimer, 0); // reschedule from timer, not from display
}

// ---- process a validated menu choice (main thread only) ------------------
void processChoice(int choice, float stepSize = 0.0f) {
    switch (choice) {
        case 1: moveRobot(-g_step, 0);  cout << "  -> Move Left\n";  break;
        case 2: moveRobot( g_step, 0);  cout << "  -> Move Right\n"; break;
        case 3: moveRobot(0,  g_step);  cout << "  -> Move Up\n";    break;
        case 4: moveRobot(0, -g_step);  cout << "  -> Move Down\n";  break;
        case 5:
            g_step = stepSize;
            cout << "  -> Step size: " << g_step << "\n";
            break;
        case 6: resetRobot(); break;
        case 7:
            g_autoOn = !g_autoOn;
            if (g_autoOn) {
                s_demoDir = 0; s_demoTick = 0;
                glutTimerFunc(60, autoMoveTimer, 0); // start timer once here
                cout << "  -> Auto-Move Demo ON\n";
            } else {
                cout << "  -> Auto-Move Demo OFF\n";
            }
            break;
        case 0: cleanup(); cout << "Goodbye.\n"; exit(0);
    }
    glutPostRedisplay();
}

// ---- keyboard callback (main thread) -------------------------------------
// 1-4 = move; 6 = reset; 7 = demo; 0 = exit
// '5' (set step size) is intentionally console-only — it requires typed input
void keyboard(unsigned char key, int, int) {
    switch (key) {
        case '1': processChoice(1); break; // left
        case '2': processChoice(2); break; // right
        case '3': processChoice(3); break; // up
        case '4': processChoice(4); break; // down
        case '6': processChoice(6); break; // reset
        case '7': processChoice(7); break; // demo
        case '0': processChoice(0); break; // exit
    }
}

// ---- cleanup (GL resources, main thread only) ----------------------------
void cleanup() {
    glDeleteBuffers(1, &vboWarehouse);
    glDeleteBuffers(1, &vboZones);
    glDeleteBuffers(1, &vboRobot);
}

// ---- idle: dispatch pending action from menu thread ----------------------
void idle() {
    Action pa;
    bool quit;
    { lock_guard<mutex> lk(g_mutex); pa = g_pending; g_pending.valid = false; quit = g_exitRequested; }
    if (quit) { cleanup(); cout << "Goodbye.\n"; exit(0); }
    if (pa.valid) processChoice(pa.choice, pa.stepSize);
}

// ---- console menu (background thread) ------------------------------------
void menuThread() {
    while (true) {
        cout << "\n=== Warehouse Robot Simulator ===\n"
             << "  1. Move Left\n"
             << "  2. Move Right\n"
             << "  3. Move Up\n"
             << "  4. Move Down\n"
             << "  5. Set Step Size\n"
             << "  6. Return to Start\n"
             << "  7. Auto-Move Demo\n"
             << "  0. Exit\n"
             << "Selection: ";

        int choice;
        if (!(cin >> choice)) {
            cin.clear(); cin.ignore(1000, '\n');
            cout << "  [!] Enter a number 0-7.\n";
            continue;
        }
        if (choice < 0 || choice > 7) {
            cout << "  [!] Invalid selection (0-7 only).\n";
            continue;
        }

        float stepSize = 0.0f;
        if (choice == 5) {
            cout << "  Enter positive step size: ";
            if (!(cin >> stepSize) || stepSize <= 0.0f) {
                cin.clear(); cin.ignore(1000, '\n');
                cout << "  [!] Must be a positive number.\n";
                continue;
            }
        }

        { lock_guard<mutex> lk(g_mutex); g_pending = {true, choice, stepSize}; }
    }
}

// ---- main -----------------------------------------------------------------
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Warehouse Robot Route Simulator");

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
