#include <GL/glut.h>
#include <cstdlib>
#include <string>

const int WINDOW_WIDTH = 2167;
const int WINDOW_HEIGHT = 672;

bool dropdownOpen = false;
std::string selectedTerm = "1-20262027";

void setColor(float r, float g, float b) {
    glColor3f(r, g, b);
}

void drawRect(float x, float y, float width, float height,
              float r, float g, float b) {
    setColor(r, g, b);

    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
}

void drawOutline(float x, float y, float width, float height,
                 float r, float g, float b) {
    setColor(r, g, b);

    glBegin(GL_LINE_LOOP);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
}

void drawText(float x, float y, const char* text,
              void* font = GLUT_BITMAP_HELVETICA_12,
              float r = 0.08f,
              float g = 0.15f,
              float b = 0.22f) {
    setColor(r, g, b);
    glRasterPos2f(x, y);

    for (int i = 0; text[i] != '\0'; i++) {
        glutBitmapCharacter(font, text[i]);
    }
}

void drawCenteredText(float x, float y, float width,
                      const char* text,
                      void* font = GLUT_BITMAP_HELVETICA_12,
                      float r = 0.08f,
                      float g = 0.15f,
                      float b = 0.22f) {
    int textWidth = glutBitmapLength(
        font,
        reinterpret_cast<const unsigned char*>(text)
    );

    drawText(
        x + (width - textWidth) / 2.0f,
        y,
        text,
        font,
        r,
        g,
        b
    );
}

void drawDropdownArrow(float x, float y) {
    setColor(0.72f, 0.72f, 0.72f);

    glBegin(GL_TRIANGLES);
    glVertex2f(x, y + 7);
    glVertex2f(x + 10, y + 7);
    glVertex2f(x + 5, y);
    glEnd();
}

void drawHeader() {
    drawText(
        20,
        WINDOW_HEIGHT - 54,
        "Schedule and Assessment",
        GLUT_BITMAP_HELVETICA_18,
        0.05f,
        0.10f,
        0.16f
    );

    drawText(
        WINDOW_WIDTH - 240,
        WINDOW_HEIGHT - 45,
        "Schedule and Assessment",
        GLUT_BITMAP_HELVETICA_12,
        0.05f,
        0.10f,
        0.16f
    );
}

void drawControls() {
    const float labelX = 25;
    const float controlY = WINDOW_HEIGHT - 145;

    drawText(
        labelX,
        controlY + 10,
        "Term - School Year :"
    );

    const float dropdownX = 201;
    const float dropdownY = controlY - 2;
    const float dropdownW = 155;
    const float dropdownH = 41;

    drawRect(
        dropdownX,
        dropdownY,
        dropdownW,
        dropdownH,
        1.0f,
        1.0f,
        1.0f
    );

    drawOutline(
        dropdownX,
        dropdownY,
        dropdownW,
        dropdownH,
        0.55f,
        0.55f,
        0.55f
    );

    drawText(
        dropdownX + 9,
        dropdownY + 14,
        selectedTerm.c_str(),
        GLUT_BITMAP_HELVETICA_12,
        0.65f,
        0.65f,
        0.65f
    );

    drawDropdownArrow(
        dropdownX + dropdownW - 16,
        dropdownY + 17
    );

    drawRect(
        382,
        dropdownY - 4,
        88,
        45,
        0.10f,
        0.63f,
        0.25f
    );

    drawText(
        397,
        dropdownY + 12,
        "Submit",
        GLUT_BITMAP_HELVETICA_12,
        1.0f,
        1.0f,
        1.0f
    );

    if (dropdownOpen) {
        drawRect(
            dropdownX,
            dropdownY - 35,
            dropdownW,
            35,
            1.0f,
            1.0f,
            1.0f
        );

        drawOutline(
            dropdownX,
            dropdownY - 35,
            dropdownW,
            35,
            0.55f,
            0.55f,
            0.55f
        );

        drawText(
            dropdownX + 9,
            dropdownY - 22,
            "1-20262027",
            GLUT_BITMAP_HELVETICA_12,
            0.20f,
            0.20f,
            0.20f
        );
    }
}

