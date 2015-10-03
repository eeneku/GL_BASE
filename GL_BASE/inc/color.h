#pragma once

#include <GL\glew.h>

struct Color
{
	GLfloat r;
	GLfloat g;
	GLfloat b;
	GLfloat a;
};

static const Color clrRed = { 1.0f, 0.0f, 0.0f, 1.0f };
static const Color clrBlue = { 0.0f, 0.0f, 1.0f, 1.0f };
static const Color clrGreen = { 0.0f, 1.0f, 0.0f, 1.0f };
static const Color clrWhite = { 1.0f, 1.0f, 1.0f, 1.0f };
static const Color clrBlack = { 0.0f, 0.0f, 0.0f, 1.0f };