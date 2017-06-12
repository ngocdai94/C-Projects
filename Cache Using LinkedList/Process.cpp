/*
 Author: Dai Nguyen
 Program: A simple cache for basic data science tasks.
 Last Modification: 05/19/2017
*/

#include "Cache.h"

string const commandFile = "commands.in";   // command filename
string const outputFile = "result.out";     // result filename

static const int NOT_FOUND_MAX_MIN = -1;    // condition check for Max or Min
static const double NOT_FOUND_AVG_MED = -1; // condition check for Avg or Med

//check data file function
bool checkData(string fileName);

//readFiles: use to find max, min, avg, or med if value is not in cache
bool readFiles(Cache& cacheName, int idx, double* result,
			   string fileName, char fisrtChar, string commandName);

int main()
{
  cout << endl << endl << endl;
  
  // instantiate object simpleCache
  Cache simpleCache;
  
  int idx = 0;              // store index of letter
  int commandTotal = 0;     // store command total in command file
  
  double result = 0;        // store result from readFiles function
  double elapsed_secs = 0;  // store time of each task
  
  char firstChar;           // convert from string to char; get index of letter
  string firstString;       // first letter of a filename
  string fileName;          // filename of a datafile
  string readData;          // read data from command file
  string commandName;       // store command name from command file
  
  cout << endl;
  
  // prepare to read file and write file
  ifstream infile;
  ofstream outfile;
  
  // open command file and output file
  infile.open(commandFile);
  outfile.open(outputFile);
  
  // check if file is open? Exit if not; otherwise execute
  if (infile.fail()){
	cout << "File failure, exitting program.....";
	cin.get(); // pausing program
	return 0;
  }
  
  // initilize clock timmer
  clock_t begin = clock();

  // get command total in this file
  infile >> commandTotal;
  
  // ignore a blank space after command total
  infile.ignore();

  // read commands file. Check and retrieve value from cache if value is there
  while (getline(infile, readData)){
	// get data filename
	fileName = readData.substr(0, readData.find(" "));
    
	// get command name
	commandName = readData.substr(readData.find(" ")+1, readData.size()-1);
    
	// get first letter of fileName
	firstString = (fileName.substr(0, 1));
    
	// convert to char
	firstChar = firstString[0];
    
	// index of letter
	idx = firstChar - 'A';

	// if data file fail to open, exit program
	if(!checkData(fileName)){
	  return 0;
	}
	
	// Max
	if(commandName == "Max"){	
	  if (simpleCache.checkMax(fileName, firstChar, idx) == NOT_FOUND_MAX_MIN){
		// if data file file to open, exit program
		if(!readFiles(simpleCache, idx, &result,
					  fileName,firstChar, commandName)){
		  return 0;
		}

		// else read data file to find max
		readFiles(simpleCache, idx, &result, fileName, firstChar, commandName);
	  } else {
		// retrieve data from cache
		// cast integer return type to double
		result=static_cast<int>(simpleCache.checkMax(fileName,firstChar,idx));
	  }
	  // print out result to the prompt and result.out file
	  elapsed_secs = double(clock() - begin) / CLOCKS_PER_SEC;

	  cout << fileName <<" Max: " << fixed << setprecision (0) << result
		   << " in " << fixed << setprecision(2)
		   << elapsed_secs << " seconds"<< endl;
	  
	  outfile << fileName <<" Max: " << fixed << setprecision (0) << result
			  << " in " << fixed << setprecision(2)
			  << elapsed_secs << " seconds"<< endl;
	}
	
	// Min
	if (commandName == "Min"){
	  if (simpleCache.checkMin(fileName, firstChar, idx) == NOT_FOUND_MAX_MIN){
		// if data file file to open, exit program
		if(!readFiles(simpleCache, idx, &result,
					  fileName,firstChar, commandName)){
		  return 0;
		}
		
		// else read data file to find min
		readFiles(simpleCache, idx, &result, fileName, firstChar, commandName);
	  } else {
		// retrieve data from cache
		// cast integer return type to double   
		result=static_cast<int>(simpleCache.checkMin(fileName,firstChar,idx));
	  }
	  // print out result to the prompt and result.out file
	  elapsed_secs = double(clock() - begin) / CLOCKS_PER_SEC;

	  cout << fileName <<" Min: " << fixed << setprecision (0) << result
		   << " in " << fixed << setprecision(2)
		   << elapsed_secs << " seconds"<< endl;

	  outfile << fileName <<" Min: " << fixed << setprecision (0) << result
		      << " in " << fixed << setprecision(2)
			  << elapsed_secs << " seconds"<< endl;
	}
	
	// Avg
	if (commandName == "Avg"){
	  if (simpleCache.checkAvg(fileName, firstChar, idx) == NOT_FOUND_AVG_MED){
		// if data file file to open, exit program
		if(!readFiles(simpleCache, idx, &result,
					  fileName,firstChar, commandName)){
		  return 0;
		}
		
		readFiles(simpleCache, idx, &result, fileName, firstChar, commandName);
	  } else {
		// retrieve data from cache
		// cast integer return type to double   
		result = simpleCache.checkAvg(fileName,firstChar,idx);
	  }
	  // print out result to the prompt and result.out file
	  elapsed_secs = double(clock() - begin) / CLOCKS_PER_SEC;

	  cout << fileName <<" Avg: " << fixed << setprecision (1) << result
		   << " in " << fixed << setprecision(2)
		   << elapsed_secs << " seconds"<< endl;

	  outfile << fileName <<" Avg: " << fixed << setprecision (1) << result
		      << " in " << fixed << setprecision(2)
			  << elapsed_secs << " seconds"<< endl;
	}
	
	// Med
	if (commandName == "Med"){
	  if (simpleCache.checkMed(fileName, firstChar, idx) == NOT_FOUND_AVG_MED){
		// if data file file to open, exit program
		if(!readFiles(simpleCache, idx, &result,
					  fileName,firstChar, commandName)){
		  return 0;
		}
		
		readFiles(simpleCache, idx, &result, fileName, firstChar, commandName);
	  } else {
		// retrieve data from cache
		// cast integer return type to double
		result = simpleCache.checkMed(fileName,firstChar,idx);
	  }
	  // print out result to the prompt and result.out file
	  elapsed_secs = double(clock() - begin) / CLOCKS_PER_SEC;

	  cout << fileName <<" Med: " << fixed << setprecision (1) << result
		   << " in " << fixed << setprecision(2)
		   << elapsed_secs << " seconds"<< endl;

	  outfile << fileName <<" Med: " << fixed << setprecision (1) << result
		      << " in " << fixed << setprecision(2)
			  << elapsed_secs << " seconds"<< endl;
	}
  }

  // close files
  infile.close();
  outfile.close();

  // let uses know that result.out is created
  cout << endl << endl << "'result.out' has been created." << endl <<  endl;

  return 0;
}

