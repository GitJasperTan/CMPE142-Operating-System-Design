#include <iostream>
#include <time.h>
#include <deque>
using namespace std;

struct age_queue_struct {
	int value;
	int last_referenced;

	age_queue_struct() {
		value = -1;
		last_referenced = 0;
	}

};

int reference_string[5][100];
int hit_ratio_arr[6][5];
int page_swaps_arr[6][5];
deque<int> age_que;
deque<age_queue_struct> age_queue;
void randomize_reference_strings();
void FIFO();
void LRU();
void LFU();
void MFU();
void random_pick();
void optimal_pick();
void print_statistics();


void randomize_reference_strings() {

	int i = 0;
	int r = 0;

	for (int k = 0; k < 5; k++) {
		for (int j = 0; j < 100; j++) {

			if (j == 0) {
				i = rand() % 8;
				reference_string[k][j] = i;
				//Used for diagnostic purposes
				//printf("Index %i | r = %i | i = %i\n", j, r, i);
				continue;
			}

			r = rand() % 10;
			if (r == 0 || r == 1) { //i-1
				if (i != 0) { //Corner case, ensures i != -1
					i = i - 1;
					reference_string[k][j] = i;
				}
				else reference_string[k][j] = i;
			}
			else if (r == 2 || r == 3) { //i = i
				reference_string[k][j] = i;
			}
			else if (r == 4 || r == 5) { //i + 1
				if (i != 7) { //Corner case, ensures i != 8
					i = i + 1;
					reference_string[k][j] = 1;
				}
				else reference_string[k][j] = i;
			}
			else if (r >= 6 && r <= 9) {
				i = rand() % 8;
				reference_string[k][j] = i;
			}
			else {
				printf("ERROR: r value should not be lower than 0 or higher than 9\n");
				exit(1);
			}

			//Used for diagnostic purposes
			//printf("Index %i | r = %i | i = %i\n", j, r, i);

		}
	}
}

void FIFO() {

	cout << "******************************" << endl;
	cout << "Beginning FIFO Simulation" << endl;

	for (int k = 0; k < 5; k++) {
		cout << endl << "Simulation #" << k + 1 << endl;
		cout << "Index \t Ref \t Frames \t Action" << endl;

		int page_frames[3] = { -1, -1, -1 };
		int current_value = 0;
		int oldest_reference_string = 0;
		int page_swaps = 0;
		int hit_ratio = 0;
		bool eviction = false;

		for (int j = 0; j < 100; j++) {

			current_value = reference_string[k][j];

			//Checks if current_value is already in the age_que
			for (int i = 0; i < age_que.size(); i++) { //might need que.size() + 1
				if (age_que[i] == current_value) { //If current_value is in the queue, we dont need to place it in queue
					hit_ratio++;
					hit_ratio_arr[0][k]++;
					goto end;
				}
			}
			age_que.push_back(current_value);


			//If current reference_string is already in the page frames, skip to next reference_string
			for (int i = 0; i < 3; i++) {
				if (page_frames[i] == current_value) goto end;
			}

			//From hereon we can assume that current_value is not in the page frames
			//Looks for an empty slot in the page_frames
			for (int i = 0; i < 3; i++) {
				if (page_frames[i] == -1) {
					page_frames[i] = current_value;
					goto end;
				}
			}

			//From hereon we can assume that there are no empty slots, therefore we must push the oldest reference_string out
			oldest_reference_string = age_que.front();
			age_que.pop_front();
			for (int i = 0; i < 3; i++) {
				if (page_frames[i] == oldest_reference_string) {
					page_frames[i] = current_value;
					page_swaps++;
					page_swaps_arr[0][k]++;
					eviction = true;
				}
			}



		end:
			
			//Print
			printf("i:%i \t %i \t %i %i %i \t\t", j, current_value, page_frames[0], page_frames[1], page_frames[2]);
			if (eviction) {
				printf("%i evicted, %i entered\n", oldest_reference_string, current_value);
			}
			else cout << endl;

			eviction = false;
		} //End of inner for loop
		cout << "Simulation #" << k+1 << " Statistics - " << "Hit Ratio: " << hit_ratio << "% Page Swaps: " << page_swaps << endl;

		age_que.clear();
	}//End of outer for loop
	cout << endl << "Ending FIFO Simulation" << endl;
	cout << "******************************" << endl;
}

