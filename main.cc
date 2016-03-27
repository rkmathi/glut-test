#include "./main.h"

/* Constants */
const bool kIS_DEBUG = true;
const uint64_t kFRAME_MS = 33;
const float kSQUARE_SIZE = 40.0f;

/* Global variables */
uint64_t frame_count = 0;
uint64_t score = 0;
GLfloat area4p[4][2] = {
  { 0.0f, 0.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f },
};

int main(int argc, char *argv[]) {
  LogDebug("call glutInitWindowPosition()", kIS_DEBUG);
  glutInitWindowPosition(0, 0);
  LogDebug("call glutInitWindowSize()", kIS_DEBUG);
  glutInitWindowSize(640, 640);
  LogDebug("call glutInit()", kIS_DEBUG);
  glutInit(&argc, argv);
  LogDebug("call glutInitDisplayMode()", kIS_DEBUG);
  glutInitDisplayMode(GLUT_RGBA);
  LogDebug("call glutCreateWindow()", kIS_DEBUG);
  glutCreateWindow("Hello, GLUT!");
  LogDebug("call glutDisplayFunc()", kIS_DEBUG);
  glutDisplayFunc(Display);
  LogDebug("call glutReshapeFunc()", kIS_DEBUG);
  glutReshapeFunc(Resize);
  LogDebug("call glutMouseFunc()", kIS_DEBUG);
  glutMouseFunc(Mouse);
  LogDebug("call glutKeyboardFunc()", kIS_DEBUG);
  glutKeyboardFunc(Keyboard);
  LogDebug("call glutTimerFunc()", kIS_DEBUG);
  glutTimerFunc(kFRAME_MS, Timer, 0);
  LogDebug("call init()", kIS_DEBUG);
  Init();
  LogDebug("call glutMainLoop()", kIS_DEBUG);
  glutMainLoop();
  return 0;
}

void Display(void) {
  glClear(GL_COLOR_BUFFER_BIT);
  GLfloat colorGray[4] =  { 0.5f, 0.5f, 0.5f, 1.0f };
  GLfloat colorRed[4] =   { 1.0f, 0.0f, 0.0f, 1.0f };

  /* Display frame */
  char timer_buf[80];
  sprintf(timer_buf, "FRAME: %llu", frame_count);
  RenderString(0.0f, 20.0f, timer_buf, colorGray);

  /* Display score */
  char score_buf[80];
  sprintf(score_buf, "SCORE: %llu", score);
  RenderString(0.0f, 40.0f, score_buf, colorGray);

  /* Display square */
  glColor4fv(colorRed);
  glBegin(GL_QUADS);
  for (int i = 0; i < 4; ++i) {
    glVertex2d(area4p[i][0], area4p[i][1]);
  }
  glEnd();

  glFlush();
}

void Resize(int w, int h) {
  glViewport(0, 0, w, h);
  glLoadIdentity();
  //glOrtho(-w/200.0, w/200.0, -h/200.0, h/200.0, -1.0, 1.0);
  glOrtho(-0.5, (GLfloat)w - 0.5, (GLfloat)h - 0.5, -0.5, -1.0, 1.0);
}

void Mouse(int button, int state, int x, int y) {
  /* DEBUG */
  switch (button) {
  case GLUT_LEFT_BUTTON:
    LogDebug("left button", kIS_DEBUG);
    break;
  case GLUT_RIGHT_BUTTON:
    LogDebug("right button", kIS_DEBUG);
    break;
  case GLUT_MIDDLE_BUTTON:
    LogDebug("middle button", kIS_DEBUG);
    break;
  default:
    break;
  }
  switch (state) {
    case GLUT_UP:
      LogDebug("up", kIS_DEBUG);
      break;
    case GLUT_DOWN:
      LogDebug("down", kIS_DEBUG);
      break;
    default:
      break;
  }
  char buf[20];
  sprintf(buf, "at (%d, %d)", x, y);
  LogDebug(buf, kIS_DEBUG);

  /* Get click event and add score */
  if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
    GLfloat clicked_xy[2] = { (float)x, (float)y };
    bool flag = IsPointContactsSquare(clicked_xy, area4p);
    if (flag) {
      ++score;
      RandArea4p(area4p);
    }
  }
}

void Keyboard(unsigned char key, int x, int y) {
  /* DEBUG */
  char buf[80];
  sprintf(buf, "input keyboard %c: (%d, %d)", key, x, y);
  LogDebug(buf, kIS_DEBUG);

  /* Exit if input is q,Q,ESC */
  switch (key) {
    case 'q':
    case 'Q':
    case '\033': /* ESC */
      LogDebug("Exit by keyboard input", kIS_DEBUG);
      exit(EXIT_SUCCESS);
    default:
      break;
  }
}

void Timer(int value) {
  if (value == 0) {
    ++frame_count;
    glutPostRedisplay();
    glutTimerFunc(kFRAME_MS, Timer, 0);
  }
}

void Init(void) {
  glClearColor(1.0, 1.0, 1.0, 1.0); /* White */
  RandArea4p(area4p);
}

void LogDebug(std::string msg, bool is_debug) {
  if (is_debug) {
    time_t now = time(nullptr);
    tm *time_now = localtime(&now);
    char time_buffer[20];
    strftime(time_buffer, sizeof(time_buffer), "%F %T", time_now);
    std::cout << time_buffer << "\t" << msg << std::endl;
  }
}

void RenderString(float x, float y, const char *str, const GLfloat *color4f) {
  glColor4fv(color4f);
  glRasterPos3f(x, y, -1.0f);
  char *p = (char*)str;
  while (*p != '\0') glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *p++);
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
