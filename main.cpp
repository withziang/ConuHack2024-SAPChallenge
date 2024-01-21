#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include <list>
#include <limits>
#include <map>
#include <cmath>
#include <deque>
#include <utility>
#include <functional>
#include <fstream>
#include <sstream>
#include <set>
#include <cstring>

using namespace std;

#define FOR(i,a,b)   for(int i = a; i < b.size(); ++i)
#define F0(i,b)      for(int i = 0; i< b.size();++i)
#define BE(a)        a.begin(), a.end()
#define __S(a)       a.size()
#define Pls(a)       for (auto b:a) cout << b << " ";  cout << endl;



typedef long long ll;
typedef vector<int> vi;
typedef vector<vector<int>> vvi;
typedef vector <long long> vl;
typedef vector<vector<long long>> vvl;

#ifndef SAP
#define SAP

//user struct define, plus comparison
struct user {
    int date_phone_call_list[5];
    int date_reservation_list[5];
    int type; /*1: compact car
                2: medium car
                3: full_size
                4: class 1
                5: class 2*/

    user() = default;
    user(int date_call[5], int date_res[5], int car_type) {
        for (int i = 0; i < 5; ++i) {
            date_phone_call_list[i] = date_call[i];
            date_reservation_list[i] = date_res[i];
        }
        type = car_type;
    }

    bool operator<(const user& first) const {
        for (int i = 0; i < 5; ++i) {
            if (date_phone_call_list[i] != first.date_phone_call_list[i])
                return date_phone_call_list[i] < first.date_phone_call_list[i];
        }
        return false;
    }
};


//update slot
void update_slot(vector<vector<vector<vector<bool>>>>& time_schedule_true_and_false_table, int type, const int time[5], map <int, pair<int, int>> type_to_cost_and_time, int slot){
	int time_in_min_start = time[3]*60+time[4];
	for (int i =time_in_min_start; i< (time_in_min_start+type_to_cost_and_time[type].second);i++){
		time_schedule_true_and_false_table[time[2]+31*(time[1]-10)-1][i/60-7][i%60-1][slot] = 1;
	}
}


int check_slot_available_ang_update(vector <vector<vector<vector<bool>>>>& time_schedule_true_and_false_table,
						int type, const int time[5], map <int, pair<int, int>> type_to_cost_and_time){
	//2022:10:1:7:0 to 2022:11:30:19:0
	// ex: 2022:11:25:2:56
	// (31+30) month * 12 (hours) * 60 (mins)* 5
	// 10 stations, 5 specific ones and 5 extra, check the 
	bool ava[6]; // 5 extra plus 1 fixed
	memset(ava, true, sizeof(ava));

	//turn everything to min to loup through
	int time_in_min_start = time[3]*60+time[4];
	// 0 means unoccupied

	for (int i =time_in_min_start; i< (time_in_min_start+type_to_cost_and_time[type].second);i++){
		//special
		if (ava[0] && time_schedule_true_and_false_table[time[2]+31*(time[1]-10)-1][i/60-7][i%60][type]) ava[0] = 0;
		//5 to 10
		if (ava[1] && time_schedule_true_and_false_table[time[2]+31*(time[1]-10)-1][i/60-7][i%60][5]) ava[1] = 0;
		if (ava[2] && time_schedule_true_and_false_table[time[2]+31*(time[1]-10)-1][i/60-7][i%60][6]) ava[2] = 0;
		if (ava[3] && time_schedule_true_and_false_table[time[2]+31*(time[1]-10)-1][i/60-7][i%60][7]) ava[3] = 0;
		if (ava[4] && time_schedule_true_and_false_table[time[2]+31*(time[1]-10)-1][i/60-7][i%60][8]) ava[4] = 0;
		if (ava[5] && time_schedule_true_and_false_table[time[2]+31*(time[1]-10)-1][i/60-7][i%60][9]) ava[5] = 0;
	}

	//final answer
	bool final = 0;
	for (int i = 0;i < 6;i++) final = final || ava[i];


	if (final){
		if (ava[0]){
			update_slot(time_schedule_true_and_false_table, type, time, type_to_cost_and_time, type);
			return type+1;
		}
		else if (ava[1]){
			update_slot(time_schedule_true_and_false_table, type, time, type_to_cost_and_time, 5);
			return 6;
		}
		else if (ava[2]){
			update_slot(time_schedule_true_and_false_table, type, time, type_to_cost_and_time, 6);
			return 7;
		}
		else if (ava[3]){
			update_slot(time_schedule_true_and_false_table, type, time, type_to_cost_and_time, 7);
			return 8;
		}
		else if (ava[4]){
			update_slot(time_schedule_true_and_false_table, type, time, type_to_cost_and_time, 8);
			return 9;
		}
		else if (ava[5]){
			update_slot(time_schedule_true_and_false_table, type, time, type_to_cost_and_time, 9);
			return 10;
		}
	}

	return 0;

}


