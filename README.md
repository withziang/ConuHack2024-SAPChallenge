# Conuhack

This repository is dedicated to store the solution my team made for 2024 Conuhack SAP challenge

### Contributors

<!-- CONTRIBUTORS-LIST:START - Do not remove or modify this section -->
<!-- prettier-ignore-start -->
<!-- markdownlint-disable -->

![AevisMotax](https://avatars.githubusercontent.com/u/108089675?v=4) <!-- Replace with actual name and avatar URL -->
[AevisMotax](https://github.com/AevisMotax) <!-- Replace with actual name and GitHub profile URL -->

<!-- markdownlint-restore -->
<!-- prettier-ignore-end -->

<!-- CONTRIBUTORS-LIST:END -->

## Challenge detail

**Topic:** Schedule optimization, work load distribution among 10 station.

Your role is to construct a software solution focused on optimizing the schedule for a tire change shop. This shop operates from 7am to 7pm, beginning in early October and ending in late November. 5 types of vehicle
- Compact car: 30min 150$
- Medium car: 30min 150$
- Full-size car: 30min 150$
- Class 1 Truck: 1h 250$
- Class 2 Truck: 2h 700$

Input file is .CSV, sample input

```
2022-09-10 07:28,2022-11-27 07:16,compact
2022-09-09 18:30,2022-10-15 18:07,class 2 truck
2022-09-18 15:20,2022-11-01 13:16,full-size
2022-10-20 18:30,2022-11-23 09:21,full-size
2022-10-21 14:56,2022-11-23 16:22,medium
2022-09-29 18:30,2022-11-07 07:54,class 1 truck
2022-10-15 17:07,2022-10-22 18:30,class 2 truck
2022-09-06 13:43,2022-11-24 07:48,full-size
2022-11-02 18:30,2022-11-24 07:01,class 1 truck
2022-09-02 15:35,2022-10-04 07:28,full-size
2022-10-13 07:26,2022-10-24 07:07,class 1 truck
2022-10-05 07:31,2022-10-26 18:27,class 2 truck
2022-09-03 07:28,2022-10-24 07:55,class 1 truck
2022-10-09 18:28,2022-10-24 07:26,class 2 truck
2022-09-03 07:32,2022-10-06 18:14,full-size
2022-10-09 18:16,2022-11-30 14:29,class 2 truck
2022-10-22 07:22,2022-11-30 15:00,class 2 truck
2022-11-23 09:08,2022-11-23 09:08,medium
2022-10-11 09:11,2022-10-11 09:11,medium
2022-11-01 07:26,2022-11-13 07:21,medium
2022-11-24 09:41,2022-11-24 15:02,full-size
2022-09-04 07:24,2022-10-25 12:25,class 2 truck
2022-11-19 18:03,2022-11-23 11:02,full-size
2022-10-13 17:23,2022-11-04 08:09,full-size
2022-10-26 17:57,2022-10-26 17:57,compact
2022-10-29 07:10,2022-11-09 07:51,full-size
2022-10-15 18:30,2022-10-15 18:30,medium
2022-10-07 14:51,2022-10-07 14:51,class 1 truck
2022-09-13 15:32,2022-10-27 18:30,compact
2022-09-14 07:14,2022-11-07 08:48,compact
```

Output will be a calendar and Revenue and potential lost revenue.


# Solution detail

__Frontend__: Typescript react 

__Backend__: python

__API__: flask

# Repository detail

Under __main__, the sample data is provided, as well a backend c++ code and its data output. This code is not used in the final version due to API connection difficulty. Real python backend code uses similar algorithm.

Under __main/final__ version, the website file plus backend python code is provided. 

# Algorithm detail
A true and false table`(61 * 12 * 60 * 10)` matrix. For each customer, evaluate the vacancy from their reservation starting time to the end time. If it is available, return true and update the table. 
```
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
    // a true and false table of length (31+30) month * 12 (hours) * 60 (mins)  and each has 10 sub vector
    vector <vector<vector<vector<bool>>>> time_schedule_true_and_false_table(61, 
    	vector<vector<vector<bool>>>(12, 
    		vector<vector<bool>>(60, 
    			vector<bool>(10,0))));  // 0 means unoccupied
```
