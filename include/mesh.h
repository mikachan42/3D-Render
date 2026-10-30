#ifndef MESH_H
#define MESH_H

#define POINT_COUNT 6
#define FACE_COUNT 8


typedef struct {
    float x;
    float y;
} Point2D;

typedef struct {
    float x;
    float y;
    float z;
} Point3D;


extern const Point3D Points[POINT_COUNT];
extern const int Faces[FACE_COUNT][3];


#endif