void LRU() {
	cout << "******************************" << endl;
	cout << "Beginning LRU Simulation" << endl;

	for (int k = 0; k < 5; k++) {
		cout << endl << "Simulation #" << k + 1 << endl;
		cout << "Index \t Ref \t Frames \t Action" << endl;

		int current_value = 0;
		int page_frames[3] = { -1, -1, -1 };
		int oldest_reference_string = 0;
		int page_swaps = 0;
		int hit_ratio = 0;
		int swap_temp = 0;
		bool eviction = false;

		for (int j = 0; j < 100; j++) {

			current_value = reference_string[k][j];

			//Checks if current_value is already in the age_que
			for (int i = 0; i < age_que.size(); i++) { //might need que.size() + 1
				if (age_que[i] == current_value) { //If current_value is in the queue, we dont need to place it in queue

					//If current value is already in the age queue
					//push it back to the front
					//front == most recently referenced
					swap_temp = age_que[i];
					age_que.erase(age_que.begin() + i);
					age_que.push_front(swap_temp);


					hit_ratio++;
					hit_ratio_arr[1][k]++;
					goto end;
				}
			}
			age_que.push_front(current_value);


			//If current reference_string is already in the page frames, skip to next reference_string
			for (int i = 0; i < 3; i++) {
				if (page_frames[i] == current_value) goto end;
			}

			//From hereon we can assume that current_value is not in the page frames
			//Looks for an empty slot in the page_frames
			for (int i = 0; i < 3; i++) {
				if (page_frames[i] == -1) {
					page_frames[i] = current_value;
					goto end;
				}
			}

			//From hereon we can assume that there are no empty slots, therefore we must push the longest non referenced object
			oldest_reference_string = age_que.back();
			age_que.pop_back();
			for (int i = 0; i < 3; i++) {
				if (page_frames[i] == oldest_reference_string) {
					page_frames[i] = current_value;
					page_swaps++;
					page_swaps_arr[1][k]++;
					eviction = true;
				}
			}



		end:
			
			//Print
			printf("i:%i \t %i \t %i %i %i \t\t", j, current_value, page_frames[0], page_frames[1], page_frames[2]);
			if (eviction) {
				printf("%i evicted, %i entered\n", oldest_reference_string, current_value);
			}
			else cout << endl;

			eviction = false;
		} //End of inner for loop
		cout << "Simulation #" << k+1 << " Statistics - " << "Hit Ratio: " << hit_ratio << "% Page Swaps: " << page_swaps << endl;

		age_que.clear();
	}//End of outer for loop
	cout << endl << "Ending LRU Simulation" << endl;
	cout << "******************************" << endl;
}

void LFU() {
	cout << "******************************" << endl;
	cout << "Beginning LFU Simulation" << endl;


	for (int k = 0; k < 5; k++) {
		cout << endl << "Simulation #" << k + 1 << endl;
		cout << "Index \t Ref \t Frames \t Action" << endl;

		age_queue_struct page_frames[3];
		age_queue_struct lowest;
		int hit_ratio = 0;
		int page_swaps = 0;
		bool eviction = false;


		for (int j = 0 ; j < 100; j++) {

			age_queue_struct temp_struct;
			temp_struct.value = reference_string[k][j];
			temp_struct.last_referenced = 0;

			//if current reference string is already in the page frame, skip to next reference string
			for (int i = 0; i < 3; i++) {
				if (page_frames[i].value == temp_struct.value) {
					page_frames[i].last_referenced++; 
					
					hit_ratio++;
					hit_ratio_arr[2][k]++;
					goto end;
				}
			}

			//Fromo hereon we can assume that temp_struct is noot in the page frames
			//Looks for an empty slot in the page_Frames
			for (int i = 0; i < 3; i++) {
				if (page_frames[i].value == -1) {
					page_frames[i] = temp_struct;
					goto end;
				}
			}
			
			lowest = page_frames[0];
			for (int i = 1; i < 3; i++) {
				if (page_frames[i].last_referenced < lowest.last_referenced) lowest = page_frames[i];
			}

			//From here we can assume that there are no empty slots, therefore we must push the least frequently used.
			for (int i = 0; i < 3; i++) {
				if (page_frames[i].value == lowest.value) {
					page_frames[i] = temp_struct;
					page_swaps++;
					page_swaps_arr[2][k]++;
					eviction = true;
				}
			}

			
			
			end:
			//Print
			printf("i:%i \t %i \t %i %i %i \t\t", j, temp_struct.value, page_frames[0].value, page_frames[1].value, page_frames[2].value);
			if (eviction) {
				printf("%i evicted, %i entered\n", lowest.value, temp_struct.value);
			}
			else cout << endl;

			eviction = false;
		}//end of inner for loop
		cout << "Simulation #" << k+1 << " Statistics - " << "Hit Ratio: " << hit_ratio << "% Page Swaps: " << page_swaps << endl;
	}//end of outer for loop


	cout << endl << "Ending LFU Simulation" << endl;
	cout << "******************************" << endl;
}

