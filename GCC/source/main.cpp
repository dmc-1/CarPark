#include <iostream>
#include <fstream>
#include <cstdlib> 
#include <iomanip>
#include <string>
#include <vector>
#include <algorithm>
#include <streambuf>
#include <SDL.h>

using namespace std;

void generateInputFile();
void generateJasonInputFile();
void drawGraph(vector<int> num_cars);
void processSimpleInputFile(vector<int> &num_cars, int slot_length);
void processJsonInputFile(vector<int> &num_cars, int slot_length);


int main(int argc, char *argv[])
{
    //Flag which decides whether the input is from a simple or json format as describe in the challenge instructions. 
    bool simple_input = false; 
       
    //Generate input files in simple or json format as describe in the challenge instructions.
    //if(simple_input){
    //    generateInputFile();   
    //} else {
    //    generateJasonInputFile();
    //}   
    
    /*OBSERVATION_PERIOD is the duration in minutes of the observation period for which the car parking business is observed;
    TIMESLOT_LENGTH is the length of one time interval for which number of cars are counted (a car is counted in a given time slot if its arrival - leave interval intersects
    that time slot)*/
    int observation_days;    
    if(simple_input){
        observation_days = 1;
    } else {
        observation_days = 4;
    }           
	int const OBSERVATION_PERIOD = observation_days * 24 * 60;
	int const TIMESLOT_LENGTH = 30;
	int const NUM_SLOTS = OBSERVATION_PERIOD / TIMESLOT_LENGTH;

    //The processing functions count the number of cars in each timeslot and store the counts in the num_cars vector
	vector<int> num_cars = vector<int>(NUM_SLOTS, 0);   
    if(simple_input){
        processSimpleInputFile(num_cars, TIMESLOT_LENGTH);
    } else {        
        processJsonInputFile(num_cars, TIMESLOT_LENGTH);
    }  
	
    //Print numbers of cars (car park business) in the time slots throughout the observation period.
	cout << "Number of cars by " << TIMESLOT_LENGTH << " minute time slots:\n";
	for (int i = 0; i < num_cars.size(); i++) {
		cout << num_cars[i] << ' ';
	}
	cout << "\n";
        
    //Drawes the graph of the parking lot business. Uses SDL library
	drawGraph(num_cars);

    return 0;
}

//Generates a simple input file as described in the challenge instructions.  
void generateInputFile() {
	ofstream infile;
	infile.open("input/input.txt");
	for (int i = 0; i < 100; i++) {
		int startHour = rand() % 24;
		int startMinute = rand() % 60;
		int startTime = startHour * 60 + startMinute;
		int stayTime = rand() % (24 * 60 - startTime);
		int endTime = startTime + stayTime;
		int endHour = endTime / 60;
		int endMinute = endTime % 60;
		infile << setfill('0') << setw(2) << startHour << ":" 
			<< setfill('0') << setw(2) << startMinute  << " " 
			<< setfill('0') << setw(2) << endHour << ":" 
			<< setfill('0') << setw(2) << endMinute << "\n";
	}
	infile.close();
}

/*Generates a json input file as described in the challenge instructions. The start time for a car can be 2016-05-01 00:00,
the stay time is random time in minutes of up to 1 day.*/  
void generateJasonInputFile() {
	ofstream infile;
	infile.open("input/json.txt");
	infile << "[";
	for (int i = 0; i < 100; i++) {
		int startDay = rand() % 3 + 1;
		int startHour = rand() % 24;
		int startMinute = rand() % 60;
		int startTime = (startDay - 1) * 24 * 60 + startHour * 60 + startMinute;
		int stayTime = rand() % (1 * 24 * 60);
		int endTime = startTime + stayTime;
		int endDay = (endTime / (24 * 60)) + 1;
		int endHour = (endTime % (24 * 60)) / 60;
		int endMinute = (endTime % (24 * 60)) % 60;
		infile << "{\n"
			<< "\t\t\"Id\": " << i << ",\n"
			<< "\t\t\"ArrivalTime\": \"2016-05-"
			<< setfill('0') << setw(2) << startDay << "T"
			<< setfill('0') << setw(2) << startHour << ":"
			<< setfill('0') << setw(2) << startMinute << ":00\",\n"
			<< "\t\t\"LeaveTime\": \"2016-05-"
			<< setfill('0') << setw(2) << endDay << "T"
			<< setfill('0') << setw(2) << endHour << ":"
			<< setfill('0') << setw(2) << endMinute << ":00\"\n"
		    << "}";
		if (i < 100 - 1)	
			infile << ", ";
	}
	infile << "]";
	infile.close();
}

