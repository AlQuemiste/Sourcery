// https://www.codeproject.com/articles/820116/embedding-python-program-in-a-c-cplusplus-code

#include <stdio.h>
#include <Python.h>

int main()
{
	PyObject* pInt;

	Py_Initialize();

	PyRun_SimpleString("print('Hello World from Embedded Python!!!')");
	PyRun_SimpleString("import matplotlib");
    PyRun_SimpleString("c = 2; i = -2;");
    PyRun_SimpleString("print(\"c = %d; i = %d\" % (c, i))");

	Py_Finalize();

	printf("\nPress any key to exit...\n");
	// if(!getch()) getch();
	return 0;
}
