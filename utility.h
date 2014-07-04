#ifndef UTILITY_H
#define UTILITY_H

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

using std::cout;
using std::cerr;
using std::endl;
using std::ofstream;
using std::ostream;
using std::string;
using std::stringstream;
using std::vector;

template<class T>
string str(T s)
{
    stringstream ss;
    string res;
    ss << s;
    ss >> res;
    return res;
}

template<class T>
void write_plot(string& outfile, vector<T>& data)
{
    ofstream out(outfile.c_str());
    for(int i = 0; i < data.size(); i++) {
        out << i << "\t" << data[i] << endl;
    }
    out.close();
    cout << "Wrote table " << outfile << endl;
}

void fatal_error(const string& err, int code = 1);

#endif
