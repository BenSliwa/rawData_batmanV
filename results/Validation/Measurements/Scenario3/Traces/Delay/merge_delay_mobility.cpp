#include <iostream>
#include <sstream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <fstream>
#include <string>
using namespace std;


template<typename Out>
void split(const std::string &s, char delim, Out result) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        *(result++) = item;
    }
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}

string readLineBin(ifstream& in){
    string line="";

    char ch=0;
    while(ch != '\n' & !in.eof()){
        in.get(ch);
        line+=ch;
    }
    return line;
}


int main( void )
{
  	// Reading Data
	string filename_throughput = "../../Delay/delay2.txt";
	string filename_trace = "mobility_1534393784.txt";
	string filename_write = "Delay_Mobility_2.txt";

	char cstr[256];
	char cstr2[256];
	fstream wstr;
	ifstream rstr1;
	ifstream rstr2;
	rstr1.open(filename_throughput.c_str(), ios::in);
	rstr2.open(filename_trace.c_str(), ios::in);
	if (rstr1.is_open() && rstr2.is_open())
	{
		string strOne="", strTwo="";
		while( !rstr1.eof()| !rstr2.eof())
		{
			string strOne, strTwo;

			strOne=readLineBin(rstr1);
			strTwo=readLineBin(rstr2);
			if(strOne.empty() | strTwo.empty())
				break;
			//cout << strOne << " " << strTwo << endl;
			double time = stod(strOne.substr(0, 2));
			double delay = stod(strOne.substr(2, 5));

			//cout << time << " " << delay << endl;
			std::vector<string> x = split(strTwo, ',');
			float latitude = stof(x[1]);
			float longitude = stof(x[2]);

			wstr.open(filename_write, ios::out|ios::app|ios::binary);
			wstr << time << "," << latitude << "," << longitude << "," << delay << endl;
			wstr.close();
			cout << time << "," << latitude << "," << longitude << "," << delay << endl;

   		}

	}
	else{
		cout << "rstr no open!" << endl;	
	}
	
	
	rstr1.close();
	rstr2.close();
	return 0;
}