void drawStudentInformation() {
    const float leftX = 26;
    const float rightX = 1028;
    const float startY = 421;
    const float lineHeight = 22;

    drawText(
        leftX,
        startY,
        "Enrollment Status : ENROLLED"
    );

    drawText(
        leftX,
        startY - lineHeight,
        "Student # : 202310284"
    );

    drawText(
        leftX,
        startY - lineHeight * 2,
        "Name : VIRAY, EARL JOHN BARREDO"
    );

    drawText(
        leftX,
        startY - lineHeight * 3,
        "Phone : 0969503596   Zip Code : 4024"
    );

    drawText(
        leftX,
        startY - lineHeight * 4,
        "Classification :"
    );

    drawText(
        rightX,
        startY - lineHeight,
        "College : COMPUTER STUDIES    Program : BSCSE"
    );

    drawText(
        rightX,
        startY - lineHeight * 2,
        "Year Level : 3     1st term, SY 26-27"
    );

    drawText(
        rightX,
        startY - lineHeight * 3,
        "Address : SOUTH CITY HOMES SUBD., BRGY. STO TOMAS"
    );
}

void displayCourses() {
    const float tableX = 25;
    const float tableY = 45;
    const float rowH = 29;
    const float headerH = 31;

    const float colTitle = tableX + 148;
    const float colSection = tableX + 1035;
    const float colUnits = tableX + 1155;
    const float colDays = tableX + 1245;
    const float colTime = tableX + 1320;
    const float colRoom = tableX + 1810;

    const float rowStart = tableY + rowH;

    // CS0011
    float y = rowStart + rowH * 6 + 10;

    drawCenteredText(tableX + 10, y, 100, "CS0011",
                     GLUT_BITMAP_HELVETICA_10);
    drawText(colTitle, y, "MOBILE PROGRAMMING",
             GLUT_BITMAP_HELVETICA_10);
    drawCenteredText(colSection - 20, y, 75, "TN35",
                     GLUT_BITMAP_HELVETICA_10);
    drawCenteredText(colUnits - 10, y, 45, "3",
                     GLUT_BITMAP_HELVETICA_10);
    drawCenteredText(colDays - 20, y, 70, "M / W",
                     GLUT_BITMAP_HELVETICA_10);
    drawCenteredText(colTime - 45, y, 390,
                     "11:00:00-12:50:00 / 11:00:00-12:50:00",
                     GLUT_BITMAP_HELVETICA_10);
    drawCenteredText(colRoom - 35, y, 180, "F608 / F1209",
                     GLUT_BITMAP_HELVETICA_10);

    // CS0016
    y = rowStart + rowH * 5 + 10;

    drawCenteredText(tableX + 10, y, 100, "CS0016",
                     GLUT_BITMAP_HELVETICA_10);
    drawText(colTitle, y, "NETWORK AND COMMUNICATIONS 2A",
             GLUT_BITMAP_HELVETICA_10);
    drawCenteredText(colSection - 20, y, 75, "TS31",
                     GLUT_BITMAP_HELVETICA_10);
    drawCenteredText(colUnits - 10, y, 45, "3",
                     GLUT_BITMAP_HELVETICA_10);
    drawCenteredText(colDays - 20, y, 70, "T / TH",
                     GLUT_BITMAP_HELVETICA_10);
    drawCenteredText(colTime - 45, y, 390,
                     "17:00:00-18:50:00 / 17:00:00-18:50:00",
                     GLUT_BITMAP_HELVETICA_10);
    drawCenteredText(colRoom - 35, y, 180, "ONLINE / F1101",
                     GLUT_BITMAP_HELVETICA_10);

    // CS0019
    y = rowStart + rowH * 4 + 10;

    drawCenteredText(tableX + 10, y, 100, "CS0019",
                     GLUT_BITMAP_HELVETICA_10);
    drawText(colTitle, y, "MODELING AND SIMULATION",
             GLUT_BITMAP_HELVETICA_10);
    drawCenteredText(colSection - 20, y, 75, "TN35",
                     GLUT_BITMAP_HELVETICA_10);
    drawCenteredText(colUnits - 10, y, 45, "3",
                     GLUT_BITMAP_HELVETICA_10);
    drawCenteredText(colDays - 20, y, 70, "F / T",
                     GLUT_BITMAP_HELVETICA_10);
    drawCenteredText(colTime - 45, y, 390,
                     "11:00:00-12:50:00 / 11:00:00-12:50:00",
                     GLUT_BITMAP_HELVETICA_10);
    drawCenteredText(colRoom - 35, y, 180, "ONLINE / ONLINE",
                     GLUT_BITMAP_HELVETICA_10);

    // CS0025
    y = rowStart + rowH * 3 + 10;

    drawCenteredText(tableX + 10, y, 100, "CS0025",
                     GLUT_BITMAP_HELVETICA_10);
    drawText(colTitle, y, "SOFTWARE ENGINEERING 1",
             GLUT_BITMAP_HELVETICA_10);
    drawCenteredText(colSection - 20, y, 75, "TN35",
                     GLUT_BITMAP_HELVETICA_10);
    drawCenteredText(colUnits - 10, y, 45, "3",
                     GLUT_BITMAP_HELVETICA_10);
    drawCenteredText(colDays - 20, y, 70, "F / TH",
                     GLUT_BITMAP_HELVETICA_10);
    drawCenteredText(colTime - 45, y, 390,
                     "15:00:00-16:50:00 / 15:00:00-16:50:00",
                     GLUT_BITMAP_HELVETICA_10);
    drawCenteredText(colRoom - 35, y, 180, "ONLINE / E609",
                     GLUT_BITMAP_HELVETICA_10);

    // CS0045
    y = rowStart + rowH * 2 + 10;

    drawCenteredText(tableX + 10, y, 100, "CS0045",
                     GLUT_BITMAP_HELVETICA_10);
    drawText(colTitle, y,
             "CS ELECTIVE - COMPUTER GRAPHICS AND VISUAL COMPUTING",
             GLUT_BITMAP_HELVETICA_10);
    drawCenteredText(colSection - 20, y, 75, "TN35",
                     GLUT_BITMAP_HELVETICA_10);
    drawCenteredText(colUnits - 10, y, 45, "3",
                     GLUT_BITMAP_HELVETICA_10);
    drawCenteredText(colDays - 20, y, 70, "M / TH",
                     GLUT_BITMAP_HELVETICA_10);
    drawCenteredText(colTime - 45, y, 390,
                     "13:00:00-14:50:00 / 13:00:00-14:50:00",
                     GLUT_BITMAP_HELVETICA_10);
    drawCenteredText(colRoom - 35, y, 180, "E601 / E610",
                     GLUT_BITMAP_HELVETICA_10);

    // GED0065
    y = rowStart + rowH + 10;

    drawCenteredText(tableX + 10, y, 100, "GED0065",
                     GLUT_BITMAP_HELVETICA_10);
    drawText(colTitle, y, "ENVIRONMENTAL SCIENCE",
             GLUT_BITMAP_HELVETICA_10);
    drawCenteredText(colSection - 20, y, 75, "TN48",
                     GLUT_BITMAP_HELVETICA_10);
    drawCenteredText(colUnits - 10, y, 45, "3",
                     GLUT_BITMAP_HELVETICA_10);
    drawCenteredText(colDays - 20, y, 70, "F / T",
                     GLUT_BITMAP_HELVETICA_10);
    drawCenteredText(colTime - 45, y, 390,
                     "09:00:00-10:50:00 / 09:00:00-10:50:00",
                     GLUT_BITMAP_HELVETICA_10);
    drawCenteredText(colRoom - 35, y, 180, "ONLINE / ASYNCH",
                     GLUT_BITMAP_HELVETICA_10);

    // GED0075
    y = rowStart + 10;

    drawCenteredText(tableX + 10, y, 100, "GED0075",
                     GLUT_BITMAP_HELVETICA_10);
    drawText(colTitle, y, "LINEAR ALGEBRA",
             GLUT_BITMAP_HELVETICA_10);
    drawCenteredText(colSection - 20, y, 75, "TS21",
                     GLUT_BITMAP_HELVETICA_10);
    drawCenteredText(colUnits - 10, y, 45, "3",
                     GLUT_BITMAP_HELVETICA_10);
    drawCenteredText(colDays - 20, y, 70, "F / T",
                     GLUT_BITMAP_HELVETICA_10);
    drawCenteredText(colTime - 45, y, 390,
                     "13:00:00-14:50:00 / 13:00:00-14:50:00",
                     GLUT_BITMAP_HELVETICA_10);
    drawCenteredText(colRoom - 35, y, 180, "ONLINE / ONLINE",
                     GLUT_BITMAP_HELVETICA_10);
}

