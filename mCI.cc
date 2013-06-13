#include <iostream>
#include <fstream>
#include <cstdio>
#include <tchar.h>
#include <string>
#include <vector>
#include "Data.h"
#include <cmath>

/*
 * I'm calling every 0.25 seconds ( or half cycle) a mit
 *
 * assume that the start and end of a new signal is every 0.2 seconds
 * first file_ind when a signal change happens
 * by taking the time when the signal switches from off to on.
 * and using that as your starting point.
 * 
 * then file_ind the first time there are 3 off mits in a row
 * 
 * once that's done record mits until you have found 3 off mits in a row
 * 
 * discard the last 3 mits (stop signal) and compare what we've recorded
 * to what we know
 */
 std::vector<std::string> signals;
 
 void sigInit(){
	signals.emplace_back("1011");
	signals.emplace_back("11010101");
	signals.emplace_back("110101101");
	signals.emplace_back("110101");
	signals.emplace_back("1");
	signals.emplace_back("10101101");
	signals.emplace_back("1101101");
	signals.emplace_back("1010101");
	signals.emplace_back("101");
	signals.emplace_back("1011011011");
	signals.emplace_back("1101011");
	signals.emplace_back("10110101");
	signals.emplace_back("11011");
	signals.emplace_back("1101");
	signals.emplace_back("11011011");
	signals.emplace_back("101101101");
	signals.emplace_back("1101101011");
	signals.emplace_back("101101");
	signals.emplace_back("10101");
	signals.emplace_back("11");
	signals.emplace_back("101011");
	signals.emplace_back("10101011");
	signals.emplace_back("1011011");
	signals.emplace_back("110101011");
	signals.emplace_back("1101011011");
	signals.emplace_back("110110101");
 }
 
 double diff(double d1, double d2){
	if(std::abs(d1 - d2) < 1e-4)
		return 0;
	else
		return d1-d2;
 }
 
 void seeData(std::vector<Data> data){
	for(int i = 0; i < data.size(); ++i){
		std::cout << data[i].time << " " <<data[i].value << std::endl;
	}
}

 //this function file_inds the start of an mbit
 int file_indSignalChange(std::vector<Data> data){
	for(int i = 1; i < data.size(); ++i){
		if(data[i].value > 1 && data[i-1].value < 1){
			return i;
		}else if(data[i].value < 1 && data[i-1].value > 1){
			return i;
		}
	}
	return -1;
 }
 
 //after file_inding a signal change do a half step to file_ind the mid point of a
 //signal to determine what the signal is for that mit
 int midPoint(int start, std::vector<Data> data){
 
	double nextVal = data[start].time + 0.1;
	for(int i = start; i < data.size(); ++i){
		if((data[i].time - nextVal) > 0){
			if(fabs(data[i].time - nextVal) < fabs(data[i-1].time - nextVal))
				return i;
			else
				return i-1;
		}
	}
		
	return -1;
 }
 
 //This gets us the next mit in the list
  int nextPoint(int start, std::vector<Data> data){
  
	double nextVal = data[start].time + 0.2;
	for(int i = start; i < data.size(); ++i){
		if((data[i].time - nextVal) > 0){
			if(fabs(data[i].time - nextVal) < fabs(data[i-1].time - nextVal))
				return i;
			else
				return i-1;
		}
	}
		
	return -1;
 }
 
 int file_indStartPoint(std::vector<Data> data){
	int zerosInRow = 0;
	int j = file_indSignalChange(data);
	int start = midPoint(j, data);
	for( int i = start; i < data.size();i = nextPoint(i, data)){
		if(i == -1){
			return -1;
		}
		if(data[i].value < 1){	
			++zerosInRow;
			if(zerosInRow == 3){
				return i;
			}
		}else{
			zerosInRow = 0;
		}
	}
	return -1;
 }
 
 //puts the signal into a string
 std::string recordSignal( std::vector<Data> data){
	std::string toReturn = std::string("");
	
	int zerosInRow = 0;
	int start = file_indStartPoint(data);
	start = nextPoint(start, data);
	for(int i = start; i < data.size(); i = nextPoint(i, data)){
		if(i==-1){
			return std::string("ERROR");
		}
		if(data[i].value > 1){
			toReturn += "1";
			zerosInRow = 0;
		}else{
			
			toReturn += "0";
			++zerosInRow;
			if(zerosInRow == 3){
				toReturn.erase(toReturn.end()-3,toReturn.end());
				return toReturn;
			}
		}
	}
	return std::string("ERROR");
 }
 
 std::vector<Data> getData(TCHAR * filename){
 
	std::ifstream file_in;
	std::string line;
	float time, intensity;
	std::vector<Data> toReturn;
	
	file_in.open(filename);
	if(!file_in.is_open()){
		std::cout <<"could not open file" << std::endl;
		return toReturn;
	}
	
	while(file_in.good()){
	
		std::getline(file_in, line);
		
			if(line.compare("END")==0){
				return toReturn;
			}else{
				sscanf(line.c_str(),"%f %f", &time, &intensity);
				toReturn.push_back(Data(time,intensity));
			}
	}
	return std::vector<Data>();
 }
 
 
 std::string compareSignal(std::string str){
	for(int i = 0; i < signals.size(); ++i){
		if(str.compare(signals[i]) == 0){
			return std::string() += (char)(i+65);
		}
	}
	return std::string("ERROR");
 }

int main(int argc, TCHAR * args[]){
	if(argc ==1){
		std::cout << "Usage: mCI <Input filename>";
		return 0;
	}
	sigInit();
	
	std::string signal;
	std::vector<Data> data = getData(args[1]);
	
	signal = recordSignal(data);

	signal = compareSignal(signal);
	std::cout << signal << std::endl;
	
	//Plot the information
	system("cmd /c start matlab -nodesktop -nosplash -r \"GraphPatchInfo('dumpdata.txt')\"");
	return 0;

}






