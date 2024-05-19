#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <utility>

std::pair<unsigned int, unsigned int> initShaders(int& statusCode);
std::pair<unsigned int, unsigned int> initVAOs();
void cleanUpShadersAndVAOs();

#endif