int main(){

    //automatically create output file txt, change input file direction, change output file location if needed
	/*cout << "Input csv file directory: " << endl;  */
	const char* filePathin = R"(C:\Users\ziang\Downloads\datafile.csv)";   
                                                   
    ifstream inputFile(filePathin);
    if (!inputFile.is_open()) {
        cerr << "Error input" << endl;
        return 1;
    }
    const char* filePathout = R"(C:\code\sublime text\SAP\output.txt)";                                                                                           //change here
    ofstream outputFile(filePathout);
    if (!outputFile.is_open()) {
        cerr << "Error output" << endl;
        return 1;
    }

	//main list define 
    set<user> users_list;
    string line;

    // init count
    long long revenue =0 , potential_lost_revenue =0;

    //some map for different types of vehicle
    map <string, int> mp_car_type_to_int = {{  "compact",0},{"medium ", 1},{"full-size", 2},{"class 1 truck", 3},{"class 2 truck", 4}};
    map <int, string> mp_car_type_to_str = {{0,"compact"},{1,"medium "},{2,"full-size"},{3,"class 1 truck"},{4, "class 2 truck"}};
    map <int, pair<int, int>> type_to_cost_and_time = {{0,make_pair(150,30)},{1,make_pair(150,30)},{2,make_pair(150,30)},{3,make_pair(250,60)},{4,make_pair(700,120)}};
    
    //inport the data
    while (getline(inputFile, line)){
    	string date_call, date_list, type;
    	stringstream ss(line);
    	char comma; 

    	getline(ss,date_call, ',');
    	getline(ss,date_list, ',');
    	getline(ss,type,',');

    	int a[5];
		a[0] = atoi(date_call.substr(0, 4).c_str());
		a[1] = atoi(date_call.substr(5, 2).c_str());
		a[2] = atoi(date_call.substr(8, 2).c_str());
		a[3] = atoi(date_call.substr(11, 2).c_str());
		a[4] = atoi(date_call.substr(14, 2).c_str());

		int b[5];
		b[0] = atoi(date_list.substr(0, 4).c_str());
		b[1] = atoi(date_list.substr(5, 2).c_str());
		b[2] = atoi(date_list.substr(8, 2).c_str());
		b[3] = atoi(date_list.substr(12, 2).c_str());
		b[4] = atoi(date_list.substr(14, 2).c_str());

		//check the time is ligit 2022:10:1:7:0 to 2022:11:30:19:0
		if (b[0]==2022 && (b[1]== 10 || b[1] == 11) && (b[3]<=19 && b[3] >=7)){
    	users_list.emplace(a, b, mp_car_type_to_int[type]);
    	}else{
    		potential_lost_revenue += type_to_cost_and_time[mp_car_type_to_int[type]].first;
    	}
    	

    }

 	/*

    for (auto it = users_list.begin(); it != users_list.end();it++){
    	for (int i =0; i< 5;i++){
    		cout << it -> date_phone_call_list[i] << ":";
    	}
    	cout << " ";
    	for (int i =0; i< 5;i++){
    		cout << it -> date_reservation_list[i] << ":";
    	}
    	cout << " ";
    	cout << it -> type;
    	cout << endl;
    }
    cout << users_list.size();
    */



    // a true and false table of length (31+30) month * 12 (hours) * 60 (mins)  and each has 10 sub vector
    vector <vector<vector<vector<bool>>>> time_schedule_true_and_false_table(61, 
    	vector<vector<vector<bool>>>(12, 
    		vector<vector<bool>>(60, 
    			vector<bool>(10,0))));  // 0 means unoccupied


    // final output to the calendar 
    /*
				starting time + end time + car type + stations
				..........

					revenue    +     potential revenue lost


					    int date_phone_call_list[5];
					    int date_reservation_list[5];
					    int type; 
    */
    set<user> accepted_user;
    vector <int> stations;

    for (auto it = users_list.begin(); it != users_list.end(); it++){
    	int val = check_slot_available_ang_update(time_schedule_true_and_false_table, it->type, it->date_reservation_list, type_to_cost_and_time);
    	if (val > 0){
    		revenue += type_to_cost_and_time[it->type].first;
    		accepted_user.emplace(*it);
    		stations.push_back(val);

    	}else{
    		potential_lost_revenue += type_to_cost_and_time[it->type].first;
    	}
    }
    outputFile << "Revenue: " << revenue<< endl;
    outputFile << "Potential_lost_revenue: " << potential_lost_revenue <<endl;
    int pos{};
    for (auto it = accepted_user.begin(); it != accepted_user.end(); it++){
    	outputFile << it ->date_reservation_list[0] << ":"<<it ->date_reservation_list[1] << ":"<<it ->date_reservation_list[2]
    	<< ":"<<it ->date_reservation_list[3]<< ":"<<it ->date_reservation_list[4] << "    " << 
	
		it ->date_reservation_list[0] << ":"<<it ->date_reservation_list[1] << ":"<<it ->date_reservation_list[2] 
    	<< ":"<<it ->date_reservation_list[3] + (type_to_cost_and_time[it ->type].second /60)<< ":"<<it ->date_reservation_list[4] + (type_to_cost_and_time[it ->type].second %60)<< "    " << 


    	mp_car_type_to_str[it->type] << "    " <<

    	type_to_cost_and_time[it ->type].first

    	<< " "<<

    	 stations[pos++] << endl;
    }

    inputFile.close();
    outputFile.close();

}
	



#endif