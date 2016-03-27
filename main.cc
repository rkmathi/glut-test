#include <ctime>
#include <iostream>
#include <random>

#include <GLUT/glut.h>

/* Function prototypes */
void LogDebug(std::string, bool);
void RenderString(double, double, const char*, const GLfloat*);
void RandArea4p(GLfloat [][2]);
bool IsPointContactsSquare(const GLfloat point[], const GLfloat area4p[][2]);
void display(void);
void resize(int, int);
void mouse(int, int, int, int);
void keyboard(unsigned char, int, int);
void timer(int);
void init(void);

/* Constants */
const uint64_t kFRAME_MS = 33;
const float kSQUARE_SIZE = 40.0f;

/* Global variables */
uint64_t frame_count = 0;
uint64_t score = 0;
GLfloat area4p[4][2] = {
  { 0.0f, 0.0f },
  { 0.0f, 0.0f },
  { 0.0f, 0.0f },
  { 0.0f, 0.0f },
};

int main(int argc, char *argv[]) {
  bool is_debug = true;
  LogDebug("call glutInitWindowPosition()", is_debug);
  glutInitWindowPosition(0, 0);
  LogDebug("call glutInitWindowSize()", is_debug);
  glutInitWindowSize(640, 640);
  LogDebug("call glutInit()", is_debug);
  glutInit(&argc, argv);
  LogDebug("call glutInitDisplayMode()", is_debug);
  glutInitDisplayMode(GLUT_RGBA);
  LogDebug("call glutCreateWindow()", is_debug);
  glutCreateWindow("Hello, GLUT!");
  LogDebug("call glutDisplayFunc()", is_debug);
  glutDisplayFunc(display);
  LogDebug("call glutReshapeFunc()", is_debug);
  glutReshapeFunc(resize);
  LogDebug("call glutMouseFunc()", is_debug);
  glutMouseFunc(mouse);
  LogDebug("call glutKeyboardFunc()", is_debug);
  glutKeyboardFunc(keyboard);
  LogDebug("call glutTimerFunc()", is_debug);
  glutTimerFunc(kFRAME_MS, timer, 0);
  LogDebug("call init()", is_debug);
  init();
  LogDebug("call glutMainLoop()", is_debug);
  glutMainLoop();
  return 0;
}

void LogDebug(std::string msg, bool is_debug) {
  if (is_debug) {
    time_t now = time(nullptr);
    tm *time_now = localtime(&now);
    char time_buffer[20];
    strftime(time_buffer, sizeof(time_buffer), "%F %T", time_now);
    std::cout
      << time_buffer
      << "\t"
      << msg
      << std::endl;
  }
}

void RenderString(double x, double y, const char *str, const GLfloat *color4f) {
  glColor4fv(color4f);
  glRasterPos3d(x, y, -1.0);
  char *p = (char*)str;
  while (*p != '\0') glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *p++);
}

void display(void) {
  glClear(GL_COLOR_BUFFER_BIT);

  GLfloat colorGray[4] = { 0.5, 0.5, 0.5, 1.0 };
  char timer_buf[80];
  sprintf(timer_buf, "FRAME: %llu", frame_count);
  RenderString(0.0, 20.0, timer_buf, colorGray);

  char score_buf[80];
  sprintf(score_buf, "SCORE: %llu", score);
  RenderString(0.0, 40.0, score_buf, colorGray);

  GLfloat colorRed[4] = { 1.0, 0.0, 0.0, 1.0 };
  glColor4fv(colorRed);
  glBegin(GL_QUADS);
  for (int i = 0; i < 4; ++i) {
    glVertex2d(area4p[i][0], area4p[i][1]);
  }
  glEnd();

  glFlush();
}

void resize(int w, int h) {
  glViewport(0, 0, w, h);
  glLoadIdentity();
  //glOrtho(-w/200.0, w/200.0, -h/200.0, h/200.0, -1.0, 1.0);
  glOrtho(-0.5, (GLdouble)w - 0.5, (GLdouble)h - 0.5, -0.5, -1.0, 1.0);
}

bool IsPointContactsSquare(const GLfloat point[], const GLfloat area4p[][2]) {
  if (
    (area4p[0][0] <= point[0] && point[0] <= area4p[2][0]) &&
    (area4p[0][1] <= point[1] && point[1] <= area4p[2][1])
  ) {
    return true;
  } else {
    return false;
  }
}

void mouse(int button, int state, int x, int y) {
  /* Get click event and add score */
  if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
    GLfloat clicked_xy[2] = { (float)x, (float)y };
    bool flag = IsPointContactsSquare(clicked_xy, area4p);
    if (flag) {
      ++score;
      RandArea4p(area4p);
    }
  }

  /* DEBUG */
  switch (button) {
  case GLUT_LEFT_BUTTON:
    LogDebug("left button", true);
    break;
  case GLUT_RIGHT_BUTTON:
    LogDebug("right button", true);
    break;
  case GLUT_MIDDLE_BUTTON:
    LogDebug("middle button", true);
    break;
  default:
    break;
  }
  switch (state) {
    case GLUT_UP:
      LogDebug("up", true);
      break;
    case GLUT_DOWN:
      LogDebug("down", true);
      break;
    default:
      break;
  }
  char buf[20];
  sprintf(buf, "at (%d, %d)", x, y);
  LogDebug(buf, true);
}

void keyboard(unsigned char key, int x, int y) {
  /* DEBUG */
  char buf[80];
  sprintf(buf, "input keyboard %c: (%d, %d)", key, x, y);
  LogDebug(buf, true);

  /* Exit if input is q, Q, ESC */
  switch (key) {
    case 'q':
    case 'Q':
    case '\033': /* ESC */
      LogDebug("Exit by keyboard input", true);
      exit(EXIT_SUCCESS);
    default:
      break;
  }
}

void RandArea4p(GLfloat f[][2]) {
  std::random_device rnd;
  std::mt19937_64 mt(rnd());
  std::uniform_int_distribution<> rand20_600(20, 600);
  float rand_x = (float)rand20_600(mt);
  float rand_y = (float)rand20_600(mt);
  f[0][0] = rand_x;
  f[0][1] = rand_y;
  f[1][0] = rand_x;
  f[1][1] = rand_y + kSQUARE_SIZE;
  f[2][0] = rand_x + kSQUARE_SIZE;
  f[2][1] = rand_y + kSQUARE_SIZE;
  f[3][0] = rand_x + kSQUARE_SIZE;
  f[3][1] = rand_y;
}

void timer(int value) {
  ++frame_count;
  glutPostRedisplay();
  glutTimerFunc(kFRAME_MS, timer, 0);
}

void init(void) {
  glClearColor(1.0, 1.0, 1.0, 1.0); /* White */
  RandArea4p(area4p);
}
