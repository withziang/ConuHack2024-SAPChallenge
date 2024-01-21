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

// function to check if the time slot is avaible

bool check_slot_available(vector <vector<vector<vector<bool>>>> time_schedule_true_and_false_table,
						vector <vector<vector<int>>> time_schedule_extra_slot,
						 int type, int time[5]){

}



int main(){

    //automatically create output file txt, change input file direction, change output file location if needed
	/*cout << "Input csv file directory: " << endl;  */
	const char* filePathin = R"(C:\Users\ziang\Downloads\datafile.csv)";   

    string sourceFileName(__FILE__);
    string base = sourceFileName.substr(sourceFileName.find_last_of("\\") + 1, sourceFileName.find_last_of(".") - sourceFileName.find_last_of("\\") - 1);
                                                         
    ifstream inputFile(filePathin);
    if (!inputFile.is_open()) {
        cerr << "Error input" << endl;
        return 1;
    }
    const char* filePathout = R"(C:\code\sublime text\SAP)";                                                                                           //change here
    ofstream outputFile(filePathout + base + ".txt");
    if (!outputFile.is_open()) {
        cerr << "Error output" << endl;
        return 1;
    }

	//main list define 
    set<user> users_list;
    string line;

    //some map for different types of vehicle
    map <string, int> mp = {{"compact", 0},{"medium", 1},{"full-size", 2},{"class 1 truck", 3},{"class 2 truck", 4}};
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
    	users_list.emplace(a, b, mp[type]);
    	}
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



    // a true and false table of length (31+30) month * 12 (hours) * 60 (mins)  and each has five sub vector
    vector <vector<vector<vector<bool>>>> time_schedule_true_and_false_table(61, 
    	vector<vector<vector<bool>>>(12, 
    		vector<vector<bool>>(60, 
    			vector<bool>(5,0))));
    vector <vector<vector<int>>> time_schedule_extra_slot(61,
    	vector<vector<int>>(12,vector<int>(60,0)));

    // init count
    long long revenue{}, potential_lost_revenue{};


    for (auto it = users_list.begin(); it != users_list.end(); ++it){
    	if (check_slot_available(time_schedule_true_and_false_table, it->type, it->date_reservation_list)){
    		revenue += type_to_cost_and_time[it->type].first;

    		update_slot(it->type, it->date_reservation_list);
    	}else{
    		potential_lost_revenue += type_to_cost_and_time[it->type].first;
    	}
    }









    inputFile.close();
    outputFile.close();

}
	



#endif