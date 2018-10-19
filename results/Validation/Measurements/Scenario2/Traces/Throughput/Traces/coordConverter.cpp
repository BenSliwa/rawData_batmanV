#include <iostream>
#include <sstream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <fstream>
#include <string>
#include <math.h>

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
	string filename_trace = "mobility_1534392510.txt";
	string filename_write = "mobility_1534392510_cartesian.txt";

	char cstr[256];
	fstream wstr; // write string
	ifstream rstr; // read string
	rstr.open(filename_trace.c_str(), ios::in);
	if (rstr.is_open())
	{
		string strOne="";
		while(!rstr.eof())
		{
			strOne = readLineBin(rstr);
			
			if(strOne.empty())
				break;
			//cout << strOne << " " << endl;
			
			std::vector<string> readVector = split(strOne, ',');
			double time = round( stod(readVector[0]) );
			double latitude = ( stod(readVector[1]) * ((double)M_PI) / 180 );
			double longitude = ( stod(readVector[2]) * ((double)M_PI) / 180 );
			double earth_radius = 6378 * 1000; //[m]
			double x = earth_radius * cos(latitude) * cos(longitude) - 3938000;
			double y = earth_radius * cos(latitude) * sin(longitude) - 512240;
			
			
			wstr.open(filename_write, ios::out|ios::app|ios::binary);
			wstr << time << "," << x << "," << y << endl;
			wstr.close();

			cout << time << ", " << x << ", " << y << endl;

   		}

	}
	else{
		cout << "rstr no open!" << endl;	
	}
	
	
	rstr.close();
	return 0;
}