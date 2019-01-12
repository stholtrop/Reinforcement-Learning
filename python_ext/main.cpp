#include <Python.h>
#include <vector>
#include "matrix.cpp"
#include "flippo.cpp"


static PyObject * vector_to_list(const std::vector<double> v) {
    PyObject* result = PyList_New(0);
    for (auto a : v) {
        PyList_Append(result, PyFloat_FromDouble(a));
    }
    return result;
}

static PyObject * game_getBoard(PyObject *self, PyObject *args) {
    PyObject* result = PyList_New(0);
    for (auto a : Flippo::createGame(1)) {
        PyObject* matrix = vector_to_list(a.board.getData());
        PyList_Append(result, matrix);
    }
    return result;
}

static PyMethodDef GameMethods[] = {
    {"getBoard", game_getBoard, METH_VARARGS, "Get list of boards"},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef game = {
    PyModuleDef_HEAD_INIT,
    "game",
    NULL,
    -1,
    GameMethods
};

PyMODINIT_FUNC PyInit_game(void) {
    return PyModule_Create(&game);
}