/*Processes the simple input file, counts numbers of cars in time slots of specified length, and stores the counts in num_cars vector. 
It is assumed that the observation was made during 24 hours time. The function reads the file line by line and extracts time interval of car's stay, 
then the count of cars in all timeslots intersected with this interval is incremented. Finally the function finds the highest number in 
num_cars vector which corresponds to the busiest time and prints out the result.*/
void processSimpleInputFile(vector<int> &num_cars, int slot_length) {
	ifstream infile;
	infile.open("input/input.txt");
	string str;
	while (infile.peek() != EOF) {
		getline(infile, str, ':');
		int start_hour = stoi(str);
		getline(infile, str, ' ');
		int start_minute = stoi(str);
		getline(infile, str, ':');
		int end_hour = stoi(str);
		getline(infile, str, '\n');
		int end_minute = stoi(str);

		int start_time = start_hour * 60 + start_minute;
		int start_slot = start_time / slot_length;
		int end_time = end_hour * 60 + end_minute;
		int end_slot = end_time / slot_length;

		for (int i = start_slot; i < end_slot + 1; i++) {
			num_cars[i] += 1;
		}
	}

	int max_slot = 0;
	int max_number = 0;
	for (int i = 0; i < num_cars.size(); i++) {
		if (num_cars[i] > max_number) {
			max_slot = i;
			max_number = num_cars[i];
		}
	}
	int busiest_time = max_slot * slot_length;
	int busiest_hour = busiest_time / 60;
	int busiest_minute = busiest_time % 60;

	cout << "\nThe busiest time is " << setfill('0') << setw(2) << busiest_hour << ":" 
		<< setfill('0') << setw(2) << busiest_minute << " " << " with number of cars " << max_number << "\n";
}

/*This works similar to the previous function. However, here the assumption is that observation was made for over several days. Besides the input
is read from a json file. The parsing logic of the json file is hardcoded. It is assumed that the json file is exactly in the format as described in the challenge*/
void processJsonInputFile(vector<int> &num_cars, int slot_length) {
	ifstream buffer;
	buffer.open("input/json.txt");
	string str;
	while (buffer.peek() != ']') {
		getline(buffer, str, 'T');
		getline(buffer, str, 'T');
		int start_day = stoi(str.substr(str.length() - 2));
		getline(buffer, str, ':');
		int start_hour = stoi(str);
		getline(buffer, str, ':');
		int start_minute = stoi(str);
		getline(buffer, str, 'T');
		getline(buffer, str, 'T');
		int end_day = stoi(str.substr(str.length() - 2));
		getline(buffer, str, ':');
		int end_hour = stoi(str);
		getline(buffer, str, ':');
		int end_minute = stoi(str);
		getline(buffer, str, '}');

		int start_time = (start_day - 1) * 24 * 60 + start_hour * 60 + start_minute;
		int start_slot = start_time / slot_length;
		int end_time = (end_day - 1) * 24 * 60 + end_hour * 60 + end_minute;
		int end_slot = end_time / slot_length;

		for (int i = start_slot; i < end_slot + 1; i++) {
			num_cars[i] += 1;
		}
	}

	int max_slot = 0;
	int max_number = 0;
	for (int i = 0; i < num_cars.size(); i++) {
		if (num_cars[i] > max_number) {
			max_slot = i;
			max_number = num_cars[i];
		}
	}
	int busiest_time = max_slot * slot_length;
	int busiest_day = busiest_time / (24 * 60) + 1;
	int busiest_hour = (busiest_time % (24 * 60) ) / 60;
	int busiest_minute = busiest_time % 60;

	cout << "The busiest time was "
		<< "2016-05-"
		<< setfill('0') << setw(2) << busiest_day << " " 
		<< setfill('0') << setw(2) << busiest_hour << ":" 
		<< setfill('0') << setw(2) << busiest_minute  
		<< " with number of cars " << max_number << "\n";
}

/*I used SDL library to plot the business graph. The function takes a vector which contains parking lot business numbers and plots a graph.
It shows a graph, and waits for 5 seconds before it closes*/
void drawGraph(vector<int> num_cars) {
	if (SDL_Init(SDL_INIT_VIDEO) == 0) {
		SDL_Window* window = NULL;
		SDL_Renderer* renderer = NULL;

		int const L_SCALE = 5;
		int const H_SCALE = 10;
		int const WIN_HEIGHT = 500;

		if (SDL_CreateWindowAndRenderer(num_cars.size() * L_SCALE, WIN_HEIGHT, 0, &window, &renderer) == 0) {
			SDL_bool done = SDL_FALSE;

            SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
			SDL_RenderClear(renderer);
            
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
			for (int i = 0; i < num_cars.size() - 1; i++) {
				SDL_RenderDrawLine(renderer, i * L_SCALE, WIN_HEIGHT - num_cars[i] * H_SCALE, (i + 1) * 5, WIN_HEIGHT - num_cars[i + 1] * 10);
			}

			SDL_RenderPresent(renderer);
            
            //Wait for five seconds before wait.            
            SDL_Delay( 5000 );
                
		}

		if (renderer) {
			SDL_DestroyRenderer(renderer);
		}
		if (window) {
			SDL_DestroyWindow(window);
		}
	}
	SDL_Quit();

}







