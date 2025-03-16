#ifndef __LINEAR_ALG_H
#define __LINEAR_ALG_H

#include <math.h>

typedef struct Vector2 {
    double x;
    double y;
} Vector2;

typedef struct Vector2i {
    int x;
    int y;
} Vector2i;

void multiplyByMatrix(Vector2 *vector, int **matrix) {
    Vector2 tmp = *vector;
    tmp.x = vector->x*matrix[0][0] + vector->y*matrix[0][1];
    tmp.y = vector->x*matrix[1][0] + vector->y*matrix[1][1];
    vector->x = tmp.x;
    vector->y = tmp.y;
}

void rotateVector(Vector2 *vector, double angle) {
    Vector2 tmp = *vector;
    tmp.x = vector->x*cos(angle) - vector->y*sin(angle);
    tmp.y = vector->x*sin(angle) + vector->y*cos(angle);
    vector->x = tmp.x;
    vector->y = tmp.y;
}

double dotProduct(Vector2 vector1, Vector2 vector2) {
    return vector1.x*vector2.x+vector1.y*vector2.y;
}

double norm(Vector2 vector) {
    return sqrt(dotProduct(vector, vector));
}

#endif