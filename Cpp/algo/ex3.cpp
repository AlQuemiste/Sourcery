#include <iostream>

class Useless {
public:
    Useless() { throw "You can't construct me!"; }

};

static Useless object;

int main() {
    std::cout << "This will never be printed" << std::endl;

    return 0;
}
