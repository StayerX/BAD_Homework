#include <iostream>
#include <memory>
#include <stdexcept>
#include <thread>
#include <vector>
#include <iomanip>
//call executable and return std::out
//adapted from http://stackoverflow.com/questions/478898/how-to-execute-a-command-and-get-output-of-command-within-c-using-posix
std::string exec(const char* cmd);

//call executable and write into array
void populate(std::string** target, int i, int j);

using namespace std;

int main(int argc, char* argv[])
{
	if (argc < 3) {
		cerr << "Usage: " << argv[0] << " N:integer M:integer" << endl;
		return 1;
	}

	int n = atoi(argv[1]);
	int m = atoi(argv[2]);

	string** output = new string*[n];
	for (int i = 0; i < n; i++)
		output[i] = new string[m];

	vector<thread> threadPool;

	//launch N*M threads to populate matrix
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			threadPool.push_back(thread(populate, output, i, j));

	//wait for all to finish
	for (int i = 0; i < n * m; i++)
		threadPool[i].join();

	//print
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			cout << setw(5) << output[i][j].c_str() << " ";
		}
		cout << endl;
	}
	
	//cleanup
	for (int i = 0; i < n; i++)
		delete[] output[i];
	delete[] output;
}

void populate(string** target, int i, int j)
{
	char cmd[100];
	sprintf_s(cmd, "CallMe.exe %i %i", i, j);
	std::string res = exec(cmd);
	target[i][j] = (res.length() == 0) ? "-inf" : res; //nothing printed to std::out when segfault occurs
}

#ifdef _WIN32
#define popen _popen
#define pclose _pclose
std::string exec(const char* cmd) 
{
	char buffer[128];
	std::string result = "";
	FILE* pipe = popen(cmd, "r");
	if (!pipe) throw std::runtime_error("popen() failed!");
	try {
		while (!feof(pipe)) {
			if (fgets(buffer, 128, pipe) != NULL)
				result += buffer;
		}
	}
	catch (...) {
		pclose(pipe);
		throw;
	}
	pclose(pipe);
	return result.substr(0, result.length() - 1);
}
#endif
