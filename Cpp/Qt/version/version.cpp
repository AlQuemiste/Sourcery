#include <QtCore>
#include <iostream>

int main() {
    std::cout << "Qt version: " << qVersion() << std::endl;
    return 0;
}


/*

g++ -o version version.cpp -I/usr/include/x86_64-linux-gnu/qt5/QtCore \
-I/usr/include/x86_64-linux-gnu/qt5 -L/usr/lib/qt5 -lQt5Core -fPIC

*/
