#include <iostream>
#include <memory>
#include <stdexcept>
#include <thread>
#include <vector>
#include <iomanip>

using namespace std;

//call executable and return std::out
//adapted from http://stackoverflow.com/questions/478898/how-to-execute-a-command-and-get-output-of-command-within-c-using-posix
std::string exec(const char* cmd);

//call executable and write into array
void populate(std::string** target, int i, int j, string (*exec)(const char*));

void runTests();

using namespace std;

int main(int argc, char* argv[])
{
	if (argc == 1) {
		cerr << "Usage: " << argv[0] << " N:integer M:integer" << endl;
		cerr << "Test:  " << argv[0] << " -test" << endl;
		return 1;
	}

	if (string(argv[1]).compare("-test") == 0) {
		runTests();
		return 0;
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
			threadPool.push_back(thread(populate, output, i, j, &exec));

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

void populate(string** target, int i, int j, string(*exec)(const char*))
{
	char cmd[100];
	sprintf_s(cmd, "CallMe.exe %i %i", i, j);
	std::string res = exec(cmd);
	cout << res.c_str() << endl;
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

string execMock(const char* cmd) {
	return "PASS";
}

void test(string header, string actual, string expected) {
	cout << "FUNCTION: " << header.c_str() << endl;
	cout << "ACTUAL:   " << actual.c_str() << endl;
	cout << "EXPECTED: " << expected.c_str() << endl;
	if (actual.compare(expected.c_str()) == 0)
		cout << "_PASS_" << endl;
	else
		cout << "_FAIL_" << endl;
	cout << endl;
}

void runTests() {
	//test 1
	test("exec", exec("echo honk honk honk"), "honk honk honk");

	//test 2
	string** target = new string*[1];
	target[0] = new string[1];
	populate(target, 0, 0, &execMock);
	test("populate", target[0][0], "PASS");
	delete[] target[0];
	delete[] target;
}