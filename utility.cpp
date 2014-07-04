#include "utility.h"

void fatal_error(const string& err, int code)
{
    cerr << "\033[1;31mFatal Error:\033[0m " << err << endl;
    exit(code);
}
