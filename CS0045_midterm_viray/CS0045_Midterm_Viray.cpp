/*==============================================================================
  PLEASE INSERT COIN
  CS0045 midterm - C++ / OpenGL (compatibility profile) / FreeGLUT mini-game

  BUILD (same flags as the course "Build OpenGL Program" VS Code task):
      g++ PLEASE_INSERT_COIN.cpp -o PLEASE_INSERT_COIN.exe -lfreeglut -lopengl32 -lglu32

  WHAT IT IS
      A routine 2D snack purchase on a quiet night street suddenly becomes a
      3D rescue: you operate a maintenance claw inside the vending machine to
      recover the jammed snack. Three customers make increasingly odd requests.
      Nothing in the game text explains them; the last thing you see is a
      reflection in the glass.

  CONTROLS
      Mouse : click COIN SLOT, click the requested snack button, click the
              delivered snack to collect it, click menu options.
      Keys  : W A S D / arrow keys  move claw (X = left/right, Z = depth)
              SPACE                 lower claw to grab / release carried snack
              P or ESC              pause (rescue only)
              ENTER                 confirm / continue / collect
              R                     retry (only on the RECOVERY UNSUCCESSFUL screen)

  FILE LAYOUT (each section opens with a comment block explaining how it works)
      1. INCLUDES, CONSTANTS AND PALETTE
      2. AUTHORITATIVE DATA (enums, structs, the three stage records)
      3. COORDINATE, LAYOUT AND HIT-TEST HELPERS
      4. GAMEPLAY LOGIC AND COLLISION DETECTION
      5. SHARED RETRO PRESENTATION (primitives, text, GL pass setup, vertex arrays)
      6. 2D ENVIRONMENT AND CHARACTER RENDERING
      7. 3D ENVIRONMENT AND CLAW RENDERING
      8. HUD, MENUS AND TRANSITION RENDERING
      9. GLUT CALLBACKS AND TIMING
     10. main
==============================================================================*/


/*==============================================================================
  1. INCLUDES, CONSTANTS AND PALETTE

  HOW IT WORKS
      Everything tunable lives here as a named constant so gameplay numbers can
      be changed in one place (START_ATTEMPTS, tolerances, speeds, durations).
      Two coordinate spaces are used:
        * 2D "logical" space: 320 x 180 units, origin bottom-left. Street,
          machine, menus and all hit-test rectangles share it.
        * 3D machine-interior space: X = width, Y = height, Z = depth
          (larger Z = deeper into the machine, away from the camera).
      Gameplay (collision, speed, tolerance, chute) uses 3D world units only;
      window size and pixel scale never change gameplay.
      The palette is shared: warm amber/cream for the street and machine,
      cool blue-green for the interior with warm accents on the chute.
==============================================================================*/
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif
#include <GL/glut.h>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <cstdlib>

static const float PI_F = 3.14159265f;

// ---- window / logical 2D space -------------------------------------------
static const int   WIN_W = 1280, WIN_H = 720;      // proposed presentation size
static const float LW = 320.0f, LH = 180.0f;       // logical 2D size (16:9)

// ---- timing ----------------------------------------------------------------
static const float FIXED_STEP      = 0.010f;       // gameplay step (10 ms)
static const float MAX_FRAME_DELTA = 0.050f;       // clamp for long frames
static const int   MAX_CATCHUP     = 8;            // bounded catch-up steps
static const int   TIMER_MS        = 8;            // single update chain period

// ---- rescue world (3D units) ----------------------------------------------
static const float WX_MIN = -6.0f, WX_MAX = 6.0f;  // compartment width
static const float WZ_MIN =  0.0f, WZ_MAX = 8.0f;  // compartment depth
static const float CLAW_HALF   = 0.5f;             // claw footprint half-size
static const float SNACK_HALF  = 0.6f;             // package footprint half-size
static const float CLAW_SPEED  = 5.0f;             // units / second
static const float DESCEND_SPD = 7.0f;
static const float ASCEND_SPD  = 7.0f;
static const float CLAW_TOP_Y  = 6.2f;             // parked / carry height
static const float GRAB_TOL_X  = 0.45f;            // forgiving grab tolerance
static const float GRAB_TOL_Z  = 0.45f;
static const float DROP_TIME   = 0.45f;            // snack fall animation
static const int   START_ATTEMPTS = 3;             // adjustable prototype default
// chute acceptance rectangle (floor, front-right). The drawn guide uses
// exactly these numbers, so the visual rim equals the rule.
static const float CHUTE_X0 = 3.8f, CHUTE_X1 = 6.0f, CHUTE_Z0 = 0.0f, CHUTE_Z1 = 2.2f;
// stage-3 restocking arm
static const float ARM_HX = 1.3f, ARM_HZ = 0.5f, ARM_Z = 4.3f;
static const float ARM_MIN_X = -4.4f, ARM_MAX_X = 4.4f, ARM_SPEED = 1.8f;

// ---- scripted durations (seconds) -----------------------------------------
static const float T_JAM = 1.0f, T_WARN = 0.7f, T_SHIFT = 1.1f;
static const float T_TO3D = T_JAM + T_WARN + T_SHIFT;
static const float T_TO2D = 0.9f;
static const float T_RESULT_FADE = 0.5f;
static const float OUTRO_DUR[4] = { 0.9f, 1.4f, 1.1f, 0.6f };  // exit, linger, turn, flicker

// ---- palette ----------------------------------------------------------------
struct Col { float r, g, b; };
static const Col PAL_SKY_TOP   = { 0.05f, 0.06f, 0.16f };
static const Col PAL_SKY_LOW   = { 0.20f, 0.14f, 0.26f };
static const Col PAL_BUILDING  = { 0.10f, 0.09f, 0.17f };
static const Col PAL_BUILDING2 = { 0.14f, 0.12f, 0.21f };
static const Col PAL_WINDOW_ON = { 0.95f, 0.72f, 0.30f };
static const Col PAL_WINDOW_OFF= { 0.16f, 0.15f, 0.26f };
static const Col PAL_PAVEMENT  = { 0.22f, 0.21f, 0.28f };
static const Col PAL_CURB      = { 0.38f, 0.35f, 0.42f };
static const Col PAL_AMBER     = { 1.00f, 0.74f, 0.28f };
static const Col PAL_CREAM     = { 1.00f, 0.93f, 0.74f };
static const Col PAL_MACHINE   = { 0.52f, 0.16f, 0.12f };
static const Col PAL_MACHINE_D = { 0.30f, 0.09f, 0.08f };
static const Col PAL_PANEL     = { 0.16f, 0.09f, 0.07f };
static const Col PAL_GLASS     = { 0.80f, 0.74f, 0.52f };
static const Col PAL_SKIN      = { 0.92f, 0.72f, 0.58f };
static const Col PAL_INK       = { 0.05f, 0.04f, 0.07f };
static const Col PAL_WHITE     = { 1.00f, 1.00f, 1.00f };
static const Col PAL_CHIPS     = { 0.85f, 0.20f, 0.16f };
static const Col PAL_CHOC      = { 0.40f, 0.22f, 0.12f };
static const Col PAL_JUICE     = { 0.98f, 0.55f, 0.12f };
static const Col PAL_GOLD      = { 0.95f, 0.80f, 0.25f };
static const Col PAL_TEAL_D    = { 0.04f, 0.12f, 0.16f };   // interior dark
static const Col PAL_TEAL      = { 0.10f, 0.30f, 0.34f };
static const Col PAL_TEAL_L    = { 0.20f, 0.52f, 0.52f };
static const Col PAL_STEEL     = { 0.45f, 0.58f, 0.62f };
static const Col PAL_WARN      = { 1.00f, 0.55f, 0.15f };
static const Col PAL_MARKER    = { 1.00f, 0.95f, 0.30f };

static Col mixCol(Col a, Col b, float t) {
    Col c = { a.r + (b.r - a.r) * t, a.g + (b.g - a.g) * t, a.b + (b.b - a.b) * t };
    return c;
}
static Col scaleCol(Col a, float s) { Col c = { a.r * s, a.g * s, a.b * s }; return c; }
static void setCol(Col c, float a = 1.0f) { glColor4f(c.r, c.g, c.b, a); }
static float clampf(float v, float lo, float hi) { return v < lo ? lo : (v > hi ? hi : v); }
static float lerpf(float a, float b, float t) { return a + (b - a) * t; }
static float smooth01(float t) { t = clampf(t, 0.0f, 1.0f); return t * t * (3.0f - 2.0f * t); }


/*==============================================================================
  2. AUTHORITATIVE DATA

  HOW IT WORKS
      Exactly one top-level GameState is active; a pause/help/confirm overlay
      is a separate Overlay value so the paused rescue substate is preserved.
      The claw has its own ClawState. Small structs own related values:
        GameData  - state, stage, attempts, purchase and result flags
        ClawData  - position, held keys, substate, carry flag, drop outcome
        StageData - three fixed records (target, layout, dialogue, colours)
        Clocks    - scene/cue/outro timers (presentation only)
      Renderers only READ these; the helpers in section 4 and the input
      callbacks in section 9 are the only code that changes them.
==============================================================================*/
enum GameState { TITLE, CUSTOMER_INTRO, MACHINE_2D, TRANSITION_TO_3D, RESCUE_3D,
                 TRANSITION_TO_2D, STAGE_RESULT, GAME_OVER, REFLECTION_OUTRO };
enum ClawState { IDLE_EMPTY, DESCENDING, ASCENDING_EMPTY, ASCENDING_WITH_SNACK,
                 CARRYING, RESOLVING_DROP };
enum Overlay   { OV_NONE, OV_PAUSE, OV_HELP_TITLE, OV_HELP_RESCUE,
                 OV_CONFIRM_RESTART, OV_CONFIRM_STREET };
enum SnackType { SNACK_CHIPS = 0, SNACK_CHOC = 1, SNACK_JUICE = 2 };
enum FailReason { FAIL_MISS, FAIL_DROP };
enum OutroPhase { OUT_EXIT = 0, OUT_LINGER, OUT_TURN, OUT_FLICKER };

struct Package { float x, z; SnackType type; };

struct StageData {
    SnackType target;
    float tx, tz;                 // target start position
    float startX, startZ;         // claw start (always clear of obstacles)
    Package obs[8];               // fixed packages (solid)
    int   obsCount;
    bool  hasArm;                 // moving restocking arm (stage 3)
    const char* request;          // editable customer lines
    const char* reaction;
    Col   shirt, accent;
};

// Snack dimensions (draw size) and names, indexed by SnackType.
static const float SNACK_W[3] = { 1.10f, 1.00f, 0.95f };
// collision half-extents match what is DRAWN (crimps, wrapper ends included)
static const float SNACK_HX[3] = { 0.58f, 0.76f, 0.50f };
static const float SNACK_HZ[3] = { 0.31f, 0.47f, 0.50f };
static const float SNACK_H[3] = { 1.50f, 0.85f, 1.40f };
static const float SNACK_D[3] = { 0.55f, 0.90f, 0.95f };
static const char* SNACK_NAME[3] = { "CHIPS", "CHOCOLATE BAR", "JUICE CARTON" };
static const char* SNACK_BTN[3]  = { "CHIPS", "CHOC", "JUICE" };

// Three stages. Stage 1 = open approach (tutorial), stage 2 = tighter
// arrangement, stage 3 = juice carton behind a moving restocking arm.
static const StageData STAGES[3] = {
  { SNACK_CHIPS, -2.0f, 5.5f, -4.0f, 1.0f,
    { {-5.0f, 5.8f, SNACK_JUICE}, {3.5f, 6.0f, SNACK_CHOC}, {0.5f, 3.5f, SNACK_CHIPS} }, 3, false,
    "The chips, please. I've been here all evening.",
    "Thanks. That's better.",
    { 0.22f, 0.36f, 0.70f }, { 0.25f, 0.18f, 0.12f } },
  { SNACK_CHOC, 1.0f, 5.0f, -4.0f, 1.0f,
    { {-1.4f, 5.0f, SNACK_CHIPS}, {3.4f, 5.0f, SNACK_JUICE}, {1.0f, 7.0f, SNACK_CHIPS},
      {-4.0f, 3.2f, SNACK_JUICE}, {-1.2f, 2.8f, SNACK_CHOC}, {5.0f, 3.4f, SNACK_CHOC} }, 6, false,
    "The chocolate one. The wrapper needs to stay on.",
    "Thanks. That's better. I'll leave it wrapped for now.",
    { 0.42f, 0.44f, 0.50f }, { 0.78f, 0.12f, 0.15f } },
  { SNACK_JUICE, 0.0f, 6.6f, -4.0f, 1.0f,
    { {-3.2f, 6.6f, SNACK_CHIPS}, {3.2f, 6.6f, SNACK_CHOC}, {-5.0f, 3.0f, SNACK_JUICE} }, 3, true,
    "A juice carton, please. We weren't sure you'd still be here.",
    "Thanks. That's better. You can go now.",
    { 0.95f, 0.78f, 0.18f }, { 0.95f, 0.45f, 0.10f } }
};

struct ClawData {
    float x, z, y;                // world position (y = vertical)
    ClawState state;
    bool  carrying;               // exactly one target can be attached
    bool  missPending;            // a descent that found nothing
    bool  dropSuccess;            // outcome recorded at release time
    float dropT;                  // fall-animation progress
    float snackX, snackZ, snackY; // target position when not carried
    float snackFallFromY;
};

struct GameData {
    GameState state;
    Overlay   overlay;
    int       stageIndex;         // always in [0,2]
    int       attempts;
    bool      hasCoin;
    bool      cycleResolved;      // guard: a grab/drop cycle charges at most once
    bool      delivered;
    int       resultPhase;        // 0 = awaiting collection, 1 = reaction shown
    int       menuSel;            // pause menu / title focus
    int       confSel;            // confirm dialog focus (0 = Cancel)
    int       titleSel;
    int       outroPhase;
};

struct Clocks {
    float stateT;                 // time since the current state began
    float cueT;                   // customer-arrival cue clock (intro + purchase)
    float titleT;                 // ambient presentation clock
    float visT;                   // 3D visual clock (frozen while rescue is paused / failed)
    float feedbackT;              // purchase feedback line lifetime
    float failT;                  // "attempt lost" notice
    float rescueT;                // time since rescue reveal (help prompt)
    float armX, armDir;           // moving restocking arm phase
    char  feedback[48];
    char  failMsg[48];
};

static GameData G;
static ClawData C;
static Clocks   K;

// held movement keys, one flag per physical binding (so releasing A does not
// cancel a still-held Left arrow)
enum { H_A, H_D, H_W, H_S, H_LEFT, H_RIGHT, H_UP, H_DOWN, H_COUNT };
static bool gHeld[H_COUNT];

static int  gWinW = WIN_W, gWinH = WIN_H;
static int  gLastMs = 0;
static float gAccum = 0.0f;


