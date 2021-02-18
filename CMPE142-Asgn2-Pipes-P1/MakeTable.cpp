#include <iostream>
#include <cstring>
#include <sys/wait.h>
#include <unistd.h>
#include <string>
using namespace std;

void MakeTable();

int main() {
    MakeTable();
}

void MakeTable()
{
	string line = "";

	cout << "<html>" << endl;
	cout << "<body>" << endl;
	cout << "<table border='1'>" << endl;
	cout << "<tr>" << endl;
	cout << "<th>Start</th><th>End</th><th>First</th><th>Middle</th><th>Last</th>" << endl;
	cout << "</tr>" << endl;

	while (getline(cin, line)) cout << line;

	cout << "</table>" << endl;
	cout << "</body>" << endl;
	cout << "</html>" << endl;

	// Close the READ end of the pipe.
}