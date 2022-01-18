#define PY_SSIZE_T_CLEAN
#ifndef POINT_H
#define POINT_H

#include <Python.h>
#include <structmember.h>

// Macro definition
#define DIMENSION 2

// Type definition
typedef struct {
    // Pointオブジェクトの内容
    PyObject_HEAD  // それぞれのオブジェクト構造体の先頭に必須なもの
    // Type-specific fields go here.
    int x;
    int y;
} PointObject;

// Prototype declaration
static PyTypeObject PointType;

#endif