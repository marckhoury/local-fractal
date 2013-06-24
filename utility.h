#ifndef UTILITY_H
#define UTILITY_H

#include <string>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <iostream>
#include <fstream>

using std::string;
using std::stringstream;
using std::cout;
using std::endl;
using std::vector;
using std::ofstream;
using std::ostream;

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
void WritePlot(string& outfile, vector<T>& data)
{
	ofstream out(outfile.c_str());
	for(int i = 0; i < data.size(); i++)
		out << i << "\t" << data[i] << endl;
	out.close();
	cout << "Wrote table " << outfile << endl;
}

#endif
