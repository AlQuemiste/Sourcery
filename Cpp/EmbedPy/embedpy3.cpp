// #define Py_LIMITED_API 0x03070000
#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <cstdio>

int main()
{
	char filename[] = "pyemb7.py";

	Py_Initialize();

	FILE* fp = _Py_fopen(filename, "r");
    if (!fp)
        printf("File '%s' not found.\n", filename);
    else
        PyRun_SimpleFile(fp, filename);

    PyObject* p;
	p = PyLong_FromLong(50);
	printf("value<long> = %ld\n", PyLong_AsLong(p));

    Py_XDECREF(p);

    //----------------------------------------

    PyObject* const pName = PyUnicode_FromString("os");
	PyObject* const pModule = PyImport_Import(pName);

    printf("AN>> pModule = %x\n", pModule);
    if(pModule)
	{
		PyObject* pFunc = PyObject_GetAttrString(pModule, "getInteger");
		if(pFunc && PyCallable_Check(pFunc))
		{
			PyObject* pValue = PyObject_CallObject(pFunc, NULL);
			printf("C: getInteger() = %ld\n", PyLong_AsLong(pValue));
		}
		else
		{
			printf("ERROR: function getInteger()\n");
		}

	}
	else
	{
		printf("ERROR: Module not imported\n");
	}
    //----------------------------------------

    Py_Finalize();
	return 0;
}