void MFU() {
	cout << "******************************" << endl;
	cout << "Beginning MFU Simulation" << endl;


	for (int k = 0; k < 5; k++) {
		cout << endl << "Simulation #" << k + 1 << endl;
		cout << "Index \t Ref \t Frames \t Action" << endl;

		age_queue_struct page_frames[3];
		age_queue_struct highest;
		int hit_ratio = 0;
		int page_swaps = 0;
		bool eviction = false;


		for (int j = 0 ; j < 100; j++) {

			age_queue_struct temp_struct;
			temp_struct.value = reference_string[k][j];
			temp_struct.last_referenced = 0;

			//if current reference string is already in the page frame, skip to next reference string
			for (int i = 0; i < 3; i++) {
				if (page_frames[i].value == temp_struct.value) {
					page_frames[i].last_referenced++; 
					
					hit_ratio++;
					hit_ratio_arr[3][k]++;
					goto end;
				}
			}

			//Fromo hereon we can assume that temp_struct is noot in the page frames
			//Looks for an empty slot in the page_Frames
			for (int i = 0; i < 3; i++) {
				if (page_frames[i].value == -1) {
					page_frames[i] = temp_struct;
					goto end;
				}
			}
			
			highest = page_frames[0];
			for (int i = 1; i < 3; i++) {
				if (page_frames[i].last_referenced > highest.last_referenced) highest = page_frames[i];
			}

			//From here we can assume that there are no empty slots, therefore we must pop the most frequently used.
			for (int i = 0; i < 3; i++) {
				if (page_frames[i].value == highest.value) {
					page_frames[i] = temp_struct;
					page_swaps++;
					page_swaps_arr[3][k]++;
					eviction = true;
				}
			}

			
			
			end:
			//Print
			printf("i:%i \t %i \t %i %i %i \t\t", j, temp_struct.value, page_frames[0].value, page_frames[1].value, page_frames[2].value);
			if (eviction) {
				printf("%i evicted, %i entered\n", highest.value, temp_struct.value);
			}
			else cout << endl;

			eviction = false;
		}//end of inner for loop
		cout << "Simulation #" << k+1 << " Statistics - " << "Hit Ratio: " << hit_ratio << "% Page Swaps: " << page_swaps << endl;
	}//end of outer for loop


	cout << endl << "Ending MFU Simulation" << endl;
	cout << "******************************" << endl;
}