/*==============================================================================
  3. COORDINATE, LAYOUT AND HIT-TEST HELPERS

  HOW IT WORKS
      The window may be any size, but the picture is always a 16:9 content
      rectangle centred in the window (letterbox bars are black). GLUT reports
      the mouse with a TOP-left origin in pixels; mouseToLogical() flips Y and
      maps through the content rectangle into the 320x180 bottom-left logical
      space. Clicks in the bars are rejected. Every clickable control is a
      named Rect below and the SAME Rect is used for drawing and hit testing,
      so what you see is what you click.
==============================================================================*/
struct Rect { float x0, y0, x1, y1; };
static bool inRect(const Rect& r, float x, float y) {
    return x >= r.x0 && x <= r.x1 && y >= r.y0 && y <= r.y1;
}

struct ViewportInfo { int x, y, w, h; float scale; };
static ViewportInfo gVP = { 0, 0, WIN_W, WIN_H, 4.0f };

static void computeViewport() {
    int w = gWinW < 1 ? 1 : gWinW, h = gWinH < 1 ? 1 : gWinH;    // zero-size guard
    float s = fminf(w / LW, h / LH);
    gVP.scale = s;
    gVP.w = (int)(LW * s); gVP.h = (int)(LH * s);
    gVP.x = (w - gVP.w) / 2; gVP.y = (h - gVP.h) / 2;
}

static bool mouseToLogical(int mx, int my, float& lx, float& ly) {
    float px = (float)mx;
    float py = (float)(gWinH - my);                  // flip to bottom-left origin
    lx = (px - gVP.x) / gVP.scale;
    ly = (py - gVP.y) / gVP.scale;
    return lx >= 0.0f && lx <= LW && ly >= 0.0f && ly <= LH;   // reject bars
}

// ---- shared layout rectangles (logical units) ----------------------------
static const Rect R_CABINET  = {  92.0f,  36.0f, 208.0f, 174.0f };
static const Rect R_GLASS    = { 100.0f,  72.0f, 170.0f, 166.0f };
static const Rect R_PANEL    = { 174.0f,  72.0f, 204.0f, 166.0f };
static const Rect R_CHUTE2D  = { 100.0f,  40.0f, 170.0f,  64.0f };   // flap / collection
static const Rect R_DISPLAY  = { 176.0f, 142.0f, 202.0f, 158.0f };
static const Rect R_COIN     = { 180.0f, 124.0f, 198.0f, 138.0f };
static const Rect R_BTN[3]   = { { 178.0f, 108.0f, 200.0f, 119.0f },
                                 { 178.0f,  94.0f, 200.0f, 105.0f },
                                 { 178.0f,  80.0f, 200.0f,  91.0f } };
static const Rect R_BAND     = {   6.0f,   2.0f, 314.0f,  30.0f };   // dialogue band
static const Rect R_CONTINUE = { 246.0f,   5.0f, 311.0f,  17.0f };
static const Rect R_TITLE_START = { 10.0f, 96.0f, 82.0f, 110.0f };
static const Rect R_TITLE_HELP  = { 10.0f, 78.0f, 82.0f,  92.0f };
static const Rect R_PAUSE_OPT[4] = { { 100.0f, 112.0f, 220.0f, 128.0f },
                                     { 100.0f,  92.0f, 220.0f, 108.0f },
                                     { 100.0f,  72.0f, 220.0f,  88.0f },
                                     { 100.0f,  52.0f, 220.0f,  68.0f } };
static const Rect R_CONF_CANCEL = { 100.0f, 46.0f, 155.0f, 60.0f };
static const Rect R_CONF_OK     = { 165.0f, 46.0f, 220.0f, 60.0f };
static const Rect R_RETRY       = { 120.0f, 66.0f, 200.0f, 82.0f };
static const Rect R_HELP_BACK   = { 135.0f,  4.0f, 185.0f, 16.0f };

// Control labels, used by BOTH the HUD and the help panels so they agree.
static const char* CTRL_MOVE    = "MOVE: W A S D / ARROWS";
static const char* CTRL_LOWER   = "SPACE: LOWER CLAW";
static const char* CTRL_RELEASE = "SPACE: RELEASE";
static const char* CTRL_PAUSE   = "P / ESC: PAUSE";
static const char* PAUSE_SHORT[4] = { "RESUME", "RESTART STAGE", "CONTROLS", "END RUN" };
static const char* PAUSE_LABEL[4] = { "RESUME", "REINITIALIZE (RESTART STAGE)", "CONTROLS",
                                      "RETURN TO STREET (END RUN)" };


/*==============================================================================
  4. GAMEPLAY LOGIC AND COLLISION DETECTION

  HOW IT WORKS
      * State changes go through transitionTo(), which initialises each
        state's clocks exactly once and clears held keys.
      * initializeNewGame / initializeStage / retryStage / returnToTitle have
        distinct jobs (full reset / next stage / same-stage retry with
        attempts restored / back to the unpaid title).
      * Rescue simulation (stepRescue) runs in fixed 10 ms steps from the single
        timer chain. The claw moves with no inertia; X/Z is clamped to the
        compartment and tested against solid packages and the moving arm using
        the claw's (or carried snack's) real footprint, one axis at a time.
      * Grab: only the current target can be taken and only if the claw is
        within GRAB_TOL_X/Z (equality accepted). Carried snack never slips.
      * Every failed grab / invalid drop funnels through resolveFailure(),
        guarded by cycleResolved so one cycle costs exactly one attempt.
      * Release records its outcome AT release time (chute rectangle test on
        the snack centre); the fall animation can't change the result.
      * Purchase rules: a coin enables selection, extra coins are ignored, a
        wrong item keeps the credit and costs no attempt.
==============================================================================*/
static const StageData& stage() { return STAGES[G.stageIndex]; }

static void clearHeldKeys() { for (int i = 0; i < H_COUNT; ++i) gHeld[i] = false; }

static void setFeedback(const char* s) {
    strncpy(K.feedback, s, sizeof(K.feedback) - 1);
    K.feedback[sizeof(K.feedback) - 1] = 0;
    K.feedbackT = 2.4f;
}

static void resetArm() { K.armX = ARM_MIN_X; K.armDir = 1.0f; }

// Restores claw, target and obstacle phase to this stage's safe start layout.
// Preserves attempts and purchase state.
static void resetRescueLayout() {
    const StageData& s = stage();
    C.x = s.startX; C.z = s.startZ; C.y = CLAW_TOP_Y;
    C.state = IDLE_EMPTY; C.carrying = false; C.missPending = false;
    C.dropSuccess = false; C.dropT = 0.0f;
    C.snackX = s.tx; C.snackZ = s.tz; C.snackY = SNACK_H[s.target] * 0.5f;
    C.snackFallFromY = C.snackY;
    resetArm();
    clearHeldKeys();
    gAccum = 0.0f;
}

static void transitionTo(GameState s) {
    G.state = s;
    K.stateT = 0.0f;
    clearHeldKeys();
    gAccum = 0.0f;
    if (s == CUSTOMER_INTRO) K.cueT = 0.0f;               // arrival cues start here
    if (s == RESCUE_3D) K.rescueT = 0.0f;
    if (s == STAGE_RESULT) G.resultPhase = 0;
    if (s == REFLECTION_OUTRO) G.outroPhase = OUT_EXIT;
}

static void initializeStage(int i) {
    G.stageIndex = (i < 0) ? 0 : (i > 2 ? 2 : i);
    G.attempts = START_ATTEMPTS;
    G.hasCoin = false; G.delivered = false; G.cycleResolved = false;
    G.overlay = OV_NONE;
    K.cueT = 0.0f; K.feedbackT = 0.0f; K.failT = 0.0f; K.feedback[0] = 0;
    resetRescueLayout();
}

static void initializeNewGame() {
    G.stageIndex = 0; G.resultPhase = 0; G.outroPhase = 0;
    G.menuSel = 0; G.confSel = 0; G.titleSel = 0;
    initializeStage(0);
}

static void returnToTitle() {
    initializeNewGame();
    transitionTo(TITLE);
}

static void startGame() {
    initializeNewGame();
    transitionTo(CUSTOMER_INTRO);
}

// Same-stage retry: full attempts, clean claw/target/arm, straight back to
// the rescue (the purchase is already done, the customer does not change).
static void retryStage() {
    G.attempts = START_ATTEMPTS;
    G.cycleResolved = false; G.delivered = false;
    G.overlay = OV_NONE;
    K.failT = 0.0f;
    resetRescueLayout();
    transitionTo(RESCUE_3D);
}

// ---- purchase ---------------------------------------------------------------
static void insertCoin() {
    if (G.state != MACHINE_2D) return;
    if (G.hasCoin) setFeedback("CREDIT ALREADY INSERTED");
    else { G.hasCoin = true; setFeedback("COIN ACCEPTED"); }
}

static void selectSnack(int type) {
    if (G.state != MACHINE_2D) return;
    if (!G.hasCoin) { setFeedback("INSERT COIN FIRST"); return; }
    if ((SnackType)type != stage().target) { setFeedback("PLEASE CHECK YOUR SELECTION"); return; }
    G.hasCoin = false;                              // correct pick consumes the credit
    resetRescueLayout();
    transitionTo(TRANSITION_TO_3D);                 // jam -> warning -> camera shift
}

// ---- collision ----------------------------------------------------------------
static bool rectOverlap(float ax, float az, float ahx, float ahz, float bx, float bz, float bhx, float bhz) {
    return fabsf(ax - bx) < ahx + bhx && fabsf(az - bz) < ahz + bhz;
}

// moving footprint: the claw, or the carried snack combined with the claw
static float activeHX() { return C.carrying ? fmaxf(CLAW_HALF, SNACK_HX[stage().target]) : CLAW_HALF; }
static float activeHZ() { return C.carrying ? fmaxf(CLAW_HALF, SNACK_HZ[stage().target]) : CLAW_HALF; }

// true if a footprint of half-extents (hx,hz) centred at (x,z) hits a solid.
static bool footprintBlocked(float x, float z, float hx, float hz) {
    const StageData& s = stage();
    for (int i = 0; i < s.obsCount; ++i) {
        int t = s.obs[i].type;
        if (rectOverlap(x, z, hx, hz, s.obs[i].x, s.obs[i].z, SNACK_HX[t], SNACK_HZ[t])) return true;
    }
    if (s.hasArm && rectOverlap(x, z, hx, hz, K.armX, ARM_Z, ARM_HX, ARM_HZ)) return true;
    return false;
}

static bool inChute(float x, float z) {
    const float e = 1e-4f;
    return x >= CHUTE_X0 - e && x <= CHUTE_X1 + e && z >= CHUTE_Z0 - e && z <= CHUTE_Z1 + e;
}

static float clawBottomY() { return SNACK_H[stage().target] + 0.6f; }

static bool targetInGrabRange() {
    return fabsf(C.x - C.snackX) <= GRAB_TOL_X + 1e-4f && fabsf(C.z - C.snackZ) <= GRAB_TOL_Z + 1e-4f;
}

// ---- attempts -------------------------------------------------------------
static void resolveFailure(FailReason why) {
    if (G.cycleResolved) return;                    // one cycle, one charge
    G.cycleResolved = true;
    if (G.attempts > 0) G.attempts--;
    snprintf(K.failMsg, sizeof(K.failMsg), "%s - ATTEMPT LOST", why == FAIL_MISS ? "NO GRIP" : "MISSED CHUTE");
    K.failT = 2.5f;
    if (G.attempts <= 0) { transitionTo(GAME_OVER); return; }   // keep the failed pose as the backdrop
    resetRescueLayout();                            // safe layout, attempts preserved
}

static void deliverSuccess() {
    if (G.cycleResolved) return;                    // never award twice
    G.cycleResolved = true;
    G.delivered = true;
    transitionTo(TRANSITION_TO_2D);
}

// ---- claw actions (Space) ---------------------------------------------------
static void requestGrabOrRelease() {
    if (G.state != RESCUE_3D || G.overlay != OV_NONE) return;
    if (C.state == IDLE_EMPTY) {
        clearHeldKeys();                            // motion input is ignored during the action
        G.cycleResolved = false;                    // a new grab cycle begins
        C.missPending = false;
        C.state = DESCENDING;
    } else if (C.state == CARRYING) {
        clearHeldKeys();
        C.snackX = C.x; C.snackZ = C.z;
        C.snackFallFromY = C.y - 0.5f - SNACK_H[stage().target] * 0.5f;
        C.dropSuccess = inChute(C.snackX, C.snackZ);   // outcome fixed now
        C.carrying = false;
        C.dropT = 0.0f;
        C.state = RESOLVING_DROP;
    }
}

// ---- fixed-step rescue simulation -----------------------------------------
static void moveClaw(float dt) {
    float dx = ((gHeld[H_D] || gHeld[H_RIGHT]) ? 1.0f : 0.0f) - ((gHeld[H_A] || gHeld[H_LEFT]) ? 1.0f : 0.0f);
    float dz = ((gHeld[H_W] || gHeld[H_UP]) ? 1.0f : 0.0f) - ((gHeld[H_S] || gHeld[H_DOWN]) ? 1.0f : 0.0f);
    if (dx == 0.0f && dz == 0.0f) return;
    if (dx != 0.0f && dz != 0.0f) { dx *= 0.70710678f; dz *= 0.70710678f; }   // cap diagonal
    float hx = activeHX(), hz = activeHZ();
    float nx = clampf(C.x + dx * CLAW_SPEED * dt, WX_MIN + hx, WX_MAX - hx);
    if (!footprintBlocked(nx, C.z, hx, hz)) C.x = nx;
    float nz = clampf(C.z + dz * CLAW_SPEED * dt, WZ_MIN + hz, WZ_MAX - hz);
    if (!footprintBlocked(C.x, nz, hx, hz)) C.z = nz;
}

static void updateArm(float dt) {
    if (!stage().hasArm) return;
    float nx = K.armX + K.armDir * ARM_SPEED * dt;
    if (nx > ARM_MAX_X) { nx = ARM_MAX_X; K.armDir = -1.0f; }
    if (nx < ARM_MIN_X) { nx = ARM_MIN_X; K.armDir = 1.0f; }
    // never push through the claw or its snack: reverse instead of teleporting
    if (rectOverlap(C.x, C.z, activeHX(), activeHZ(), nx, ARM_Z, ARM_HX, ARM_HZ)) { K.armDir = -K.armDir; return; }
    K.armX = nx;
}

static void stepRescue(float dt) {
    updateArm(dt);
    switch (C.state) {
    case IDLE_EMPTY:
    case CARRYING:
        moveClaw(dt);
        break;
    case DESCENDING:
        C.y -= DESCEND_SPD * dt;
        if (C.y <= clawBottomY()) {
            C.y = clawBottomY();
            if (targetInGrabRange()) { C.carrying = true; C.state = ASCENDING_WITH_SNACK; }
            else { C.missPending = true; C.state = ASCENDING_EMPTY; }
        }
        break;
    case ASCENDING_EMPTY:
        C.y += ASCEND_SPD * dt;
        if (C.y >= CLAW_TOP_Y) {
            C.y = CLAW_TOP_Y;
            if (C.missPending) resolveFailure(FAIL_MISS); else C.state = IDLE_EMPTY;
        }
        break;
    case ASCENDING_WITH_SNACK:
        C.y += ASCEND_SPD * dt;
        if (C.y >= CLAW_TOP_Y) { C.y = CLAW_TOP_Y; C.state = CARRYING; }
        break;
    case RESOLVING_DROP:
        C.dropT += dt;
        if (C.dropT >= DROP_TIME) {
            C.snackY = SNACK_H[stage().target] * 0.5f;
            if (C.dropSuccess) deliverSuccess(); else resolveFailure(FAIL_DROP);
        }
        break;
    }
}