//check data file function
bool checkData(string fileName)
{
  // ready to read data file
  ifstream infile;

  //get data filename (ex. A.data, A2.data, etc.)
  string dataFile = fileName + ".data";

  //open datafile
  infile.open(dataFile);

  //check if file is open?
  if (infile.fail()){
	cout << "\nFile failure: cannot open " << dataFile
		 << "\nHit enter to exit program....." << endl;
	cin.get(); // pausing program
	return false;
  }

  infile.close();
  return true;
}

//readFiles: use to find max, min, avg, or med if value is not in cache
bool readFiles(Cache& cacheName, int idx, double* result,
			   string fileName, char firstChar, string commandName)
  
{
  // ready to read data file
  ifstream infile;

  // use vector to store value and calculate median
  vector<double> tempArr;
  
  int max = 0;      // store max
  int min = 0;      // store min
  int temp = 0;     // get first number of input file to find max or min
  int count = 0;    // count how many items in a data file
  
  double num = 0;   // read number from file
  double avg = 0;   // store averge of all number in a datafile
  double med = 0;   // store median in a datafile
  double total = 0; // store total of all number
  
  //get data filename (ex. A.data, A2.data, etc.)
  string dataFile = fileName + ".data";
  
  //open datafile
  infile.open(dataFile);
  
  //find max, min, avg, and med
  //get 1st value in data file to find max and min
  infile >> temp;
  max = temp;
  min = temp;

  //update total, count, tempArr
  tempArr.push_back(temp);
  total += temp;
  count++;

  //read data file
  while (infile >> num){
	tempArr.push_back(num);
	total += num;
	count++;
	
	if(max < num){
	  max = num;
	}
	
	if(min >= num){
	  min = num;
	}
  }

  // save max to cache
  if (commandName == "Max"){
	cacheName.saveMax(fileName, idx, max);
	*result = static_cast<int>(max);

  }

  // save min to cache
  if (commandName == "Min"){
	cacheName.saveMin(fileName, idx,  min);
	*result = static_cast<int>(min);
	
  }

  // save average to cache
  if (commandName == "Avg"){
	avg = total/count;
	cacheName.saveAvg(fileName, idx, avg);
	*result = avg;
	
  }

  // find median and save median to cache
  if (commandName == "Med") {
	cacheName.saveMed(fileName, firstChar, tempArr, med, idx, count);
	*result = med;
  }

  // reset values and close data file
  total = 0;
  count = 0;
  tempArr.clear();
  infile.close();
    
  return true;
}
