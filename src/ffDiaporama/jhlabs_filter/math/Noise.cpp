#include "Noise.h"
bool Noise::start = true;
int Noise::p[B + B + 2];
float Noise::g3[B + B + 2][3];// = new float[B + B + 2][3];
float Noise::g2[B + B + 2][2];// = new float[B + B + 2][2];
float Noise::g1[B + B + 2];// = new float[B + B + 2];