// ---- result / outro ---------------------------------------------------------
static void acknowledgeResult() {
    if (G.state != STAGE_RESULT) return;
    if (G.resultPhase == 0) { G.resultPhase = 1; return; }   // collect once
    if (G.stageIndex < 2) { initializeStage(G.stageIndex + 1); transitionTo(CUSTOMER_INTRO); }
    else { transitionTo(REFLECTION_OUTRO); }          // never index a fourth stage
}

static void updateOutro(float dt) {
    K.stateT += dt;
    while (G.outroPhase <= OUT_FLICKER && K.stateT >= OUTRO_DUR[G.outroPhase]) {
        K.stateT -= OUTRO_DUR[G.outroPhase];          // boundaries crossed in order, once
        G.outroPhase++;
    }
    if (G.outroPhase > OUT_FLICKER) returnToTitle();  // ordinary PLEASE INSERT COIN screen
}

// ---- menu actions -----------------------------------------------------------
static void resumeRescue() {
    G.overlay = OV_NONE; clearHeldKeys(); gAccum = 0.0f;
    gLastMs = glutGet(GLUT_ELAPSED_TIME);          // no stale elapsed time applied after a pause
}

static void pauseRescue() {
    if (G.state != RESCUE_3D || G.overlay != OV_NONE) return;
    G.overlay = OV_PAUSE; G.menuSel = 0; clearHeldKeys();
}

static void activatePauseOption(int i) {
    switch (i) {
    case 0: resumeRescue(); break;
    case 1: G.overlay = OV_CONFIRM_RESTART; G.confSel = 0; break;     // defaults to Cancel
    case 2: G.overlay = OV_HELP_RESCUE; break;
    case 3: G.overlay = OV_CONFIRM_STREET; G.confSel = 0; break;
    }
}

static void activateConfirm(bool ok) {
    if (!ok) { G.overlay = OV_PAUSE; return; }
    if (G.overlay == OV_CONFIRM_RESTART) retryStage();
    else if (G.overlay == OV_CONFIRM_STREET) returnToTitle();
}

static void activateTitleOption(int i) {
    if (i == 0) startGame(); else G.overlay = OV_HELP_TITLE;
}

// ---- machine disturbance cues: pure functions of the scene clocks -----------
// Cues only change LOCAL BRIGHTNESS. They never touch credit, target, attempts,
// collision, input gating or progress, and are the same on every run.
struct Cues { float dip, indicator, selPulse, lamps, dim; };

static float pulseAt(float t, float t0, float dur) {
    if (t < t0 || t > t0 + dur) return 0.0f;
    return sinf((t - t0) / dur * PI_F);
}

static Cues computeCues() {
    Cues c = { 0, 0, 0, 0, 0 };
    float t = K.cueT;
    if (G.state == CUSTOMER_INTRO || G.state == MACHINE_2D) {
        switch (G.stageIndex) {
        case 0:   // brief display flicker + indicator blink
            c.dip = 0.8f * pulseAt(t, 0.4f, 0.15f);
            c.indicator = pulseAt(t, 0.4f, 0.20f);
            break;
        case 1:   // irregular selection-light pulses + recurring three-lamp pattern
            c.selPulse = 0.5f * fmaxf(pulseAt(t, 0.5f, 0.12f), fmaxf(pulseAt(t, 1.2f, 0.12f), pulseAt(t, 2.1f, 0.12f)));
            c.lamps = fmaxf(pulseAt(t, 2.5f, 0.4f), pulseAt(t, 5.0f, 0.4f));
            break;
        case 2:   // maintenance indicator flutter, nearby lights soften
            c.indicator = pulseAt(t, 0.0f, 0.15f);
            c.dim = 0.45f * smooth01(t / 0.4f);
            break;
        }
    } else if (G.state == REFLECTION_OUTRO) {
        float p = K.stateT;
        if (G.outroPhase == OUT_TURN)    c.lamps = pulseAt(p, 0.0f, 0.4f);       // same motif
        if (G.outroPhase == OUT_FLICKER) c.lamps = pulseAt(p, 0.0f, 0.4f);
    }
    return c;
}

// outro light flicker: brightness multiplier for the machine during FLICKER_VANISH
static float outroFlicker() {
    if (G.state != REFLECTION_OUTRO || G.outroPhase != OUT_FLICKER) return 1.0f;
    float u = K.stateT / OUTRO_DUR[OUT_FLICKER];
    if (u < 0.15f) return 1.0f;
    if (u < 0.30f) return 0.35f;
    if (u < 0.50f) return 0.9f;
    if (u < 0.65f) return 0.4f;
    return 1.0f;
}


/*==============================================================================
  5. SHARED RETRO PRESENTATION

  HOW IT WORKS
      * Small helpers (rect, triangle fan, text) used by every scene.
      * begin2D()/begin3D() are the "GL pass" helpers: each sets viewport,
        scissor, projection, model-view, depth, blending and client-array
        state it needs, so no scene relies on what a previous scene left.
      * Rendering goes straight to the window (the framebuffer-pixelation
        enhancement is deferred); the chunky look comes from grid-aligned
        shapes, a limited palette and flat face shading.

      VERTEX ARRAYS (course Module 04 evidence)
        - glDrawArrays : gRail2D (shelf rails/dividers in the machine glass,
          GL_LINES, 2 floats per vertex) and gGrid3D (floor grid of the
          machine interior, GL_LINES, 3 floats per vertex). A vertex here is
          one end of a line segment. Storing them once in an array avoids
          re-sending dozens of glVertex calls every frame.
        - glDrawElements : a unit cuboid. 24 face-local vertex records (4 per
          face - a corner appears in 3 faces, once per face, because every
          face has its own shade colour) and 36 GLushort indices (6 faces x
          2 triangles x 3). An index refers to a row in the vertex array, so
          the same mesh is reused with a translate/scale for every package,
          wall, pipe, shelf and claw part. glDrawElements differs from
          glDrawArrays in that it walks the INDEX list instead of the vertex
          list directly. Counts passed are index counts (36), not triangles.
==============================================================================*/
static void* fontSmall() { return gVP.scale >= 3.0f ? GLUT_BITMAP_9_BY_15 : GLUT_BITMAP_8_BY_13; }
static void* fontBig()   { return gVP.scale >= 3.0f ? GLUT_BITMAP_HELVETICA_18 : GLUT_BITMAP_HELVETICA_12; }

static float textW(const char* s, void* font) {
    int px = 0;
    for (; *s; ++s) px += glutBitmapWidth(font, *s);
    return px / gVP.scale;
}
static void drawTextRaw(float x, float y, const char* s, void* font) {
    glRasterPos2f(x, y);
    for (; *s; ++s) glutBitmapCharacter(font, *s);
}
// text with a dark drop-shadow for legibility over any background
static void drawText(float x, float y, const char* s, Col c, void* font, float a = 1.0f) {
    float o = 0.35f;
    setCol(PAL_INK, a); drawTextRaw(x + o, y - o, s, font);
    setCol(c, a);       drawTextRaw(x, y, s, font);
}
static void drawTextCentered(float cx, float y, const char* s, Col c, void* font, float a = 1.0f) {
    drawText(cx - textW(s, font) * 0.5f, y, s, c, font, a);
}

static void fillRect(float x0, float y0, float x1, float y1) {
    glBegin(GL_QUADS);
    glVertex2f(x0, y0); glVertex2f(x1, y0); glVertex2f(x1, y1); glVertex2f(x0, y1);
    glEnd();
}
static void fillRectC(const Rect& r, Col c, float a = 1.0f) { setCol(c, a); fillRect(r.x0, r.y0, r.x1, r.y1); }
static void strokeRect(float x0, float y0, float x1, float y1) {
    glBegin(GL_LINE_LOOP);
    glVertex2f(x0, y0); glVertex2f(x1, y0); glVertex2f(x1, y1); glVertex2f(x0, y1);
    glEnd();
}
static void fillEllipse(float cx, float cy, float rx, float ry, int seg = 14) {
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (int i = 0; i <= seg; ++i) {
        float a = 2.0f * PI_F * i / seg;
        glVertex2f(cx + cosf(a) * rx, cy + sinf(a) * ry);
    }
    glEnd();
}
static void fillTri(float x0, float y0, float x1, float y1, float x2, float y2) {
    glBegin(GL_TRIANGLES); glVertex2f(x0, y0); glVertex2f(x1, y1); glVertex2f(x2, y2); glEnd();
}

// full-screen (content) tint in logical space
static void fullTint(Col c, float a) {
    if (a <= 0.001f) return;
    setCol(c, clampf(a, 0.0f, 1.0f)); fillRect(0, 0, LW, LH);
}

// ---- GL pass setup / restore ----------------------------------------------
static void applyContentViewport() {
    glViewport(gVP.x, gVP.y, gVP.w, gVP.h);
    glEnable(GL_SCISSOR_TEST);
    glScissor(gVP.x, gVP.y, gVP.w, gVP.h);
}

static void begin2D() {
    applyContentViewport();
    glDisable(GL_DEPTH_TEST); glDisable(GL_LIGHTING); glDisable(GL_TEXTURE_2D); glDisable(GL_FOG);
    glDisableClientState(GL_VERTEX_ARRAY); glDisableClientState(GL_COLOR_ARRAY);
    glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glLineWidth(1.0f);
    glMatrixMode(GL_PROJECTION); glLoadIdentity(); glOrtho(0, LW, 0, LH, -1, 1);
    glMatrixMode(GL_MODELVIEW);  glLoadIdentity();
}

static void begin3D(float eyeBlend) {
    applyContentViewport();
    glEnable(GL_DEPTH_TEST); glDepthFunc(GL_LEQUAL); glDepthMask(GL_TRUE);
    glDisable(GL_LIGHTING); glDisable(GL_TEXTURE_2D); glDisable(GL_FOG);
    glDisable(GL_BLEND);
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    gluPerspective(48.0, LW / LH, 0.5, 120.0);
    glMatrixMode(GL_MODELVIEW);  glLoadIdentity();
    // fixed, moderately elevated oblique camera. eyeBlend 0 -> close to the
    // glass (used by the reveal), 1 -> the final rescue camera.
    float e = smooth01(eyeBlend);
    gluLookAt(lerpf(0.0f, 0.0f, e), lerpf(3.0f, 9.2f, e), lerpf(-3.0f, -8.8f, e),
              0.0f, lerpf(2.5f, 1.6f, e), lerpf(3.0f, 4.0f, e),
              0.0f, 1.0f, 0.0f);
    glScalef(-1.0f, 1.0f, 1.0f);       // looking along +Z mirrors X; undo so +X = screen right
}

// ---- static vertex data -------------------------------------------------------
static const int RAIL_MAX = 160;
static GLfloat gRail2D[RAIL_MAX * 2];      // x,y per vertex
static int     gRailCount = 0;
static GLfloat gGrid3D[64 * 3];            // x,y,z per vertex
static int     gGridCount = 0;

// unit cuboid: 24 face-local records (positions) + per-face shade, 36 indices
static const GLfloat gCubePos[24 * 3] = {
    // +X
     .5f,-.5f,-.5f,  .5f, .5f,-.5f,  .5f, .5f, .5f,  .5f,-.5f, .5f,
    // -X
    -.5f,-.5f, .5f, -.5f, .5f, .5f, -.5f, .5f,-.5f, -.5f,-.5f,-.5f,
    // +Y (top)
    -.5f, .5f,-.5f, -.5f, .5f, .5f,  .5f, .5f, .5f,  .5f, .5f,-.5f,
    // -Y (bottom)
    -.5f,-.5f, .5f, -.5f,-.5f,-.5f,  .5f,-.5f,-.5f,  .5f,-.5f, .5f,
    // +Z (back)
     .5f,-.5f, .5f,  .5f, .5f, .5f, -.5f, .5f, .5f, -.5f,-.5f, .5f,
    // -Z (front, faces the camera)
    -.5f,-.5f,-.5f, -.5f, .5f,-.5f,  .5f, .5f,-.5f,  .5f,-.5f,-.5f
};
static const GLfloat gFaceShade[6] = { 0.80f, 0.62f, 1.00f, 0.42f, 0.68f, 0.90f };
static GLushort gCubeIdx[36];
static GLfloat  gCubeCol[24 * 3];

static void buildStaticGeometry() {
    // shelf rails + dividers of the 2D machine glass (GL_LINES)
    gRailCount = 0;
    const float shelfY[3] = { 80.0f, 110.0f, 140.0f };
    for (int s = 0; s < 3; ++s) {
        for (int k = 0; k < 2; ++k) {                    // two lines per shelf: lip + underside
            float y = shelfY[s] - k * 1.5f;
            gRail2D[gRailCount * 2] = R_GLASS.x0; gRail2D[gRailCount * 2 + 1] = y; gRailCount++;
            gRail2D[gRailCount * 2] = R_GLASS.x1; gRail2D[gRailCount * 2 + 1] = y; gRailCount++;
        }
        for (int d = 0; d <= 4; ++d) {                   // slot dividers
            float x = R_GLASS.x0 + d * 16.0f + 2.0f;
            gRail2D[gRailCount * 2] = x; gRail2D[gRailCount * 2 + 1] = shelfY[s]; gRailCount++;
            gRail2D[gRailCount * 2] = x; gRail2D[gRailCount * 2 + 1] = shelfY[s] + 22.0f; gRailCount++;
        }
    }
    // floor grid of the interior (GL_LINES): lines along X every 2 z, along Z every 2 x
    gGridCount = 0;
    for (int zi = 0; zi <= 4; ++zi) {
        float z = zi * 2.0f;
        gGrid3D[gGridCount * 3] = WX_MIN; gGrid3D[gGridCount * 3 + 1] = 0.02f; gGrid3D[gGridCount * 3 + 2] = z; gGridCount++;
        gGrid3D[gGridCount * 3] = WX_MAX; gGrid3D[gGridCount * 3 + 1] = 0.02f; gGrid3D[gGridCount * 3 + 2] = z; gGridCount++;
    }
    for (int xi = 0; xi <= 6; ++xi) {
        float x = WX_MIN + xi * 2.0f;
        gGrid3D[gGridCount * 3] = x; gGrid3D[gGridCount * 3 + 1] = 0.02f; gGrid3D[gGridCount * 3 + 2] = WZ_MIN; gGridCount++;
        gGrid3D[gGridCount * 3] = x; gGrid3D[gGridCount * 3 + 1] = 0.02f; gGrid3D[gGridCount * 3 + 2] = WZ_MAX; gGridCount++;
    }
    // cuboid indices: each face = 2 triangles over its own 4 records
    for (int f = 0; f < 6; ++f) {
        GLushort b = (GLushort)(f * 4), *p = &gCubeIdx[f * 6];
        p[0] = b; p[1] = b + 1; p[2] = b + 2;
        p[3] = b; p[4] = b + 2; p[5] = b + 3;
    }
}

