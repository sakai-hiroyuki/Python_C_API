#include "point.h"

/*
    参考: "拡張の型の定義: チュートリアル" (https://docs.python.org/ja/3.9/extending/newtypes_tutorial.html)
*/

/* PointType.tp_dealloc */
static void Point_dealloc(PointObject *self){
    Py_TYPE(self)->tp_free((PyObject *) self);
}

/* PointType.tp_new */
static PyObject *Point_new(PyTypeObject *type, PyObject *args, PyObject *kwds){
    PointObject *self;
    self = (PointObject *) type->tp_alloc(type, 0);
    if (self != NULL) {
        self->x = 0;
        self->y = 0;
    }
    return (PyObject *) self;
}

/* PointType.tp_init */
static int Point_init(PointObject *self, PyObject *args, PyObject *kwds){
    static char *kwlist[] = {"x", "y", NULL};

    if(!PyArg_ParseTupleAndKeywords(args, kwds, "|ii", kwlist, &self->x, &self->y))
        return -1;

    return 0;
}

/* PointType.tp_alloc */
static PointObject *Point_alloc(PyTypeObject *self, Py_ssize_t nitems){
    PointObject *obj = (PointObject *) PyObject_MALLOC(sizeof(PointObject));
    PyObject_INIT(obj, self);
    return obj;
}

/* PointType.tp_member */
static PyMemberDef Point_members[] = {
    {"x", T_INT, offsetof(PointObject, x), 0, "x-axis"},
    {"y", T_INT, offsetof(PointObject, y), 0, "y-axis"},
    {NULL}  /* Sentinel */
};

/* PointType.tp_str */
static PyObject *Point_str(PointObject *self){
    return PyUnicode_FromFormat("(%d, %d)", self->x, self->y);
}

static PyObject *Point_tolist(PointObject *self, PyObject *Py_UNUSED(ignored)){
    PyObject *result = PyList_New(DIMENSION);
    PyList_SetItem(result, 0, PyLong_FromLong(self->x));
    PyList_SetItem(result, 1, PyLong_FromLong(self->y));
    
    return result;
}

static PyMethodDef Point_methods[] = {
    {"tolist", (PyCFunction) Point_tolist, METH_NOARGS},
    {NULL}  /* Sentinel */
};

/* PointType.tp_as_number.nb_add */
static PointObject *Point_add(PointObject* self, PointObject* other){
    PointObject *result = (PointObject *) PyType_GenericAlloc(&PointType, 0);

    result->x = self->x + other->x;
    result->y = self->y + other->y;
    
    Py_INCREF(result);
    return result;
}

/* PointType.tp_as_number.nb_subtract */
static PointObject *Point_subtract(PointObject* self, PointObject* other){
    PointObject *result = (PointObject *) PyType_GenericAlloc(&PointType, 0);

    result->x = self->x - other->x;
    result->y = self->y - other->y;

    Py_INCREF(result);
    return result;
}

/* PointType.tp_as_number.nb_multiply */
static PointObject *Point_multiply(PointObject* self, PointObject* other){
    PointObject *result = (PointObject *) PyType_GenericAlloc(&PointType, 0);

    result->x = self->x * other->x;
    result->y = self->y * other->y;
    
    Py_INCREF(result);
    return result;
}

/* PointType.tp_as_number.nb_negative */
static PointObject *Point_negative(PointObject* self){
    PointObject *result = (PointObject *) PyType_GenericAlloc(&PointType, 0);

    result->x = -self->x;
    result->y = -self->y;
    
    Py_INCREF(result);
    return result;
}

/* PointType.tp_as_number */
static PyNumberMethods Point_as_number = {
    .nb_add         = (binaryfunc) Point_add,
    .nb_subtract    = (binaryfunc) Point_subtract,
    .nb_multiply    = (binaryfunc) Point_multiply,
    .nb_negative    = (unaryfunc)  Point_negative,
};

static PySequenceMethods Point_as_sequence = {};

static PyTypeObject PointType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name        = "point.Point",
    .tp_doc         = "Point objects",
    .tp_basicsize   = sizeof(PointObject),
    .tp_itemsize    = 0,
    .tp_flags       = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    .tp_alloc       = (allocfunc)  Point_alloc,
    .tp_new         = (newfunc)    Point_new,
    .tp_init        = (initproc)   Point_init,
    .tp_dealloc     = (destructor) Point_dealloc,
    .tp_str         = (reprfunc)   Point_str,
    .tp_as_number   = &Point_as_number,
    .tp_as_sequence = &Point_as_sequence,
    .tp_members     = Point_members,
    .tp_methods     = Point_methods,
};

static PyObject *point_norm(PyObject *self, PyObject* args){
    PointObject *p;

    if(!PyArg_ParseTuple(args, "O", &p))
        return NULL;

    return PyFloat_FromDouble(sqrt(p->x * p->x + p->y * p->y));
}

static PyObject *point_dot(PyObject *self, PyObject* args){
    PointObject *p, *q;

    if(!PyArg_ParseTuple(args, "OO", &p, &q))
        return NULL;

    return PyLong_FromLong(p->x * q->x + p->y * q->y);
}

static PyMethodDef pointmethod[] = {
    {"norm", point_norm, METH_VARARGS},
    {"dot",  point_dot,  METH_VARARGS},
    {NULL}
};

static PyModuleDef pointmodule = {
    PyModuleDef_HEAD_INIT,
    .m_name    = "point",
    .m_doc     = "point module.",
    .m_size    = -1,
    .m_methods = pointmethod
};

PyMODINIT_FUNC PyInit_point(void){
    PyObject *m;
    if (PyType_Ready(&PointType) < 0)
        return NULL;

    m = PyModule_Create(&pointmodule);
    if (m == NULL)
        return NULL;
    
    Py_INCREF(&PointType);
    if (PyModule_AddObject(m, "Point", (PyObject *) &PointType) < 0) {
        Py_DECREF(&PointType);
        Py_DECREF(m);
        return NULL;
    }

    return m;
}