void random_pick() {
	cout << "******************************" << endl;
	cout << "Beginning Random Pick Simulation" << endl;

	for (int k = 0; k < 5; k++) {
		cout << endl << "Simulation #" << k + 1 << endl;
		cout << "Index \t Ref \t Frames \t Action" << endl;

		age_queue_struct page_frames[3];
		int hit_ratio = 0;
		int page_swaps = 0;
		int prev_value = 0;
		bool eviction = false;
		int rand_index = 0;


		for (int j = 0 ; j < 100; j++) {

			age_queue_struct temp_struct;
			temp_struct.value = reference_string[k][j];
			temp_struct.last_referenced = 0;

			//if current reference string is already in the page frame, skip to next reference string
			for (int i = 0; i < 3; i++) {
				if (page_frames[i].value == temp_struct.value) {
					page_frames[i].last_referenced++; 
					
					hit_ratio++;
					hit_ratio_arr[4][k]++;
					goto end;
				}
			}

			//Fromo hereon we can assume that temp_struct is noot in the page frames
			//Looks for an empty slot in the page_Frames
			for (int i = 0; i < 3; i++) {
				if (page_frames[i].value == -1) {
					page_frames[i] = temp_struct;
					goto end;
				}
			}
			
			//From here we can assume that there are no empty slots, therefore we must pop the most frequently used.
			rand_index = rand() % 3;
			prev_value = page_frames[rand_index].value;
			page_frames[rand_index] = temp_struct;
			page_swaps++;
			page_swaps_arr[4][k]++;
			eviction = true;
			
			end:
			//Print
			printf("i:%i \t %i \t %i %i %i \t\t", j, temp_struct.value, page_frames[0].value, page_frames[1].value, page_frames[2].value);
			if (eviction) {
				printf("%i evicted, %i entered\n", prev_value, temp_struct.value);
			}
			else cout << endl;

			eviction = false;
		}//end of inner for loop
		cout << "Simulation #" << k+1 << " Statistics - " << "Hit Ratio: " << hit_ratio << "% Page Swaps: " << page_swaps << endl;
	}//end of outer for loop




	cout << endl << "Ending Random Pick Simulation" << endl;
	cout << "******************************" << endl;
}

void optimal_pick() {
	cout << "******************************" << endl;
	cout << "Beginning Optimal Pick Simulation" << endl;


	for (int k = 0; k < 5; k++) {
		cout << endl << "Simulation #" << k + 1 << endl;
		cout << "Index \t Ref \t Frames \t Action" << endl;

		age_queue_struct page_frames[3];
		age_queue_struct highest;

		int hit_ratio = 0;
		int page_swaps = 0;
		bool eviction = false;


		for (int j = 0 ; j < 100; j++) {

			age_queue_struct temp_struct;
			temp_struct.value = reference_string[k][j];
			temp_struct.last_referenced = 0;

			//if current reference string is already in the page frame, skip to next reference string
			for (int i = 0; i < 3; i++) {
				if (page_frames[i].value == temp_struct.value) {
					//page_frames[i].last_referenced++; 
					
					hit_ratio++;
					hit_ratio_arr[5][k]++;
					goto end;
				}
			}

			//Fromo hereon we can assume that temp_struct is noot in the page frames
			//Looks for an empty slot in the page_Frames
			for (int i = 0; i < 3; i++) {
				if (page_frames[i].value == -1) {
					page_frames[i] = temp_struct;
					goto end;
				}
			}
			
			//Look for the next occurrence of each page value
			for (int i = 0; i < 3; i++) {
				int counter = 0;
				for (int x = j; x < 100; x++) {
					if (page_frames[i].value == reference_string[k][x]) {
						//page_frames[i].last_referenced = counter;
						break;
					}
					else counter++;
				}
				page_frames[i].last_referenced = counter;
			}

			highest = page_frames[0];
			for (int i = 1; i < 3; i++) {
				if (page_frames[i].last_referenced > highest.last_referenced) highest = page_frames[i];
			}
			

			//From here we can assume that there are no empty slots, therefore we must pop
			for (int i = 0; i < 3; i++) {
				if (page_frames[i].value == highest.value) {
					page_frames[i] = temp_struct;
					page_swaps++;
					page_swaps_arr[5][k]++;
					eviction = true;
				}
			}

			end:
			//Print
			printf("i:%i \t %i \t %i %i %i \t\t", j, temp_struct.value, page_frames[0].value, page_frames[1].value, page_frames[2].value);
			if (eviction) {
				printf("%i evicted, %i entered\n", highest.value, temp_struct.value);
			}
			else cout << endl;

			eviction = false;
		}//end of inner for loop
		cout << "Simulation #" << k+1 << " Statistics - " << "Hit Ratio: " << hit_ratio << "% Page Swaps: " << page_swaps << endl;
	}//end of outer for loop


	cout << endl << "Ending Optimal Pick Simulation" << endl;
	cout << "******************************" << endl;
}