static void drawRails2D(Col c) {
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, gRail2D);
    setCol(c); glLineWidth(2.0f);
    glDrawArrays(GL_LINES, 0, gRailCount);
    glDisableClientState(GL_VERTEX_ARRAY);
    glLineWidth(1.0f);
}

static void drawGrid3D(Col c) {
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, gGrid3D);
    setCol(c);
    glDrawArrays(GL_LINES, 0, gGridCount);
    glDisableClientState(GL_VERTEX_ARRAY);
}

// Indexed cuboid, flat face shading baked into a per-vertex color array.
static void drawCuboid(float cx, float cy, float cz, float sx, float sy, float sz, Col c) {
    for (int i = 0; i < 24; ++i) {
        float sh = gFaceShade[i / 4];
        gCubeCol[i * 3] = c.r * sh; gCubeCol[i * 3 + 1] = c.g * sh; gCubeCol[i * 3 + 2] = c.b * sh;
    }
    glPushMatrix();
    glTranslatef(cx, cy, cz); glScalef(sx, sy, sz);
    glEnableClientState(GL_VERTEX_ARRAY); glEnableClientState(GL_COLOR_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, gCubePos);
    glColorPointer(3, GL_FLOAT, 0, gCubeCol);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, gCubeIdx);
    glDisableClientState(GL_COLOR_ARRAY); glDisableClientState(GL_VERTEX_ARRAY);
    glPopMatrix();
}


/*==============================================================================
  6. 2D ENVIRONMENT AND CHARACTER RENDERING

  HOW IT WORKS
      Everything is drawn back-to-front in logical 320x180 space: sky, stars,
      building silhouettes with window grids (loops + a tiny hash so the lit
      pattern is fixed), pavement, streetlamps, the glowing machine, the
      customer beside it. The look is "pixel-style": grid-aligned rectangles,
      stepped outlines, few colours. Renderers only read state. Machine cues
      (display dip, indicator, lamp trio) are read from computeCues().
      The reflection in the glass is a faint, customer-coloured silhouette
      while the customer stands there; in the ending it lingers, turns toward
      the viewer and vanishes in a light flicker.
==============================================================================*/
static bool windowLit(int i, int j) { return ((i * 7 + j * 13 + i * j) % 5) < 2; }

static void drawSky() {
    glBegin(GL_QUADS);
    setCol(PAL_SKY_LOW); glVertex2f(0, 36); glVertex2f(LW, 36);
    setCol(PAL_SKY_TOP); glVertex2f(LW, LH); glVertex2f(0, LH);
    glEnd();
    // stars (GL_POINTS) with a slow ambient twinkle
    glPointSize(gVP.scale > 2.0f ? 2.0f : 1.0f);
    glBegin(GL_POINTS);
    for (int i = 0; i < 40; ++i) {
        float x = fmodf(i * 53.0f + 11.0f, LW), y = 110.0f + fmodf(i * 31.0f, 66.0f);
        float tw = 0.55f + 0.45f * sinf(K.titleT * 1.3f + i);
        setCol(PAL_CREAM, tw); glVertex2f(x, y);
    }
    glEnd();
    // moon: stepped pixel disc
    setCol(PAL_CREAM, 0.9f); fillRect(268, 150, 284, 166); fillRect(264, 154, 288, 162); fillRect(272, 146, 280, 170);
}

static void drawBuilding(float x, float w, float h, Col c, int seed) {
    setCol(c); fillRect(x, 36, x + w, 36 + h);
    setCol(scaleCol(c, 1.25f)); fillRect(x - 2, 36 + h, x + w + 2, 38 + h);       // stepped cornice
    for (int i = 0; (i + 1) * 9 < w - 4; ++i)
        for (int j = 0; (j + 1) * 11 < h - 8; ++j) {
            bool on = windowLit(i + seed, j);
            setCol(on ? PAL_WINDOW_ON : PAL_WINDOW_OFF, on ? 0.9f : 1.0f);
            float wx = x + 4 + i * 9, wy = 40 + j * 11;
            fillRect(wx, wy, wx + 5, wy + 6);
        }
}

static void drawPavement() {
    setCol(PAL_PAVEMENT); fillRect(0, 0, LW, 36);
    setCol(PAL_CURB);     fillRect(0, 33, LW, 36);
    setCol(scaleCol(PAL_PAVEMENT, 0.75f));
    for (int i = 0; i < 20; ++i) fillRect(i * 16.0f, 0, i * 16.0f + 1, 33);       // slab seams
}

static void drawStreetlamp(float x, float scale) {
    setCol(PAL_INK); fillRect(x - 1, 36, x + 1, 88); fillRect(x - 1, 86, x + 9, 88); fillRect(x + 7, 80, x + 10, 87);
    setCol(PAL_AMBER, 1.0f); fillRect(x + 6, 77, x + 11, 80);
    // light pool (translucent fan)
    glBegin(GL_TRIANGLE_FAN);
    setCol(PAL_AMBER, 0.30f * scale); glVertex2f(x + 8, 78);
    setCol(PAL_AMBER, 0.0f);
    for (int i = 0; i <= 16; ++i) { float a = 2 * PI_F * i / 16; glVertex2f(x + 8 + cosf(a) * 36, 78 + sinf(a) * 38); }
    glEnd();
}

static void drawStreet2D(float lampScale) {
    drawSky();
    const float bx[8] = { 0, 40, 78, 112, 176, 214, 252, 290 };
    const float bw[8] = { 42, 40, 36, 70, 40, 40, 40, 34 };
    const float bh[8] = { 78, 112, 70, 90, 104, 74, 118, 86 };
    for (int i = 0; i < 8; ++i) drawBuilding(bx[i], bw[i], bh[i], (i & 1) ? PAL_BUILDING2 : PAL_BUILDING, i * 3);
    drawPavement();
    drawStreetlamp(14, lampScale);
    drawStreetlamp(296, lampScale);
}

static void drawSnack2D(int type, float x, float y, float w, float h) {
    switch (type) {
    case SNACK_CHIPS:
        setCol(PAL_CHIPS); fillRect(x, y, x + w, y + h);
        setCol(scaleCol(PAL_CHIPS, 0.7f));                                      // crimped top (triangles)
        for (int i = 0; i < 3; ++i) fillTri(x + i * w / 3, y + h, x + (i + 1) * w / 3, y + h, x + (i + 0.5f) * w / 3, y + h + 2);
        setCol(PAL_GOLD); fillRect(x + 2, y + h * 0.35f, x + w - 2, y + h * 0.7f);
        break;
    case SNACK_CHOC:
        setCol(PAL_CHOC); fillRect(x, y + h * 0.15f, x + w, y + h * 0.65f);
        setCol(PAL_GOLD); fillRect(x, y + h * 0.35f, x + w, y + h * 0.45f);
        setCol(scaleCol(PAL_CHOC, 0.75f));
        fillTri(x, y + h * 0.15f, x, y + h * 0.65f, x - 2, y + h * 0.4f);
        fillTri(x + w, y + h * 0.15f, x + w, y + h * 0.65f, x + w + 2, y + h * 0.4f);
        break;
    default:
        setCol(PAL_JUICE); fillRect(x + 1, y, x + w - 1, y + h * 0.8f);
        setCol(PAL_WHITE); fillRect(x + 3, y + h * 0.2f, x + w - 3, y + h * 0.5f);
        setCol(scaleCol(PAL_JUICE, 0.8f)); fillTri(x + 1, y + h * 0.8f, x + w - 1, y + h * 0.8f, x + w * 0.5f, y + h);
        setCol(PAL_INK); fillRect(x + w * 0.5f - 1, y + h, x + w * 0.5f + 1, y + h + 2);
        break;
    }
}

static void drawLampTrio(float x, float y, float glow, float flick) {
    for (int i = 0; i < 3; ++i) {
        float on = clampf(0.18f + glow * 0.9f, 0, 1) * flick;
        setCol(mixCol(PAL_PANEL, PAL_AMBER, on)); fillRect(x + i * 8.0f, y, x + i * 8.0f + 5, y + 4);
    }
}

static int jamSlot() { return (stage().target - 1 + 3) % 3; }   // shelf 1, slot whose type == target

static void drawMachine2D(const Cues& cu, bool targetJammed, float jamT) {
    float flick = outroFlicker();
    float lit = (1.0f - cu.dim) * flick;
    // warm halo around the machine
    for (int i = 5; i >= 1; --i) {
        setCol(PAL_AMBER, 0.045f * lit);
        fillRect(R_CABINET.x0 - i * 6, R_CABINET.y0 - i * 3, R_CABINET.x1 + i * 6, R_CABINET.y1 + i * 3);
    }
    // cabinet + trim
    setCol(PAL_MACHINE_D); fillRect(R_CABINET.x0 - 3, R_CABINET.y0, R_CABINET.x1 + 3, R_CABINET.y1);
    setCol(PAL_MACHINE);   fillRect(R_CABINET.x0, R_CABINET.y0, R_CABINET.x1, R_CABINET.y1);
    setCol(scaleCol(PAL_AMBER, lit)); fillRect(R_CABINET.x0, R_CABINET.y1 - 6, R_CABINET.x1, R_CABINET.y1 - 4);
    setCol(PAL_INK); fillRect(R_CABINET.x0, R_CABINET.y0 - 3, R_CABINET.x1, R_CABINET.y0);   // base shadow

    // glass: lit cream interior
    setCol(scaleCol(PAL_GLASS, 0.55f + 0.45f * lit)); fillRect(R_GLASS.x0, R_GLASS.y0, R_GLASS.x1, R_GLASS.y1);
    // snacks on three shelves (loops)
    const float shelfY[3] = { 80.0f, 110.0f, 140.0f };
    for (int s = 0; s < 3; ++s)
        for (int k = 0; k < 4; ++k) {
            int type = (k + s) % 3;
            if (targetJammed && s == 1 && k == jamSlot()) continue;     // this one is in transit
            drawSnack2D(type, R_GLASS.x0 + 4 + k * 16.0f, shelfY[s] + 1, 11, type == SNACK_CHOC ? 12 : 17);
        }
    setCol(scaleCol(PAL_MACHINE_D, 1.0f)); drawRails2D(scaleCol(PAL_MACHINE_D, 1.0f));    // vertex-array rails
    if (targetJammed) {                                                                     // the jam: tilted snack stuck mid-fall
        float shake = sinf(jamT * 60.0f) * 0.8f * (jamT < T_JAM + T_WARN ? 1.0f : 0.0f);
        glPushMatrix(); glTranslatef(R_GLASS.x0 + 8 + jamSlot() * 16.0f + shake, 97.0f, 0); glRotatef(-14.0f, 0, 0, 1);
        drawSnack2D(stage().target, 0, 0, 12, stage().target == SNACK_CHOC ? 12 : 17);
        glPopMatrix();
    }
    // glass glare
    setCol(PAL_WHITE, 0.10f);
    fillTri(R_GLASS.x0, R_GLASS.y1, R_GLASS.x0 + 30, R_GLASS.y1, R_GLASS.x0, R_GLASS.y1 - 36);
    setCol(PAL_WHITE, 0.06f); fillRect(R_GLASS.x0 + 38, R_GLASS.y0 + 6, R_GLASS.x0 + 42, R_GLASS.y1 - 6);
    setCol(PAL_INK); glLineWidth(2.0f); strokeRect(R_GLASS.x0, R_GLASS.y0, R_GLASS.x1, R_GLASS.y1); glLineWidth(1.0f);

    // control panel
    fillRectC(R_PANEL, PAL_PANEL);
    setCol(PAL_AMBER, 0.6f); strokeRect(R_PANEL.x0, R_PANEL.y0, R_PANEL.x1, R_PANEL.y1);
    drawLampTrio(R_PANEL.x0 + 4, R_PANEL.y1 - 7, cu.lamps, flick);
    // maintenance indicator (cues)
    setCol(mixCol(PAL_PANEL, PAL_WARN, 0.15f + 0.85f * cu.indicator)); fillRect(R_PANEL.x1 - 8, R_PANEL.y1 - 7, R_PANEL.x1 - 4, R_PANEL.y1 - 3);
    // display
    float dd = 1.0f - cu.dip;
    setCol(scaleCol(PAL_INK, 1.0f)); fillRect(R_DISPLAY.x0, R_DISPLAY.y0, R_DISPLAY.x1, R_DISPLAY.y1);
    const char* l1 = G.hasCoin ? "SELECT" : "INSERT";
    const char* l2 = G.hasCoin ? "ITEM" : "COIN";
    if (G.state == MACHINE_2D || G.state == TITLE || G.state == CUSTOMER_INTRO || G.state == REFLECTION_OUTRO) {
        Col dc = scaleCol(PAL_AMBER, dd * flick);
        float dw = R_DISPLAY.x1 - R_DISPLAY.x0 - 2.0f;        // draw only what fits the display
        if (textW(l1, fontSmall()) <= dw) drawTextCentered((R_DISPLAY.x0 + R_DISPLAY.x1) * 0.5f, R_DISPLAY.y1 - 6, l1, dc, fontSmall());
        if (textW(l2, fontSmall()) <= dw) drawTextCentered((R_DISPLAY.x0 + R_DISPLAY.x1) * 0.5f, R_DISPLAY.y0 + 2, l2, dc, fontSmall());
    }
    // coin slot
    setCol(G.hasCoin ? PAL_GOLD : PAL_INK); fillRect(R_COIN.x0, R_COIN.y0, R_COIN.x1, R_COIN.y1);
    setCol(PAL_AMBER); strokeRect(R_COIN.x0, R_COIN.y0, R_COIN.x1, R_COIN.y1);
    setCol(PAL_INK); fillRect(R_COIN.x0 + 7, R_COIN.y0 + 3, R_COIN.x0 + 11, R_COIN.y1 - 3);
    // selection buttons (+ small lamps used by the stage-2 pulse cue)
    for (int i = 0; i < 3; ++i) {
        setCol(mixCol(PAL_MACHINE_D, PAL_CREAM, 0.15f + (G.hasCoin ? 0.12f : 0.0f))); fillRect(R_BTN[i].x0, R_BTN[i].y0, R_BTN[i].x1, R_BTN[i].y1);
        setCol(PAL_AMBER, 0.8f); strokeRect(R_BTN[i].x0, R_BTN[i].y0, R_BTN[i].x1, R_BTN[i].y1);
        float pl = clampf((G.hasCoin ? 0.35f : 0.12f) + cu.selPulse * (i == 1 ? 1.0f : (i == 0 ? 0.6f : 0.8f)), 0, 1);
        setCol(mixCol(PAL_PANEL, PAL_AMBER, pl)); fillRect(R_BTN[i].x1 - 4, R_BTN[i].y1 - 4, R_BTN[i].x1 - 2, R_BTN[i].y1 - 2);
        static const char* numLbl[3] = { "1", "2", "3" };
        const char* bl = textW(SNACK_BTN[i], fontSmall()) <= (R_BTN[i].x1 - R_BTN[i].x0) - 6.0f ? SNACK_BTN[i] : numLbl[i];
        drawText(R_BTN[i].x0 + 2, R_BTN[i].y0 + 3, bl, PAL_CREAM, fontSmall());
    }
    // delivery chute flap
    setCol(PAL_INK); fillRect(R_CHUTE2D.x0, R_CHUTE2D.y0, R_CHUTE2D.x1, R_CHUTE2D.y1);
    setCol(PAL_AMBER, 0.7f); strokeRect(R_CHUTE2D.x0, R_CHUTE2D.y0, R_CHUTE2D.x1, R_CHUTE2D.y1);
    setCol(scaleCol(PAL_MACHINE, 0.8f)); fillRect(R_CHUTE2D.x0 + 3, R_CHUTE2D.y1 - 5, R_CHUTE2D.x1 - 3, R_CHUTE2D.y1 - 2);
}

