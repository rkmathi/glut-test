#ifndef __MAIN_H_
#define __MAIN_H_

#include <ctime>
#include <iostream>
#include <random>
#include <GLUT/glut.h>

/* Function prototypes */
void LogDebug(std::string, bool);
void RenderString(float, float, const char*, const GLfloat*);
void RandArea4p(GLfloat [][2]);
bool IsPointContactsSquare(const GLfloat point[], const GLfloat area4p[][2]);
void Display(void);
void Resize(int, int);
void Mouse(int, int, int, int);
void Keyboard(unsigned char, int, int);
void Timer(int);
void Init(void);

/* Constants */
extern const bool kIS_DEBUG;
extern const uint64_t kFRAME_MS;
extern const float kSQUARE_SIZE;

/* Global variables */
extern uint64_t frame_count;
extern uint64_t score;
extern GLfloat area4p[4][2];

#endif//__MAIN_H_
