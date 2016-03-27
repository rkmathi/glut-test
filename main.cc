#include <ctime>
#include <iostream>

#include <GLUT/glut.h>

void LogDebug(std::string, bool);
void display(void);
void init(void);

int main(int argc, char *argv[]) {
  bool is_debug = true;
  LogDebug("START", is_debug);

  LogDebug("call glutInit()", is_debug);
  glutInit(&argc, argv);

  LogDebug("call glutInitDisplayMode()", is_debug);
  glutInitDisplayMode(GLUT_RGBA);

  LogDebug("call glutCreateWindow()", is_debug);
  glutCreateWindow("Hello, GLUT!");

  LogDebug("call glutDisplayFunc()", is_debug);
  glutDisplayFunc(display);

  LogDebug("call init()", is_debug);
  init();

  LogDebug("call glutMainLoop()", is_debug);
  glutMainLoop();

  LogDebug("EXIT", is_debug);
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
  glFlush();
}

void init(void) {
  glClearColor(0.0, 0.0, 1.0, 1.0);
}