// ---- customers -------------------------------------------------------------
static void drawCustomerBody(float x, float y, int st, float sway) {
    const StageData& S = STAGES[st];
    float w = 1.0f;
    (void)w;
    // legs
    setCol(PAL_INK); fillRect(x - 9, y, x - 1, y + 24); fillRect(x + 1, y, x + 9, y + 24);
    setCol(scaleCol(S.accent, 0.8f)); fillRect(x - 10, y, x - 1, y + 3); fillRect(x + 1, y, x + 10, y + 3);
    // torso
    if (st == 0) { setCol(PAL_INK); fillRect(x + 7, y + 24, x + 17, y + 44); }      // backpack
    setCol(S.shirt); fillRect(x - 11, y + 22 + sway, x + 11, y + 46 + sway);
    // arms
    setCol(scaleCol(S.shirt, 0.85f)); fillRect(x - 15, y + 24 + sway, x - 11, y + 44 + sway); fillRect(x + 11, y + 24 + sway, x + 15, y + 44 + sway);
    setCol(PAL_SKIN); fillRect(x - 15, y + 22 + sway, x - 11, y + 25 + sway); fillRect(x + 11, y + 22 + sway, x + 15, y + 25 + sway);
    if (st == 1) {                                                                    // collar + tie
        setCol(PAL_WHITE); fillTri(x - 5, y + 46 + sway, x + 5, y + 46 + sway, x, y + 36 + sway);
        setCol(S.accent);  fillTri(x - 2, y + 44 + sway, x + 2, y + 44 + sway, x, y + 28 + sway);
    }
    // head
    setCol(PAL_SKIN); fillEllipse(x, y + 54 + sway, 8, 9);
    setCol(PAL_INK);
    if (st == 0) fillRect(x - 9, y + 56 + sway, x + 9, y + 63 + sway);                // messy hair
    else if (st == 1) fillRect(x - 8, y + 58 + sway, x + 8, y + 63 + sway);           // neat hair
    else fillRect(x - 8, y + 60 + sway, x + 8, y + 63 + sway);
    fillRect(x - 4, y + 55 + sway, x - 2, y + 57 + sway); fillRect(x + 2, y + 55 + sway, x + 4, y + 57 + sway);   // eyes
    setCol(PAL_INK); glLineWidth(2.0f);
    glBegin(GL_LINE_STRIP);                                                            // mouth
    if (st == 2) { glVertex2f(x - 6, y + 50 + sway); glVertex2f(x - 3, y + 48 + sway); glVertex2f(x + 3, y + 48 + sway); glVertex2f(x + 6, y + 50 + sway); }
    else         { glVertex2f(x - 3, y + 49 + sway); glVertex2f(x + 3, y + 49 + sway); }
    glEnd(); glLineWidth(1.0f);
}

static void drawCustomer(float x, float baseY, int st) {
    float sway = sinf(K.titleT * 1.6f) * 0.5f;
    drawCustomerBody(x, baseY, st, sway);
}

// The reflection in the glass. alpha: visibility; turn: 0 profile .. 1 facing us.
static void drawReflection(int st, float alpha, float turn, float lightMul) {
    if (alpha <= 0.001f) return;
    const StageData& S = STAGES[st];
    float cx = (R_GLASS.x0 + R_GLASS.x1) * 0.5f + 4.0f, base = R_GLASS.y0 + 2.0f;
    Col body = mixCol(S.shirt, PAL_INK, 0.55f);
    float a = alpha * lightMul;
    float w = lerpf(0.55f, 1.0f, turn);                      // body rotates toward the viewer
    setCol(body, a);
    fillRect(cx - 9 * w, base, cx + 9 * w, base + 20);                                // legs
    fillRect(cx - 11 * w, base + 20, cx + 11 * w, base + 44);                         // torso
    setCol(mixCol(PAL_SKIN, PAL_INK, 0.5f), a);
    fillEllipse(cx + (1.0f - turn) * 2.5f, base + 52, 8 * w, 9);                      // head
    if (turn > 0.45f) {                                                               // eyes appear only once turned
        setCol(PAL_CREAM, a * smooth01((turn - 0.45f) / 0.4f));
        fillRect(cx - 4, base + 53, cx - 2, base + 55); fillRect(cx + 2, base + 53, cx + 4, base + 55);
    }
}

// reflection parameters for the current state (also the faint ordinary one)
static void drawReflectionForState() {
    int st = G.stageIndex;
    float lm = 1.0f;
    if (G.state == REFLECTION_OUTRO) {
        float p = K.stateT;
        switch (G.outroPhase) {
        case OUT_EXIT:    drawReflection(st, 0.14f, 0.0f, 1.0f); break;
        case OUT_LINGER:  drawReflection(st, lerpf(0.14f, 0.55f, smooth01(p / OUTRO_DUR[OUT_LINGER])), 0.0f, 1.0f); break;
        case OUT_TURN:    drawReflection(st, 0.55f, smooth01(p / OUTRO_DUR[OUT_TURN]), 1.0f); break;
        case OUT_FLICKER: {
            float u = p / OUTRO_DUR[OUT_FLICKER];
            bool vis = (u < 0.15f) || (u >= 0.30f && u < 0.50f);                     // gone for good after the flicker
            if (vis) drawReflection(st, 0.55f, 1.0f, lm);
            break; }
        }
    } else if (G.state == CUSTOMER_INTRO || G.state == MACHINE_2D || G.state == STAGE_RESULT) {
        drawReflection(st, 0.14f, 0.0f, 1.0f);                                        // ordinary, faint, unremarkable
    }
}


/*==============================================================================
  7. 3D ENVIRONMENT AND CLAW RENDERING

  HOW IT WORKS
      The interior is built from the indexed cuboid mesh (section 5) plus
      triangles (claw prongs, carton roofs), quads (floor, chute) and lines
      (cable, grid, marker, outlines). Flat face shading + the cool blue-green
      palette give the chunky low-poly look. Oversized pipes, columns, racks
      and bolts OUTSIDE the play area (x beyond +/-6.4, z beyond 8.4) imply a
      machine far bigger than the bounded rescue area; they add no routes.
      The camera is fixed (see begin3D). A bright yellow marker is drawn on the
      floor under the claw (X/Z) with a plumb line up to the claw; the target
      gets a pulsing outline that brightens when it is within grab range. The
      chute has a warm lit pad whose rim is EXACTLY the acceptance rectangle.
==============================================================================*/
static void drawSnack3D(int type, float x, float yc, float z, float tint = 1.0f) {
    float w = SNACK_W[type], h = SNACK_H[type], d = SNACK_D[type];
    switch (type) {
    case SNACK_CHIPS: {
        drawCuboid(x, yc, z, w, h, d, scaleCol(PAL_CHIPS, tint));
        drawCuboid(x, yc + h * 0.5f, z, w * 1.06f, 0.14f, d * 1.1f, scaleCol(PAL_CHIPS, 0.65f * tint));   // crimp top
        drawCuboid(x, yc - h * 0.5f + 0.07f, z, w * 1.06f, 0.14f, d * 1.1f, scaleCol(PAL_CHIPS, 0.65f * tint));
        drawCuboid(x, yc, z - d * 0.5f - 0.01f, w * 0.7f, h * 0.35f, 0.04f, scaleCol(PAL_GOLD, tint));    // label
        break; }
    case SNACK_CHOC: {
        drawCuboid(x, yc, z, w, h, d, scaleCol(PAL_CHOC, tint));
        drawCuboid(x, yc, z - d * 0.5f - 0.01f, w, h * 0.3f, 0.04f, scaleCol(PAL_GOLD, tint));             // wrapper band
        glBegin(GL_TRIANGLES);                                                                              // wrapper ends
        setCol(scaleCol(PAL_CHOC, 0.7f * tint));
        glVertex3f(x - w * 0.5f, yc - h * 0.5f, z); glVertex3f(x - w * 0.5f, yc + h * 0.5f, z); glVertex3f(x - w * 0.5f - 0.25f, yc, z);
        glVertex3f(x + w * 0.5f, yc - h * 0.5f, z); glVertex3f(x + w * 0.5f, yc + h * 0.5f, z); glVertex3f(x + w * 0.5f + 0.25f, yc, z);
        glEnd();
        break; }
    default: {
        drawCuboid(x, yc, z, w, h, d, scaleCol(PAL_JUICE, tint));
        drawCuboid(x, yc, z - d * 0.5f - 0.01f, w * 0.7f, h * 0.35f, 0.04f, scaleCol(PAL_WHITE, tint));   // label
        glBegin(GL_TRIANGLES);                                                                              // gable top (triangles)
        setCol(scaleCol(PAL_JUICE, 0.8f * tint));
        glVertex3f(x - w * 0.5f, yc + h * 0.5f, z - d * 0.5f); glVertex3f(x + w * 0.5f, yc + h * 0.5f, z - d * 0.5f); glVertex3f(x, yc + h * 0.5f + 0.35f, z);
        glVertex3f(x - w * 0.5f, yc + h * 0.5f, z + d * 0.5f); glVertex3f(x + w * 0.5f, yc + h * 0.5f, z + d * 0.5f); glVertex3f(x, yc + h * 0.5f + 0.35f, z);
        glEnd();
        drawCuboid(x, yc + h * 0.5f + 0.42f, z, 0.22f, 0.14f, 0.22f, scaleCol(PAL_INK, 1.5f));             // cap
        break; }
    }
}

static void outlineBox3D(float x, float yc, float z, float sx, float sy, float sz) {
    float x0 = x - sx, x1 = x + sx, y0 = yc - sy, y1 = yc + sy, z0 = z - sz, z1 = z + sz;
    glBegin(GL_LINE_LOOP); glVertex3f(x0, y0, z0); glVertex3f(x1, y0, z0); glVertex3f(x1, y0, z1); glVertex3f(x0, y0, z1); glEnd();
    glBegin(GL_LINE_LOOP); glVertex3f(x0, y1, z0); glVertex3f(x1, y1, z0); glVertex3f(x1, y1, z1); glVertex3f(x0, y1, z1); glEnd();
    glBegin(GL_LINES);
    glVertex3f(x0, y0, z0); glVertex3f(x0, y1, z0); glVertex3f(x1, y0, z0); glVertex3f(x1, y1, z0);
    glVertex3f(x1, y0, z1); glVertex3f(x1, y1, z1); glVertex3f(x0, y0, z1); glVertex3f(x0, y1, z1);
    glEnd();
}

static void drawInteriorShell() {
    // floor: alternating stripes (quads) + grid lines (vertex array)
    for (int i = 0; i < 8; ++i) {
        setCol((i & 1) ? PAL_TEAL : scaleCol(PAL_TEAL, 0.78f));
        glBegin(GL_QUADS);
        glVertex3f(WX_MIN, 0, i); glVertex3f(WX_MAX, 0, i); glVertex3f(WX_MAX, 0, i + 1); glVertex3f(WX_MIN, 0, i + 1);
        glEnd();
    }
    drawGrid3D(scaleCol(PAL_TEAL_L, 0.8f));
    // back + side walls
    drawCuboid(0, 4.5f, 8.6f, 14.4f, 9.0f, 0.4f, PAL_TEAL_D);
    drawCuboid(-6.5f, 4.5f, 4.3f, 0.4f, 9.0f, 9.0f, scaleCol(PAL_TEAL_D, 1.2f));
    drawCuboid( 6.5f, 4.5f, 4.3f, 0.4f, 9.0f, 9.0f, scaleCol(PAL_TEAL_D, 1.2f));
    drawCuboid(0, 0.25f, -0.35f, 13.4f, 0.5f, 0.3f, PAL_STEEL);                       // front lip
    // oversized pipes and vents on the back wall
    for (int i = 0; i < 3; ++i) drawCuboid(0, 1.6f + i * 2.6f, 8.1f, 13.0f, 0.55f, 0.55f, scaleCol(PAL_STEEL, 0.55f + 0.1f * i));
    for (int i = 0; i < 6; ++i) drawCuboid(-5.0f + i * 2.0f, 4.0f, 8.15f, 0.35f, 7.0f, 0.2f, scaleCol(PAL_TEAL, 0.9f));
    // bolts
    for (int i = 0; i < 7; ++i) for (int j = 0; j < 3; ++j)
        drawCuboid(-6.0f + i * 2.0f, 1.0f + j * 3.0f, 7.9f, 0.22f, 0.22f, 0.15f, PAL_STEEL);
    // giant structures beyond the walls (scale cues, not part of play)
    for (int i = 0; i < 4; ++i) {
        float h = 11.0f + (i % 2) * 5.0f;
        drawCuboid(-9.5f, h * 0.5f, 1.0f + i * 4.0f, 1.8f, h, 1.8f, scaleCol(PAL_TEAL, 0.55f));
        drawCuboid( 9.5f, h * 0.5f, 1.0f + i * 4.0f, 1.8f, h, 1.8f, scaleCol(PAL_TEAL, 0.55f));
    }
    for (int i = 0; i < 7; ++i) {                                                       // distant snack racks
        float h = 5.0f + (i * 5 % 4) * 2.2f;
        drawCuboid(-10.0f + i * 3.4f, h * 0.5f, 13.0f, 3.0f, h, 2.0f, scaleCol(PAL_TEAL, 0.45f + 0.05f * (i % 3)));
        drawCuboid(-10.0f + i * 3.4f, h + 0.3f, 12.0f, 0.5f, 0.5f, 0.2f, PAL_AMBER);   // warm accent lamp
    }
    // gantry rails along Z and warm lamps
    drawCuboid(-6.2f, 7.7f, 4.0f, 0.4f, 0.4f, 8.8f, PAL_STEEL);
    drawCuboid( 6.2f, 7.7f, 4.0f, 0.4f, 0.4f, 8.8f, PAL_STEEL);
    for (int i = 0; i < 3; ++i) { drawCuboid(-6.2f, 7.2f, 1.0f + i * 3.0f, 0.3f, 0.3f, 0.3f, PAL_AMBER); drawCuboid(6.2f, 7.2f, 1.0f + i * 3.0f, 0.3f, 0.3f, 0.3f, PAL_AMBER); }
}