void drawTable() {
    const float tableX = 25;
    const float tableY = 45;
    const float tableW = WINDOW_WIDTH - 50;
    const float rowH = 29;
    const float headerH = 31;

    // Header background
    drawRect(
        tableX,
        tableY + rowH * 8,
        tableW,
        headerH,
        0.88f,
        0.88f,
        0.87f
    );

    // Alternating row backgrounds
    for (int row = 0; row < 7; row += 2) {
        drawRect(
            tableX,
            tableY + rowH + row * rowH,
            tableW,
            rowH,
            0.93f,
            0.93f,
            0.92f
        );
    }

    // Table border
    drawOutline(
        tableX,
        tableY,
        tableW,
        headerH + rowH * 8,
        0.55f,
        0.55f,
        0.55f
    );

    // Column headings
    drawText(tableX + 8, tableY + rowH * 8 + 11, "Courses");
    drawText(tableX + 148, tableY + rowH * 8 + 11, "Title");
    drawText(tableX + 1035, tableY + rowH * 8 + 11, "Section");
    drawText(tableX + 1155, tableY + rowH * 8 + 11, "Units");
    drawText(tableX + 1245, tableY + rowH * 8 + 11, "Days");
    drawText(tableX + 1320, tableY + rowH * 8 + 11, "Time");
    drawText(tableX + 1810, tableY + rowH * 8 + 11, "Room");

    displayCourses();

    // Total row
    drawRect(
        tableX,
        tableY,
        tableW,
        rowH,
        0.93f,
        0.93f,
        0.92f
    );

    drawText(
        tableX + 8,
        tableY + 10,
        "TOTAL UNITS 21",
        GLUT_BITMAP_HELVETICA_10,
        0.20f,
        0.27f,
        0.32f
    );
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    drawRect(
        0,
        0,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        0.96f,
        0.97f,
        0.98f
    );

    drawHeader();
    drawControls();
    drawStudentInformation();
    drawTable();

    glutSwapBuffers();
}

