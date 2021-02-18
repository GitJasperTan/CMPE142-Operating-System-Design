#include <iostream>
#include <cstring>
#include <sys/wait.h>
#include <unistd.h>
#include <string>
using namespace std;

#define BUFFER_SIZE 256
enum {READ_END = 0, WRITE_END = 1};

void parent(int fd[]);
void child(int fd[]);


int main() {

	int fd[2]; //File Descriptor for Pipe
	pid_t pipe_id = pipe(fd);

	//Create the Pipe
	if (pipe_id << 0) {
		fprintf(stderr, "pipe() failed");
		return pipe_id;
	}

	pid_t pid = fork();
	if (pid > 0) parent(fd);
	else if (pid == 0) child(fd);
	else {
		fprintf(stderr, "fork() failed");
		return pid;
	}

}



char * conversion(string s, int n) {
	char * char_arr = new char[n + 1];
	strcpy(char_arr, s.c_str());
	return char_arr;
}

void child(int fd[])
{
	char buffer[BUFFER_SIZE];

	close(fd[WRITE_END]);
	ssize_t size;

	cout << "<html>" << endl;
	cout << "<body>" << endl;
	cout << "<table border='1'>" << endl;
	cout << "<tr>" << endl;
	cout << "<th>Start</th><th>End</th><th>First</th><th>Middle</th><th>Last</th>" << endl;
	cout << "</tr>" << endl;

	for (;;)
	{
		size = read(fd[READ_END], buffer, BUFFER_SIZE);
		if (size <= 0) break;

		if (size < BUFFER_SIZE) buffer[size] = '\0';
		printf("%s", buffer);
	}

	cout << "</table>" << endl;
	cout << "</body>" << endl;
	cout << "</html>" << endl;

	// Close the READ end of the pipe.
	close(fd[READ_END]);
}

void parent(int fd[]) {
	
	string line;
	string token = "";
	string total = "";
	char* s;

	close(fd[READ_END]);

	while (getline(cin, line, '\n')) {

		//cout << "<tr>" << endl; //initial <tr>
		total += "<tr>\n";
		s = conversion(total, total.size());
		write(fd[WRITE_END], s, strlen(s));
		delete(s);
		total = "";

		total += "\t<td>";
		//cout << "\t<td>"; //This tab may be the end of me

		for (int i = 0; i <= line.size(); i++) {
			if (line[i] == ',') {
				total += token + "</td>" + "<td>";
				//cout << token << "</td>" << "<td>";
				token = "";
			}
			else if(line[i] == line.back()) {
				token += line[i];
				total += token + "</td>\n";
				s = conversion(total, total.size());
				write(fd[WRITE_END], s, strlen(s));
				delete(s);

				total = "</tr>\n";
				s = conversion(total, total.size());
				write(fd[WRITE_END], s, strlen(s));
				delete(s);

				total = "";
				token = "";
				break;
			}
			else {
				token += line[i];
			}

		}

		//cout << "</tr>" << endl; //Ending </tr>
	}

	close(fd[WRITE_END]);

}