static void drawChute3D(float pulse) {
    // warm lit pad: quad exactly over the acceptance rectangle
    float y = 0.04f;
    glBegin(GL_QUADS);
    setCol(scaleCol(PAL_AMBER, 0.55f + 0.2f * pulse)); glVertex3f(CHUTE_X0, y, CHUTE_Z0); glVertex3f(CHUTE_X1, y, CHUTE_Z0);
    setCol(scaleCol(PAL_AMBER, 0.85f));                glVertex3f(CHUTE_X1, y, CHUTE_Z1); glVertex3f(CHUTE_X0, y, CHUTE_Z1);
    glEnd();
    glLineWidth(3.0f); setCol(PAL_CREAM);
    glBegin(GL_LINE_LOOP); glVertex3f(CHUTE_X0, 0.06f, CHUTE_Z0); glVertex3f(CHUTE_X1, 0.06f, CHUTE_Z0); glVertex3f(CHUTE_X1, 0.06f, CHUTE_Z1); glVertex3f(CHUTE_X0, 0.06f, CHUTE_Z1); glEnd();
    glLineWidth(1.0f);
    // light shafts + guide chevrons (decor only, same footprint)
    setCol(PAL_AMBER, 1.0f);
    glBegin(GL_LINES);
    glVertex3f(CHUTE_X0, 0.06f, CHUTE_Z1); glVertex3f(CHUTE_X0, 1.6f, CHUTE_Z1);
    glVertex3f(CHUTE_X1, 0.06f, CHUTE_Z1); glVertex3f(CHUTE_X1, 1.6f, CHUTE_Z1);
    glEnd();
    float cx = (CHUTE_X0 + CHUTE_X1) * 0.5f, cz = (CHUTE_Z0 + CHUTE_Z1) * 0.5f;
    setCol(PAL_INK);
    glBegin(GL_TRIANGLES); glVertex3f(cx - 0.5f, 0.07f, cz + 0.4f); glVertex3f(cx + 0.5f, 0.07f, cz + 0.4f); glVertex3f(cx, 0.07f, cz - 0.5f); glEnd();
    drawCuboid(cx, 0.35f, CHUTE_Z0 - 0.15f, CHUTE_X1 - CHUTE_X0 + 0.4f, 0.7f, 0.2f, PAL_STEEL);    // frame at the front
}

static void drawArm3D() {
    if (!stage().hasArm) return;
    float x = K.armX;
    drawCuboid(x, 1.2f, ARM_Z, ARM_HX * 2.0f, 1.2f, ARM_HZ * 2.0f, scaleCol(PAL_WARN, 0.85f));   // sweeping block
    drawCuboid(x, 0.25f, ARM_Z, ARM_HX * 2.0f + 0.3f, 0.5f, ARM_HZ * 2.0f + 0.3f, PAL_STEEL);
    drawCuboid(x, 3.4f, ARM_Z, 0.5f, 3.6f, 0.5f, PAL_STEEL);                                      // support column
    drawCuboid(x, 5.4f, ARM_Z, 0.9f, 0.5f, 0.9f, PAL_AMBER);
    // hazard stripes on the block (quads)
    for (int i = 0; i < 4; ++i) {
        setCol(i & 1 ? PAL_INK : PAL_GOLD);
        float sx = x - ARM_HX + 0.1f + i * (ARM_HX * 2.0f - 0.2f) / 4.0f, sw = (ARM_HX * 2.0f - 0.2f) / 4.0f;
        glBegin(GL_QUADS);
        glVertex3f(sx, 0.2f, ARM_Z - ARM_HZ - 0.01f); glVertex3f(sx + sw, 0.2f, ARM_Z - ARM_HZ - 0.01f);
        glVertex3f(sx + sw, 0.9f, ARM_Z - ARM_HZ - 0.01f); glVertex3f(sx, 0.9f, ARM_Z - ARM_HZ - 0.01f);
        glEnd();
    }
}

static void drawClaw3D() {
    // trolley on a cross-beam that follows the claw's depth
    drawCuboid(0.0f, 7.7f, C.z, 12.8f, 0.4f, 0.5f, scaleCol(PAL_STEEL, 0.9f));
    drawCuboid(C.x, 7.5f, C.z, 1.2f, 0.5f, 1.0f, PAL_AMBER);
    // cable (line)
    glLineWidth(2.0f); setCol(PAL_STEEL);
    glBegin(GL_LINES); glVertex3f(C.x, 7.3f, C.z); glVertex3f(C.x, C.y + 0.35f, C.z); glEnd();
    glLineWidth(1.0f);
    // claw head
    drawCuboid(C.x, C.y + 0.2f, C.z, 0.55f, 0.35f, 0.55f, PAL_STEEL);
    bool closed = C.carrying;
    float spread = closed ? 0.28f : 0.55f;
    glBegin(GL_TRIANGLES);                                                         // three triangular prongs
    for (int i = 0; i < 3; ++i) {
        float a = i * 2.0f * PI_F / 3.0f + PI_F * 0.5f, ca = cosf(a), sa = sinf(a);
        float bx = C.x + ca * 0.22f, bz = C.z + sa * 0.22f;
        float tx = C.x + ca * spread, tz = C.z + sa * spread;
        float px = -sa * 0.14f, pz = ca * 0.14f;
        setCol(scaleCol(PAL_STEEL, 1.15f));
        glVertex3f(bx + px, C.y + 0.05f, bz + pz); glVertex3f(bx - px, C.y + 0.05f, bz - pz); glVertex3f(tx, C.y - 0.5f, tz);
    }
    glEnd();
}

static void drawMarker3D(bool inRange, float t) {
    float pulse = 0.5f + 0.5f * sinf(t * 6.0f);
    float y = 0.05f, s = 0.55f;
    // plumb line from claw to floor
    setCol(PAL_MARKER, 0.9f); glLineWidth(1.0f);
    glBegin(GL_LINES); glVertex3f(C.x, y, C.z); glVertex3f(C.x, C.y, C.z); glEnd();
    // filled square + bold outline + cross: readable shape, not just colour
    setCol(PAL_MARKER);
    glBegin(GL_QUADS); glVertex3f(C.x - s * 0.5f, y, C.z - s * 0.5f); glVertex3f(C.x + s * 0.5f, y, C.z - s * 0.5f);
    glVertex3f(C.x + s * 0.5f, y, C.z + s * 0.5f); glVertex3f(C.x - s * 0.5f, y, C.z + s * 0.5f); glEnd();
    glLineWidth(3.0f); setCol(PAL_INK);
    glBegin(GL_LINE_LOOP); glVertex3f(C.x - s, y + 0.01f, C.z - s); glVertex3f(C.x + s, y + 0.01f, C.z - s);
    glVertex3f(C.x + s, y + 0.01f, C.z + s); glVertex3f(C.x - s, y + 0.01f, C.z + s); glEnd();
    glLineWidth(2.0f); setCol(inRange ? PAL_WHITE : PAL_MARKER);
    glBegin(GL_LINE_LOOP); glVertex3f(C.x - s, y + 0.02f, C.z - s); glVertex3f(C.x + s, y + 0.02f, C.z - s);
    glVertex3f(C.x + s, y + 0.02f, C.z + s); glVertex3f(C.x - s, y + 0.02f, C.z + s); glEnd();
    glBegin(GL_LINES);
    glVertex3f(C.x - s * 1.5f, y + 0.02f, C.z); glVertex3f(C.x + s * 1.5f, y + 0.02f, C.z);
    glVertex3f(C.x, y + 0.02f, C.z - s * 1.5f); glVertex3f(C.x, y + 0.02f, C.z + s * 1.5f);
    glEnd();
    glLineWidth(1.0f);
    (void)pulse;
}

static void drawScene3D() {
    const StageData& S = stage();
    drawInteriorShell();
    drawChute3D(0.5f + 0.5f * sinf(K.visT * 3.0f));
    // fixed packages (solid, same footprints as the collision boxes)
    for (int i = 0; i < S.obsCount; ++i) {
        int t = S.obs[i].type;
        drawSnack3D(t, S.obs[i].x, SNACK_H[t] * 0.5f, S.obs[i].z, 0.85f);
    }
    drawArm3D();
    // target (held, falling, or resting)
    int tt = S.target;
    bool inRange = (C.state == IDLE_EMPTY) && targetInGrabRange();
    if (C.carrying) {
        drawSnack3D(tt, C.x, C.y - 0.5f - SNACK_H[tt] * 0.5f, C.z);
    } else {
        float y = C.snackY;
        if (C.state == RESOLVING_DROP) {
            float u = clampf(C.dropT / DROP_TIME, 0.0f, 1.0f);
            y = lerpf(C.snackFallFromY, SNACK_H[tt] * 0.5f, u * u);
        }
        drawSnack3D(tt, C.snackX, y, C.snackZ);
        // target outline: pulses always, brighter when in grab range
        float p = 0.6f + 0.4f * sinf(K.visT * 5.0f);
        glLineWidth(inRange ? 3.0f : 2.0f);
        setCol(inRange ? PAL_WHITE : mixCol(PAL_MARKER, PAL_TEAL, 1.0f - p));
        if (C.state != RESOLVING_DROP) outlineBox3D(C.snackX, y, C.snackZ, SNACK_W[tt] * 0.5f + 0.18f, SNACK_H[tt] * 0.5f + 0.15f, SNACK_D[tt] * 0.5f + 0.18f);
        glLineWidth(1.0f);
    }
    drawMarker3D(inRange, K.visT);
    drawClaw3D();
}


/*==============================================================================
  8. HUD, MENUS AND TRANSITION RENDERING

  HOW IT WORKS
      The interface belongs to the world: a commercial-looking vending display
      for purchasing (credit / select / feedback, no rescue spoilers) and a
      restrained industrial "maintenance terminal" for the rescue. Pause shows
      the frozen, darkened scene under four options with an outline + brightness
      focus (not colour alone). Success says ITEM RECOVERED, failure says
      RECOVERY UNSUCCESSFUL; the ending shows no banner and no explanation.
      Transitions are timer driven: jam (shake) -> warning border -> camera
      shift into the 3D rescue. display() never advances any of these clocks.
==============================================================================*/
static void drawPanelBox(const Rect& r, Col fill, Col edge, float a = 0.92f) {
    setCol(fill, a); fillRect(r.x0, r.y0, r.x1, r.y1);
    setCol(edge);    strokeRect(r.x0, r.y0, r.x1, r.y1);
}

// ---- text fitting ---------------------------------------------------------
// Bitmap fonts are fixed in screen pixels, so when the window shrinks the same
// text covers more logical units. Everything below measures first, then wraps
// or switches to a short label instead of letting text escape its panel.
static float lineH(void* f) {
    float px = (f == GLUT_BITMAP_9_BY_15) ? 15.0f : (f == GLUT_BITMAP_HELVETICA_18 ? 18.0f : 13.0f);
    return fmaxf(8.0f, px / gVP.scale + 1.6f);
}

static const int WRAP_LINES = 8, WRAP_LEN = 96;
static int wrapText(const char* s, float maxW, void* font, char out[][WRAP_LEN]) {
    int n = 0; char cur[WRAP_LEN] = ""; const char* p = s;
    while (*p) {
        char word[WRAP_LEN]; int wl = 0;
        while (*p && *p != ' ' && wl < WRAP_LEN - 1) word[wl++] = *p++;
        word[wl] = 0; while (*p == ' ') ++p;
        char trial[2 * WRAP_LEN];
        if (cur[0]) snprintf(trial, sizeof(trial), "%s %s", cur, word); else snprintf(trial, sizeof(trial), "%s", word);
        if (cur[0] && textW(trial, font) > maxW) {
            if (n < WRAP_LINES) snprintf(out[n++], WRAP_LEN, "%s", cur);
            snprintf(cur, WRAP_LEN, "%s", word);
        } else snprintf(cur, WRAP_LEN, "%.95s", trial);
    }
    if (cur[0] && n < WRAP_LINES) snprintf(out[n++], WRAP_LEN, "%s", cur);
    return n;
}
// draws wrapped text whose first baseline is just below yTop; returns lines drawn
static int drawWrapped(float x, float yTop, float maxW, const char* s, Col c, void* font, bool center = false) {
    char ln[WRAP_LINES][WRAP_LEN];
    int n = wrapText(s, maxW, font, ln);
    float lh = lineH(font);
    for (int i = 0; i < n; ++i) {
        float y = yTop - 2.0f - lh * 0.75f - i * lh;
        if (center) drawTextCentered(x + maxW * 0.5f, y, ln[i], c, font); else drawText(x, y, ln[i], c, font);
    }
    return n;
}
static int countWrapped(float maxW, const char* s, void* font) {
    char ln[WRAP_LINES][WRAP_LEN]; return wrapText(s, maxW, font, ln);
}

// A panel that grows downward from yTop to hold all of its wrapped lines.
static float textBlock(float x0, float yTop, float w, const char* const* strs, const Col* cols, int n,
                       void* font, Col fill, Col edge, float fillA) {
    int total = 0;
    for (int i = 0; i < n; ++i) total += countWrapped(w - 8.0f, strs[i], font);
    float bottom = yTop - total * lineH(font) - 5.0f;
    Rect r = { x0, bottom, x0 + w, yTop };
    drawPanelBox(r, fill, edge, fillA);
    float y = yTop - 2.0f;
    for (int i = 0; i < n; ++i) { int k = drawWrapped(x0 + 4.0f, y, w - 8.0f, strs[i], cols[i], font); y -= k * lineH(font); }
    return bottom;
}

static void drawDialogueBand(const char* line, const char* prompt, const char* promptAlt, float reserveRight) {
    drawPanelBox(R_BAND, PAL_PANEL, PAL_AMBER, 0.93f);
    float w = (R_BAND.x1 - R_BAND.x0) - 10.0f - reserveRight, h = R_BAND.y1 - R_BAND.y0;
    float pl = prompt ? lineH(fontSmall()) : 0.0f;
    void* f = fontBig();
    if (countWrapped(w, line, f) * lineH(f) + pl > h - 2.0f) f = fontSmall();
    int n = drawWrapped(R_BAND.x0 + 5, R_BAND.y1 - 1.0f, w, line, PAL_CREAM, f);
    if (prompt && n * lineH(f) + pl <= h) {
        const char* p = prompt;
        if (textW(p, fontSmall()) > w && promptAlt) p = promptAlt;
        if (textW(p, fontSmall()) <= w) drawText(R_BAND.x0 + 5, R_BAND.y0 + 3, p, scaleCol(PAL_AMBER, 0.95f), fontSmall());
    }
}

