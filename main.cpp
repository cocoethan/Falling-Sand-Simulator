#include <GL/glut.h>
#include <vector>
#include <cstdlib>
#include <ctime>

const int width = 800;
const int height = 600;
const int gridSize = 5;
const int gridWidth = width / gridSize;
const int gridHeight = height / gridSize;
bool isDrawing = false;

struct SandParticle {
    int colorR;
    int colorG;
    int colorB;
};

std::vector<std::vector<SandParticle>> grid(gridWidth, std::vector<SandParticle>(gridHeight));

void init() {
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, width, 0.0, height);
}

void initializeSand() {
    srand(time(NULL));

    for (int i = 0; i < gridWidth; ++i) {
        for (int j = 0; j < gridHeight; ++j) {
            if (rand() % 100 < 10) {
                grid[i][j].colorR = rand() % 256;
                grid[i][j].colorG = rand() % 256;
                grid[i][j].colorB = rand() % 256;
            }
        }
    }
}

void applyGravity() {
    for (int i = 0; i < gridWidth; ++i) {
        for (int j = 1; j < gridHeight; ++j) { // Start from the second row
            if (grid[i][j].colorR != -1 && grid[i][j - 1].colorR == -1) { // If the current cell has sand AND the cell below is empty
                grid[i][j - 1] = grid[i][j]; // Drop the sand particle
                grid[i][j].colorR = -1; // Mark the cell as -1
            }
        }
    }
}

void drawSand(int x, int y) {
    int i = x / gridSize;
    int j = y / gridSize;
    if (i >= 0 && i < gridWidth && j >= 0 && j < gridHeight) {
        if (grid[i][j].colorR == -1) { // Only assign color if cell is -1
            grid[i][j].colorR = rand() % 256;
            grid[i][j].colorG = rand() % 256;
            grid[i][j].colorB = rand() % 256;
        }
    }
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            isDrawing = true;
            drawSand(x, height - y);
        }
        else if (state == GLUT_UP) {
            isDrawing = false;
        }
    }
}

void motion(int x, int y) {
    if (isDrawing) {
        drawSand(x, height - y);
    }
}

void renderGrid() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLineWidth(1.0);

    for (int i = 0; i < gridWidth; ++i) {
        for (int j = 0; j < gridHeight; ++j) {
            if (grid[i][j].colorR != -1) {
                glColor3ub(grid[i][j].colorR, grid[i][j].colorG, grid[i][j].colorB);
                glBegin(GL_QUADS);
                glVertex2i(i * gridSize, j * gridSize);
                glVertex2i((i + 1) * gridSize, j * gridSize);
                glVertex2i((i + 1) * gridSize, (j + 1) * gridSize);
                glVertex2i(i * gridSize, (j + 1) * gridSize);
                glEnd();
            }
        }
    }
    glFlush();
}

void update(int value) {
    applyGravity();
    glutPostRedisplay();
    glutTimerFunc(1000 / 30, update, 0); // 30 FPS
}

int main(int argc, char** argv) {
    for (int i = 0; i < gridWidth; ++i) {
        for (int j = 0; j < gridHeight; ++j) {
            grid[i][j].colorR = -1; // Initialize color to -1 indicating no sand particle
        }
    }

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(width, height);
    glutCreateWindow("Sand Simulation");
    init();
    initializeSand();
    glutDisplayFunc(renderGrid);
    glutTimerFunc(0, update, 0);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutMainLoop();
    return 0;
}