void reshape(int width, int height) {
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluOrtho2D(
        0,
        WINDOW_WIDTH,
        0,
        WINDOW_HEIGHT
    );

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void mouse(int button, int state, int x, int y) {
    if (button != GLUT_LEFT_BUTTON ||
        state != GLUT_DOWN) {
        return;
    }

    float glY = WINDOW_HEIGHT - y;

    const float dropdownX = 201;
    const float dropdownY = WINDOW_HEIGHT - 147;
    const float dropdownW = 155;
    const float dropdownH = 41;

    // Open or close dropdown
    if (x >= dropdownX &&
        x <= dropdownX + dropdownW &&
        glY >= dropdownY &&
        glY <= dropdownY + dropdownH) {
        dropdownOpen = !dropdownOpen;
        glutPostRedisplay();
        return;
    }

    // Select dropdown option
    if (dropdownOpen &&
        x >= dropdownX &&
        x <= dropdownX + dropdownW &&
        glY >= dropdownY - 35 &&
        glY <= dropdownY) {
        selectedTerm = "1-20262027";
        dropdownOpen = false;
        glutPostRedisplay();
        return;
    }

    // Submit button
    if (x >= 382 &&
        x <= 470 &&
        glY >= dropdownY - 4 &&
        glY <= dropdownY + 41) {
        dropdownOpen = false;
        glutPostRedisplay();
    }
}

void keyboard(unsigned char key, int, int) {
    if (key == 27) {
        std::exit(0);
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);

    glutInitDisplayMode(
        GLUT_DOUBLE | GLUT_RGB
    );

    glutInitWindowSize(
        WINDOW_WIDTH,
        WINDOW_HEIGHT
    );

    glutInitWindowPosition(80, 80);

    glutCreateWindow(
        "Schedule and Assessment"
    );

    glClearColor(
        0.96f,
        0.97f,
        0.98f,
        1.0f
    );

    glDisable(GL_DEPTH_TEST);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);

    glutMainLoop();

    return 0;
}