static void drawButton(const Rect& r, const char* label, bool focus, const char* alt = nullptr) {
    setCol(focus ? mixCol(PAL_PANEL, PAL_AMBER, 0.35f) : PAL_PANEL, 0.95f); fillRect(r.x0, r.y0, r.x1, r.y1);
    glLineWidth(focus ? 2.5f : 1.0f); setCol(focus ? PAL_CREAM : scaleCol(PAL_AMBER, 0.7f));
    strokeRect(r.x0, r.y0, r.x1, r.y1); glLineWidth(1.0f);
    if (alt && textW(label, fontSmall()) > (r.x1 - r.x0) - 4.0f) label = alt;
    drawTextCentered((r.x0 + r.x1) * 0.5f, r.y0 + (r.y1 - r.y0) * 0.5f - 1.2f, label, focus ? PAL_WHITE : PAL_CREAM, fontSmall());
}

static void drawTitleUI() {
    Rect box = { 6.0f, 72.0f, 88.0f, 172.0f };
    drawPanelBox(box, PAL_PANEL, PAL_AMBER, 0.88f);
    drawTextCentered(47.0f, 156.0f, "PLEASE", PAL_AMBER, fontBig());
    drawTextCentered(47.0f, 143.0f, "INSERT", PAL_AMBER, fontBig());
    drawTextCentered(47.0f, 130.0f, "COIN", PAL_AMBER, fontBig());
    drawTextCentered(47.0f, 116.0f, "~ ~ ~", scaleCol(PAL_CREAM, 0.6f), fontSmall());
    drawButton(R_TITLE_START, "INSERT COIN", G.titleSel == 0, "COIN");
    drawButton(R_TITLE_HELP, "HOW TO PLAY", G.titleSel == 1, "HELP");
}

static void drawPurchaseHUD() {
    char credit[24];
    snprintf(credit, sizeof(credit), "CREDIT: %d/1", G.hasCoin ? 1 : 0);
    const char* strs[6]; Col cols[6]; int n = 0;
    strs[n] = credit;               cols[n++] = PAL_AMBER;
    strs[n] = "SELECT ITEM";        cols[n++] = PAL_CREAM;
    if (gVP.scale >= 2.5f) {                                   // hints only where they fit
        strs[n] = "1. COIN SLOT";   cols[n++] = scaleCol(PAL_CREAM, 0.7f);
        strs[n] = "2. ITEM BUTTON"; cols[n++] = scaleCol(PAL_CREAM, 0.7f);
    }
    if (K.feedbackT > 0.0f && K.feedback[0]) { strs[n] = K.feedback; cols[n++] = PAL_WARN; }
    textBlock(6.0f, 172.0f, 82.0f, strs, cols, n, fontSmall(), PAL_PANEL, PAL_AMBER, 0.9f);
}

static void drawRescueHUD() {
    // restrained maintenance terminal along the bottom edge; items that do not
    // fit on a row at the current window size are dropped (lowest priority last)
    Rect box = { 6.0f, 2.0f, 314.0f, 30.0f };
    drawPanelBox(box, PAL_TEAL_D, PAL_TEAL_L, 0.92f);
    char tgt[48], att[24];
    snprintf(tgt, sizeof(tgt), "TARGET: %s", SNACK_NAME[stage().target]);
    snprintf(att, sizeof(att), "ATTEMPTS: %d", G.attempts);
    const char* hint = "CLAW IN MOTION";
    if (C.state == IDLE_EMPTY) hint = CTRL_LOWER;
    else if (C.state == CARRYING) hint = CTRL_RELEASE;
    else if (C.state == RESOLVING_DROP) hint = "RELEASING...";
    bool mv = (C.state == IDLE_EMPTY || C.state == CARRYING);
    struct Item { const char* t; Col c; };
    Item row1[3] = { { "MAINTENANCE ACCESS", PAL_TEAL_L }, { tgt, PAL_CREAM }, { att, G.attempts <= 1 ? PAL_WARN : PAL_CREAM } };
    Item row2[3] = { { hint, PAL_AMBER }, { mv ? CTRL_MOVE : "", scaleCol(PAL_CREAM, 0.75f) }, { CTRL_PAUSE, scaleCol(PAL_CREAM, 0.75f) } };
    float lh = lineH(fontSmall()), y1 = 30.0f - 2.0f - lh * 0.75f, y2 = y1 - lh;
    if (y2 < 3.0f) y2 = 3.0f;
    for (int r = 0; r < 2; ++r) {
        Item* it = r ? row2 : row1; float x = 10.0f, y = r ? y2 : y1;
        for (int i = 0; i < 3; ++i) {
            if (!it[i].t[0]) continue;
            float w = textW(it[i].t, fontSmall());
            if (x + w > 308.0f) continue;
            drawText(x, y, it[i].t, it[i].c, fontSmall()); x += w + 12.0f;
        }
    }
    if (K.failT > 0.0f) {
        float w = fminf(300.0f, textW(K.failMsg, fontSmall()) + 12.0f);
        Rect fb = { 160.0f - w * 0.5f, 150.0f, 160.0f + w * 0.5f, 150.0f + lineH(fontSmall()) + 6.0f };
        drawPanelBox(fb, PAL_TEAL_D, PAL_WARN, 0.95f);
        drawTextCentered(160.0f, fb.y0 + 4.0f, K.failMsg, PAL_WARN, fontSmall());
    }
}

static void drawHelpPanel(bool rescue) {
    fullTint(PAL_INK, 0.55f);
    const char* head = rescue ? "MAINTENANCE CONTROLS" : "HOW TO PLAY";
    const char* rl[5] = { CTRL_MOVE, "SPACE: LOWER CLAW / RELEASE SNACK", CTRL_PAUSE,
                          "TAKE THE SNACK TO THE LIT CHUTE.", "A MISS OR A BAD DROP COSTS AN ATTEMPT." };
    const char* tl[3] = { "1. CLICK THE COIN SLOT TO INSERT A COIN.", "2. CLICK THE SNACK THE CUSTOMER ASKED FOR.",
                          "3. ENTER OR CLICK TO CONTINUE." };
    const char* const* body = rescue ? rl : tl;
    int n = rescue ? 5 : 3;
    Col cols[5] = { PAL_CREAM, PAL_CREAM, PAL_CREAM, PAL_AMBER, PAL_AMBER };
    if (!rescue) { cols[3] = cols[4] = PAL_CREAM; }
    float x0 = 20.0f, w = 280.0f, yTop = 170.0f;
    float headH = countWrapped(w - 8.0f, head, fontBig()) * lineH(fontBig()) + 4.0f;
    int total = 0;
    for (int i = 0; i < n; ++i) total += countWrapped(w - 8.0f, body[i], fontSmall());
    Rect box = { x0, yTop - headH - total * lineH(fontSmall()) - 6.0f, x0 + w, yTop };
    if (box.y0 < R_HELP_BACK.y1 + 3.0f) box.y0 = R_HELP_BACK.y1 + 3.0f;
    drawPanelBox(box, rescue ? PAL_TEAL_D : PAL_PANEL, rescue ? PAL_TEAL_L : PAL_AMBER, 0.97f);
    drawWrapped(x0 + 4.0f, yTop, w - 8.0f, head, rescue ? PAL_TEAL_L : PAL_AMBER, fontBig(), true);
    float y = yTop - headH;
    for (int i = 0; i < n; ++i) y -= drawWrapped(x0 + 4.0f, y, w - 8.0f, body[i], cols[i], fontSmall()) * lineH(fontSmall());
    drawButton(R_HELP_BACK, "BACK", true);
}

static void drawPauseMenu() {
    fullTint(PAL_INK, 0.6f);
    Rect box = { 90.0f, 44.0f, 230.0f, 150.0f };
    drawPanelBox(box, PAL_TEAL_D, PAL_TEAL_L, 0.96f);
    const char* t = textW("MAINTENANCE PAUSED", fontSmall()) <= 132.0f ? "MAINTENANCE PAUSED" : "PAUSED";
    drawTextCentered(160, 136, t, PAL_TEAL_L, fontSmall());
    for (int i = 0; i < 4; ++i) drawButton(R_PAUSE_OPT[i], PAUSE_LABEL[i], G.menuSel == i, PAUSE_SHORT[i]);
}

static void drawConfirm() {
    fullTint(PAL_INK, 0.65f);
    Rect box = { 40.0f, 42.0f, 280.0f, 134.0f };
    drawPanelBox(box, PAL_TEAL_D, PAL_WARN, 0.97f);
    bool rs = (G.overlay == OV_CONFIRM_RESTART);
    float w = box.x1 - box.x0 - 8.0f;
    int k = drawWrapped(box.x0 + 4, box.y1, w, rs ? "RESTART THIS STAGE?" : "RETURN TO STREET?", PAL_WARN, fontSmall(), true);
    drawWrapped(box.x0 + 4, box.y1 - k * lineH(fontSmall()), w,
                rs ? "RESCUE PROGRESS IS DISCARDED AND ATTEMPTS ARE RESET." : "THIS RUN WILL END AND ALL PROGRESS IS DISCARDED.",
                PAL_CREAM, fontSmall(), true);
    drawButton(R_CONF_CANCEL, "CANCEL", G.confSel == 0);
    drawButton(R_CONF_OK, "CONFIRM", G.confSel == 1, "OK");
}

static void drawOverlay() {
    switch (G.overlay) {
    case OV_PAUSE:           drawPauseMenu(); break;
    case OV_HELP_TITLE:      drawHelpPanel(false); break;
    case OV_HELP_RESCUE:     drawHelpPanel(true); break;
    case OV_CONFIRM_RESTART:
    case OV_CONFIRM_STREET:  drawConfirm(); break;
    default: break;
    }
}

static void drawGameOverUI() {
    fullTint(PAL_INK, 0.6f);
    Rect box = { 40.0f, 56.0f, 280.0f, 130.0f };
    drawPanelBox(box, PAL_TEAL_D, PAL_WARN, 0.97f);
    float w = box.x1 - box.x0 - 8.0f;
    void* f = fontBig();
    if (countWrapped(w, "RECOVERY UNSUCCESSFUL", f) > 1) f = fontSmall();
    int k = drawWrapped(box.x0 + 4, box.y1 - 2, w, "RECOVERY UNSUCCESSFUL", PAL_WARN, f, true);
    drawWrapped(box.x0 + 4, box.y1 - 2 - k * lineH(f), w, "NO ATTEMPTS REMAIN.", PAL_CREAM, fontSmall(), true);
    drawButton(R_RETRY, "RETRY - R / ENTER", true, "RETRY");
}

static void drawResultUI() {
    const char* msg = (G.resultPhase == 0) ? "ITEM RECOVERED" : stage().reaction;
    const char* t[1] = { "ITEM RECOVERED" }; Col c[1] = { PAL_AMBER };
    textBlock(6.0f, 172.0f, 82.0f, t, c, 1, fontSmall(), PAL_PANEL, PAL_AMBER, 0.9f);
    if (G.resultPhase == 0) {
        drawDialogueBand(msg, "COLLECT ITEM - ENTER / CLICK THE CHUTE", "COLLECT - ENTER", 0.0f);
        // the delivered snack glows in the chute flap
        float p = 0.5f + 0.5f * sinf(K.titleT * 4.0f);
        setCol(PAL_AMBER, 0.25f + 0.2f * p); fillRect(R_CHUTE2D.x0 + 1, R_CHUTE2D.y0 + 1, R_CHUTE2D.x1 - 1, R_CHUTE2D.y1 - 1);
        drawSnack2D(stage().target, (R_CHUTE2D.x0 + R_CHUTE2D.x1) * 0.5f - 6, R_CHUTE2D.y0 + 3, 12, stage().target == SNACK_CHOC ? 12 : 16);
        setCol(PAL_CREAM); glLineWidth(2.0f); strokeRect(R_CHUTE2D.x0, R_CHUTE2D.y0, R_CHUTE2D.x1, R_CHUTE2D.y1); glLineWidth(1.0f);
    } else {
        drawDialogueBand(msg, nullptr, nullptr, R_CONTINUE.x1 - R_CONTINUE.x0 + 6.0f);
        drawButton(R_CONTINUE, "CONTINUE", true, "NEXT");
    }
}

static void drawWarningBorder(float a) {
    setCol(PAL_WARN, a);
    fillRect(0, 0, LW, 3); fillRect(0, LH - 3, LW, LH); fillRect(0, 0, 3, LH); fillRect(LW - 3, 0, LW, LH);
}