void print_statistics() {

	cout << endl << "******************************" << endl;
	cout << "Beginning of Statistics" << endl << endl;

	float temp_hit = 0;
	float temp_swap = 0;

	printf("Algorithm \t Hit Ratio \t Page Swaps \n");

	//FIFO 
	for (int i = 0; i < 5; i++) {
		printf("FIFO \t\t %i \t\t %i \n", hit_ratio_arr[0][i], page_swaps_arr[0][i]);
		temp_hit += hit_ratio_arr[0][i];
		temp_swap += page_swaps_arr[0][i]; 
	}
	temp_hit /= 5;
	temp_swap /= 5;
	printf("FIFO Averages \t %.2f \t\t %.2f \n", temp_hit, temp_swap);
	cout << endl;
	temp_hit = 0;
	temp_swap = 0;


	//LRU
	for (int i = 0; i < 5; i++) {
		printf("LRU \t\t %i \t\t %i \n", hit_ratio_arr[1][i], page_swaps_arr[1][i]);
		temp_hit += hit_ratio_arr[1][i];
		temp_swap += page_swaps_arr[1][i]; 
		
	}
	temp_hit /= 5;
	temp_swap /= 5;
	printf("LRU Averages \t %.2f \t\t %.2f \n", temp_hit, temp_swap);
	cout << endl;
	temp_hit = 0;
	temp_swap = 0;


	//LFU
	for (int i = 0; i < 5; i++) {
		printf("LFU \t\t %i \t\t %i \n", hit_ratio_arr[2][i], page_swaps_arr[2][i]);
		temp_hit += hit_ratio_arr[2][i];
		temp_swap += page_swaps_arr[2][i]; 
	}
	temp_hit /= 5;
	temp_swap /= 5;
	printf("LFU Averages \t %.2f \t\t %.2f \n", temp_hit, temp_swap);
	cout << endl;
	temp_hit = 0;
	temp_swap = 0;


	//MFU
	for (int i = 0; i < 5; i++) {
		printf("MFU \t\t %i \t\t %i \n", hit_ratio_arr[3][i], page_swaps_arr[3][i]);
		temp_hit += hit_ratio_arr[3][i];
		temp_swap += page_swaps_arr[3][i]; 
	}
	temp_hit /= 5;
	temp_swap /= 5;
	printf("MFU Averages \t %.2f \t\t %.2f \n", temp_hit, temp_swap);
	cout << endl;
	temp_hit = 0;
	temp_swap = 0;


	//Random
	for (int i = 0; i < 5; i++) {
		printf("Random \t\t %i \t\t %i \n", hit_ratio_arr[4][i], page_swaps_arr[4][i]);
		temp_hit += hit_ratio_arr[4][i];
		temp_swap += page_swaps_arr[4][i]; 
	}
	temp_hit /= 5;
	temp_swap /= 5;
	printf("Random Averages  %.2f \t\t %.2f \n", temp_hit, temp_swap);
	cout << endl;
	temp_hit = 0;
	temp_swap = 0;


	//Optimal
	for (int i = 0; i < 5; i++) {
		printf("Optimal \t %i \t\t %i \n", hit_ratio_arr[5][i], page_swaps_arr[5][i]);
		temp_hit += hit_ratio_arr[5][i];
		temp_swap += page_swaps_arr[5][i]; 
	}
	temp_hit /= 5;
	temp_swap /= 5;
	printf("Optimal Averages %.2f\t\t %.2f \n", temp_hit, temp_swap);
	cout << endl;
	temp_hit = 0;
	temp_swap = 0;





	cout << "End of Statistics" << endl;
	cout << "******************************" << endl;
}
int main() {
	srand(time(NULL));
	randomize_reference_strings();

	
	FIFO();
	LRU();
	LFU();
	MFU();
	random_pick();
	optimal_pick();

	print_statistics();
}