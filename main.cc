#include <ctime>
#include <iostream>

#include <GLUT/glut.h>

const int kMAXPOINTS = 100;
GLint point[kMAXPOINTS][2];
int point_num = 0;
int rubberband = 0;

void LogDebug(std::string, bool);
void display(void);
void resize(int, int);
void mouse(int, int, int, int);
void motion(int, int);
void keyboard(unsigned char, int, int);
void init(void);

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
  LogDebug("call glutMotionFunc()", is_debug);
  glutMotionFunc(motion);
  LogDebug("call glutKeyboardFunc()", is_debug);
  glutKeyboardFunc(keyboard);
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

void display(void) {
  glClear(GL_COLOR_BUFFER_BIT);

  if (point_num > 1) {
    glColor3d(0.0, 0.0, 0.0);
    glBegin(GL_LINES);
    for (int i = 0; i < point_num; ++i) {
      glVertex2iv(point[i]);
    }
    glEnd();
  }

  glFlush();
}

void resize(int w, int h) {
  glViewport(0, 0, w, h);
  glLoadIdentity();
  glOrtho(-0.5, (GLdouble)w - 0.5, (GLdouble)h - 0.5, -0.5, -1.0, 1.0);
}

void mouse(int button, int state, int x, int y) {
  switch(button) {
  case GLUT_LEFT_BUTTON:
    point[point_num][0] = x;
    point[point_num][1] = y;
    if (state == GLUT_UP) {
      glColor3d(0.0, 0.0, 0.0);
      glBegin(GL_LINES);
      glVertex2iv(point[point_num - 1]);
      glVertex2iv(point[point_num]);
      glEnd();
      glFlush();
      rubberband = 0;
    }
    if (point_num < kMAXPOINTS - 1) ++point_num;
    break;
  }

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

  char buf[80];
  sprintf(buf, "at (%d, %d)", x, y);
  LogDebug(buf, true);
}

void motion(int x, int y) {
  static GLint savepoint[2];

  glEnable(GL_COLOR_LOGIC_OP);
  glLogicOp(GL_INVERT);
  glBegin(GL_LINES);
  if (rubberband) {
    glVertex2iv(point[point_num - 1]);
    glVertex2iv(savepoint);
  }
  glVertex2iv(point[point_num - 1]);
  glVertex2i(x, y);
  glEnd();
  glFlush();

  glLogicOp(GL_COPY);
  glDisable(GL_COLOR_LOGIC_OP);

  savepoint[0] = x;
  savepoint[1] = y;
  rubberband = 1;
}

void keyboard(unsigned char key, int x, int y) {
  char buf[10];
  sprintf(buf, "input %c", key);
  LogDebug(buf, true);

  switch (key) {
    case 'q':
    case 'Q':
    case '\033': /* ESC */
      LogDebug("Exit by keyboard input", true);
      exit(0);
    default:
      break;
  }
}

void init(void) {
  glClearColor(1.0, 1.0, 1.0, 1.0);
}