/*==============================================================================
  9. GLUT CALLBACKS AND TIMING

  HOW IT WORKS
      * display(): picks the scene for the current state, draws that state's
        HUD, then any overlay. It never changes game state.
      * timerUpdate(): the ONE update chain. It reads GLUT_ELAPSED_TIME, clamps
        the delta, advances presentation clocks, and runs gameplay in fixed
        10 ms steps (bounded catch-up) only during an un-paused rescue.
        Pausing therefore freezes claw, snack and arm; resuming zeroes the
        accumulator so there is no jump.
      * Input callbacks are thin adapters: they normalise the event
        (key-repeat is disabled so every key-down is a fresh press), check the
        current state/overlay, then call one named action helper. Dispatch
        order: overlay first, then the active state's actions.
      * Mouse: left-button-down, converted to logical coordinates (letterbox
        rejected) then hit-tested against the shared Rects.
==============================================================================*/
static void display() {
    glDisable(GL_SCISSOR_TEST);
    glViewport(0, 0, gWinW < 1 ? 1 : gWinW, gWinH < 1 ? 1 : gWinH);
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);          // black letterbox bars

    Cues cu = computeCues();
    float lampScale = 1.0f - cu.dim;
    switch (G.state) {
    case TITLE:
        begin2D();
        drawStreet2D(1.0f); drawMachine2D(cu, false, 0); drawTitleUI();
        break;
    case CUSTOMER_INTRO:
        begin2D();
        drawStreet2D(lampScale); drawMachine2D(cu, false, 0); drawReflectionForState();
        drawCustomer(240.0f, 38.0f, G.stageIndex);
        drawPurchaseHUD();
        drawDialogueBand(stage().request, nullptr, nullptr, R_CONTINUE.x1 - R_CONTINUE.x0 + 6.0f);
        drawButton(R_CONTINUE, "CONTINUE", true, "NEXT");
        break;
    case MACHINE_2D:
        begin2D();
        drawStreet2D(lampScale); drawMachine2D(cu, false, 0); drawReflectionForState();
        drawCustomer(240.0f, 38.0f, G.stageIndex);
        drawPurchaseHUD();
        drawDialogueBand(stage().request, nullptr, nullptr, 0.0f);
        break;
    case TRANSITION_TO_3D: {
        float t = K.stateT;
        if (t < T_JAM + T_WARN) {
            begin2D();
            float sh = (t < T_JAM + T_WARN) ? sinf(t * 70.0f) * 0.7f : 0.0f;
            glTranslatef(sh, 0, 0);
            drawStreet2D(1.0f); drawMachine2D(cu, true, t);
            drawCustomer(240.0f, 38.0f, G.stageIndex);
            glLoadIdentity();
            { const char* jt[1] = { "DISPENSING INTERRUPTED" }; Col jc[1] = { PAL_WARN };
              textBlock(6.0f, 172.0f, 82.0f, jt, jc, 1, fontSmall(), PAL_PANEL, PAL_WARN, 0.92f); }
            if (t > T_JAM) {                                      // brief, restrained warning
                float u = (t - T_JAM) / T_WARN;
                drawWarningBorder(0.8f * fabsf(sinf(u * PI_F * 2.0f)));
                fullTint(PAL_WARN, 0.10f * sinf(u * PI_F));
            }
        } else {
            float u = (t - T_JAM - T_WARN) / T_SHIFT;
            glClearColor(PAL_TEAL_D.r, PAL_TEAL_D.g, PAL_TEAL_D.b, 1);
            begin3D(u); glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            drawScene3D();
            begin2D();
            fullTint(PAL_CREAM, 0.9f * (1.0f - smooth01(u * 3.0f)));    // reveal from the warning flash
        }
        break; }
    case RESCUE_3D:
    case GAME_OVER:
        glClearColor(PAL_TEAL_D.r, PAL_TEAL_D.g, PAL_TEAL_D.b, 1);
        begin3D(1.0f); glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        drawScene3D();
        begin2D();
        if (G.state == RESCUE_3D) drawRescueHUD(); else drawGameOverUI();
        break;
    case TRANSITION_TO_2D:
        glClearColor(PAL_TEAL_D.r, PAL_TEAL_D.g, PAL_TEAL_D.b, 1);
        begin3D(1.0f); glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        drawScene3D();
        begin2D();
        fullTint(PAL_INK, smooth01(K.stateT / T_TO2D));
        break;
    case STAGE_RESULT:
        begin2D();
        drawStreet2D(1.0f); drawMachine2D(cu, false, 0); drawReflectionForState();
        drawCustomer(240.0f, 38.0f, G.stageIndex);
        drawResultUI();
        fullTint(PAL_INK, 1.0f - smooth01(K.stateT / T_RESULT_FADE));
        break;
    case REFLECTION_OUTRO: {
        begin2D();
        drawStreet2D(1.0f); drawMachine2D(cu, false, 0); drawReflectionForState();
        if (G.outroPhase == OUT_EXIT) {                               // customer walks out of frame
            float u = smooth01(K.stateT / OUTRO_DUR[OUT_EXIT]);
            drawCustomer(240.0f + u * 110.0f, 38.0f, G.stageIndex);
        }
        float fl = outroFlicker();
        if (fl < 1.0f) fullTint(PAL_INK, (1.0f - fl) * 0.5f);         // light flicker over the scene
        break; }
    }
    if (G.overlay != OV_NONE) drawOverlay();
    glutSwapBuffers();
}

static void reshape(int w, int h) {
    gWinW = w < 1 ? 1 : w; gWinH = h < 1 ? 1 : h;                     // zero-size guard
    computeViewport();
    glutPostRedisplay();
}

// presentation-only clocks and scripted transitions (never reads input)
static void updatePresentation(float dt) {
    K.titleT += dt;
    if (!((G.state == RESCUE_3D && G.overlay != OV_NONE) || G.state == GAME_OVER)) K.visT += dt;
    if (K.feedbackT > 0.0f) K.feedbackT -= dt;
    if (K.failT > 0.0f && G.overlay == OV_NONE) K.failT -= dt;
    switch (G.state) {
    case CUSTOMER_INTRO:
    case MACHINE_2D:
        K.cueT += dt; break;
    case TRANSITION_TO_3D:
        K.stateT += dt;
        if (K.stateT >= T_TO3D) transitionTo(RESCUE_3D);
        break;
    case TRANSITION_TO_2D:
        K.stateT += dt;
        if (K.stateT >= T_TO2D) transitionTo(STAGE_RESULT);
        break;
    case STAGE_RESULT:
        K.stateT += dt; break;
    case RESCUE_3D:
        if (G.overlay == OV_NONE) K.rescueT += dt;
        break;
    case REFLECTION_OUTRO:
        updateOutro(dt); break;
    default: break;
    }
}

static void timerUpdate(int) {
    int now = glutGet(GLUT_ELAPSED_TIME);
    float dt = (now - gLastMs) / 1000.0f;
    gLastMs = now;
    if (dt < 0.0f) dt = 0.0f;
    if (dt > MAX_FRAME_DELTA) dt = MAX_FRAME_DELTA;                   // long stalls never teleport anything
    updatePresentation(dt);
    if (G.state == RESCUE_3D && G.overlay == OV_NONE) {
        gAccum += dt;
        int n = 0;
        while (gAccum >= FIXED_STEP && n < MAX_CATCHUP) {
            gAccum -= FIXED_STEP; ++n;
            stepRescue(FIXED_STEP);
            if (G.state != RESCUE_3D) break;                           // success/fail changed state
        }
        if (n >= MAX_CATCHUP) gAccum = 0.0f;                           // drop excess backlog
    } else {
        gAccum = 0.0f;                                                 // paused/other: no accumulated jump
    }
#ifdef _WIN32
    {   // keyboard focus (not pointer position): drop held keys if our window is not foreground
        static HWND hw = nullptr;
        if (!hw) hw = FindWindowA(nullptr, "PLEASE INSERT COIN");
        if (hw && GetForegroundWindow() != hw) clearHeldKeys();
    }
#endif
    glutPostRedisplay();
    glutTimerFunc(TIMER_MS, timerUpdate, 0);                           // exactly one next callback
}

// ---- input adapters -----------------------------------------------------------
enum LogicalKey { LK_NONE, LK_UP, LK_DOWN, LK_LEFT, LK_RIGHT, LK_ACTION, LK_PAUSE, LK_ESC, LK_ENTER, LK_RETRY };

static bool movementAccepted() {
    return G.state == RESCUE_3D && G.overlay == OV_NONE && (C.state == IDLE_EMPTY || C.state == CARRYING);
}
static void setHeld(int idx, bool v) { gHeld[idx] = v; }

static void menuMove(int& sel, int count, int delta) { sel = (sel + delta + count) % count; }

static void handleOverlayKey(LogicalKey k) {
    switch (G.overlay) {
    case OV_PAUSE:
        if (k == LK_UP)   menuMove(G.menuSel, 4, -1);
        else if (k == LK_DOWN) menuMove(G.menuSel, 4, 1);
        else if (k == LK_ENTER) activatePauseOption(G.menuSel);
        else if (k == LK_PAUSE || k == LK_ESC) resumeRescue();
        break;
    case OV_HELP_TITLE:
        if (k == LK_ENTER || k == LK_ESC) G.overlay = OV_NONE;
        break;
    case OV_HELP_RESCUE:
        if (k == LK_ENTER || k == LK_ESC) G.overlay = OV_PAUSE;        // back to the same pause selection
        break;
    case OV_CONFIRM_RESTART:
    case OV_CONFIRM_STREET:
        if (k == LK_LEFT || k == LK_RIGHT || k == LK_UP || k == LK_DOWN) G.confSel = 1 - G.confSel;
        else if (k == LK_ENTER) activateConfirm(G.confSel == 1);
        else if (k == LK_ESC) activateConfirm(false);
        break;
    default: break;
    }
}

static void handleKeyDown(LogicalKey k) {
    if (G.overlay != OV_NONE) { handleOverlayKey(k); return; }          // overlay consumes input exclusively
    switch (G.state) {
    case TITLE:
        if (k == LK_UP || k == LK_DOWN) G.titleSel = 1 - G.titleSel;
        else if (k == LK_ENTER) activateTitleOption(G.titleSel);
        break;
    case CUSTOMER_INTRO:
        if (k == LK_ENTER) transitionTo(MACHINE_2D);                    // unpaid purchase view
        break;
    case RESCUE_3D:
        if (k == LK_PAUSE || k == LK_ESC) pauseRescue();
        else if (k == LK_ACTION) requestGrabOrRelease();
        break;
    case STAGE_RESULT:
        if (k == LK_ENTER) acknowledgeResult();
        break;
    case GAME_OVER:
        if (k == LK_RETRY || k == LK_ENTER) retryStage();
        break;
    default: break;                                                      // transitions/outro: no input
    }
}

// Maps ASCII to a logical key + optional held-key slot (case-insensitive).
static void keyboardDown(unsigned char key, int, int) {
    if (key >= 'A' && key <= 'Z') key = (unsigned char)(key - 'A' + 'a');
    LogicalKey lk = LK_NONE; int held = -1;
    switch (key) {
    case 'w': lk = LK_UP;    held = H_W; break;
    case 's': lk = LK_DOWN;  held = H_S; break;
    case 'a': lk = LK_LEFT;  held = H_A; break;
    case 'd': lk = LK_RIGHT; held = H_D; break;
    case ' ': lk = LK_ACTION; break;
    case 'p': lk = LK_PAUSE; break;
    case 27:  lk = LK_ESC; break;
    case 13:  lk = LK_ENTER; break;
    case 'r': lk = LK_RETRY; break;
    }
    if (held >= 0 && movementAccepted()) setHeld(held, true);
    if (lk != LK_NONE) handleKeyDown(lk);
}

static void keyboardUp(unsigned char key, int, int) {
    if (key >= 'A' && key <= 'Z') key = (unsigned char)(key - 'A' + 'a');
    switch (key) {
    case 'w': setHeld(H_W, false); break;
    case 's': setHeld(H_S, false); break;
    case 'a': setHeld(H_A, false); break;
    case 'd': setHeld(H_D, false); break;
    }
}

static void specialDown(int key, int, int) {
    LogicalKey lk = LK_NONE; int held = -1;
    switch (key) {
    case GLUT_KEY_UP:    lk = LK_UP;    held = H_UP; break;
    case GLUT_KEY_DOWN:  lk = LK_DOWN;  held = H_DOWN; break;
    case GLUT_KEY_LEFT:  lk = LK_LEFT;  held = H_LEFT; break;
    case GLUT_KEY_RIGHT: lk = LK_RIGHT; held = H_RIGHT; break;
    }
    if (held >= 0 && movementAccepted()) setHeld(held, true);
    if (lk != LK_NONE) handleKeyDown(lk);
}

static void specialUp(int key, int, int) {
    switch (key) {
    case GLUT_KEY_UP:    setHeld(H_UP, false); break;
    case GLUT_KEY_DOWN:  setHeld(H_DOWN, false); break;
    case GLUT_KEY_LEFT:  setHeld(H_LEFT, false); break;
    case GLUT_KEY_RIGHT: setHeld(H_RIGHT, false); break;
    }
}

static void handleClick(float x, float y) {
    if (G.overlay != OV_NONE) {
        switch (G.overlay) {
        case OV_PAUSE:
            for (int i = 0; i < 4; ++i) if (inRect(R_PAUSE_OPT[i], x, y)) { G.menuSel = i; activatePauseOption(i); return; }
            break;
        case OV_HELP_TITLE:  if (inRect(R_HELP_BACK, x, y)) G.overlay = OV_NONE; break;
        case OV_HELP_RESCUE: if (inRect(R_HELP_BACK, x, y)) G.overlay = OV_PAUSE; break;
        case OV_CONFIRM_RESTART:
        case OV_CONFIRM_STREET:
            if (inRect(R_CONF_CANCEL, x, y)) { G.confSel = 0; activateConfirm(false); }
            else if (inRect(R_CONF_OK, x, y)) { G.confSel = 1; activateConfirm(true); }
            break;
        default: break;
        }
        return;
    }
    switch (G.state) {
    case TITLE:
        if (inRect(R_TITLE_START, x, y)) { G.titleSel = 0; activateTitleOption(0); }
        else if (inRect(R_TITLE_HELP, x, y)) { G.titleSel = 1; activateTitleOption(1); }
        break;
    case CUSTOMER_INTRO:
        if (inRect(R_CONTINUE, x, y)) transitionTo(MACHINE_2D);
        break;
    case MACHINE_2D:
        if (inRect(R_COIN, x, y)) insertCoin();
        else for (int i = 0; i < 3; ++i) if (inRect(R_BTN[i], x, y)) { selectSnack(i); break; }
        break;
    case STAGE_RESULT:
        if (G.resultPhase == 0 ? inRect(R_CHUTE2D, x, y) : inRect(R_CONTINUE, x, y)) acknowledgeResult();
        break;
    case GAME_OVER:
        if (inRect(R_RETRY, x, y)) retryStage();
        break;
    default: break;
    }
}

static void mouse(int button, int state, int mx, int my) {
    if (button != GLUT_LEFT_BUTTON || state != GLUT_DOWN) return;       // one click = one action
    float lx, ly;
    if (!mouseToLogical(mx, my, lx, ly)) return;                        // letterbox bars rejected
    handleClick(lx, ly);
    glutPostRedisplay();
}

// hover moves menu focus using the same rectangles as drawing / clicking
static void passiveMotion(int mx, int my) {
    float lx, ly;
    if (!mouseToLogical(mx, my, lx, ly)) return;
    if (G.overlay == OV_PAUSE) { for (int i = 0; i < 4; ++i) if (inRect(R_PAUSE_OPT[i], lx, ly)) G.menuSel = i; }
    else if (G.overlay == OV_CONFIRM_RESTART || G.overlay == OV_CONFIRM_STREET) {
        if (inRect(R_CONF_CANCEL, lx, ly)) G.confSel = 0; else if (inRect(R_CONF_OK, lx, ly)) G.confSel = 1;
    } else if (G.overlay == OV_NONE && G.state == TITLE) {
        if (inRect(R_TITLE_START, lx, ly)) G.titleSel = 0; else if (inRect(R_TITLE_HELP, lx, ly)) G.titleSel = 1;
    }
}

// hidden window must not leave a movement key "stuck" (pointer enter/leave is NOT focus; see timerUpdate)
static void visibilityFunc(int v) { if (v != GLUT_VISIBLE) clearHeldKeys(); }


/*==============================================================================
  10. main

  HOW IT WORKS
      Initialise GLUT once (double buffer, RGB, depth), create the window,
      build the static vertex data, reset the game to the title, register all
      callbacks and start the single timer chain, then enter glutMainLoop.
==============================================================================*/
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(WIN_W, WIN_H);
    glutCreateWindow("PLEASE INSERT COIN");

    buildStaticGeometry();
    computeViewport();
    memset(&K, 0, sizeof(K));
    memset(&C, 0, sizeof(C));
    memset(&G, 0, sizeof(G));
    returnToTitle();

    glutIgnoreKeyRepeat(1);                       // each key-down is a fresh press
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboardDown);
    glutKeyboardUpFunc(keyboardUp);
    glutSpecialFunc(specialDown);
    glutSpecialUpFunc(specialUp);
    glutMouseFunc(mouse);
    glutPassiveMotionFunc(passiveMotion);
    glutVisibilityFunc(visibilityFunc);

    gLastMs = glutGet(GLUT_ELAPSED_TIME);
    glutTimerFunc(TIMER_MS, timerUpdate, 0);
    glutMainLoop();
    return 0;
}
