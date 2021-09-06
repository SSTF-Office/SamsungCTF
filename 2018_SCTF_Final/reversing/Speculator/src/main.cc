#include <cstdlib>
#include <unistd.h>
#include <fstream>
#include "Speculator/Speculator.hh"

using namespace std;

void usage(char *pname)
{
    std::cerr << "[Usage] " << pname <<" [--add-treasure] [Kernel.bin] [Program.bin]" << std::endl;
    exit(0);
}

int main(int argc, char **argv) {
    int tFlag = false;
    string kern, bin;
    if (!strcmp(argv[1], "--add-treasure") && argc == 4) {
        tFlag = true;
        kern = argv[2];
        bin = argv[3];
    } else if(strcmp(argv[1], "--add-treasure") && argc == 3) {
        kern = argv[1];
        bin = argv[2];
    } else {
        usage(argv[0]);
    }

    alarm(10);
    setvbuf(stdin, 0, 2, 0);
    setvbuf(stdout, 0, 2, 0);

    Speculator *t = new Speculator(kern, bin);
    if (tFlag) {
        t->AddTreasure("./flag");
    }
    t->run();
    return 0;
}
