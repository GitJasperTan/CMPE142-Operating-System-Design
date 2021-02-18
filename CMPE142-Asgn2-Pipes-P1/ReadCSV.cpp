#include <iostream>
#include <cstring>
#include <sys/wait.h>
#include <unistd.h>
#include <string>
using namespace std;

void ReadCSV();

int main() {
    ReadCSV();
}

void ReadCSV() {
	
	string line;
	string token = "";
	char* s;

	while (getline(cin, line)) {

		cout << "<tr>" << endl; //initial <tr>
		cout << "\t<td>"; //This tab may be the end of me

		for (int i = 0; i <= line.size(); i++) {
			if (line[i] == ',') {
				cout << token << "</td>" << "<td>";
				token = "";
			}
			else if(line[i] == line.back()) {
				token += line[i];
                cout << token << "</td>" << endl;
				token = "";
				break;
			}
			else {
				token += line[i];
			}

		}

		cout << "</tr>" << endl; //Ending </tr>
	}

}