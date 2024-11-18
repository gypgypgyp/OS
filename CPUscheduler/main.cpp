//
// Created by Yunpei GU
//
#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <sstream>
#include <unordered_set>
#include <numeric>
#include <deque>
#include <string>
#include <stdexcept>
#include <iomanip>
#include <algorithm>

using namespace std;
const int INF = 1e9;
using namespace std;

struct Process {
    int id;
    int burst_time;
    int arrival_time;
    int remaining_time;  // To track remaining burst time
    int start_time;      // Time when the process starts its first execution
    int finish_time;     // Time when the process finishes execution
    int queue;           // Queue it belongs to (0, 1, or 2)
    Process(int i, int b, int a) :
    id(i),
    burst_time(b),
    arrival_time(a),
    remaining_time(b),
    start_time(-1),
    finish_time(-1),
    queue(0) {}
};

// Function to print the vector of processes
void printProcesses(const deque<Process>& processes) {
    for (const auto& p : processes) {
        cout << "ID: " << p.id << ", Burst Time: " << p.burst_time << ", Arrival Time: " << p.arrival_time << endl;
    }
}

// Comparator for sorting processes based on arrival time and process ID
bool compareProcesses(const Process& p1, const Process& p2) {
    if (p1.arrival_time == p2.arrival_time) {
        return p1.id < p2.id; // Smaller ID first
    }
    return p1.arrival_time < p2.arrival_time; // Sort by arrival time
}



// Function to print the execution order and termination times
void printExecutionOrder(const vector<string>& execution_order) {
    for (const auto& p : execution_order) {
        cout << p << " ";
    }
    cout << endl;
}

void print_exe_order(deque<Process*> q){
    int count = 0;
    for (auto p: q){
        if (count==0){
            cout << p->id << "(" << p->finish_time << ")";
            count++;
        }else{
            cout <<", " << p->id << "(" << p->finish_time << ")";
        }
    }
    cout << endl;
}

void print_queue(deque<Process*> q){
    for (auto p: q){
        cout << p->id << " " << p->burst_time << " " <<p->remaining_time << endl;
    }
    cout << endl;
}

// MAS Scheduling algorithm
void MAS_Scheduling(deque<Process>& processes) {
    deque<Process*> q0, q1, q2; // Three queues
    vector<string> execution_order; //
    deque<Process*> exe_order; //To store the order of execution
    int current_time = 0;
    int time_quantum_q0 = 6, time_quantum_q1 = 12;
    double turnaround_time = 0.0;
    double waiting_time = 0.0;
    int n = processes.size();

    // check if there is process
    if (n==0 or processes.empty()){
        cout<< "There is no process"<<endl;
        return;
    }

    Process* running_process = nullptr; // Keep track of the currently running process
    int time_slice = 0; // Current time slice already used for the running process

    while (!processes.empty() || running_process || !q0.empty() || !q1.empty() || !q2.empty()) {
        // If there are new processes arrives at curr time, add them to q0
        while (!processes.empty() && processes.front().arrival_time <= current_time) {
            q0.push_back(&processes.front());
            processes.erase(processes.begin());
        }
        // if there is a running process：
        if (running_process) {
            // 1.if the running process is finished, store it and wait to update to next job
            if (running_process->remaining_time == 0) {
                running_process->finish_time = current_time;
                turnaround_time += (running_process->finish_time - running_process->arrival_time);
                waiting_time += (running_process->finish_time - running_process->arrival_time - running_process->burst_time);
                exe_order.push_back(running_process);
                execution_order.push_back(to_string(running_process->id) + "(" + to_string(running_process->remaining_time) + ")" + "[" + to_string(current_time) + "]");
                running_process = nullptr;

                //2.if the running process run up the current time slice，but still have remaining_time，update to next job
            } else if (running_process->queue == 0 && time_slice >= time_quantum_q0) {
                exe_order.push_back(running_process);
                execution_order.push_back(to_string(running_process->id) + "(" + to_string(running_process->remaining_time) + ")" + "[" + to_string(current_time) + "]");
                running_process->queue = 1; // move to q1
                q1.push_back(running_process);
                time_slice = 0;
                running_process = nullptr;
            } else if (running_process->queue == 1 && time_slice >= time_quantum_q1) {
                exe_order.push_back(running_process);
                execution_order.push_back(to_string(running_process->id) + "(" + to_string(running_process->remaining_time) + ")" + "[" + to_string(current_time) + "]");
                running_process->queue = 2; // move to q2
                q2.push_back(running_process);
                time_slice = 0;
                running_process = nullptr;

                // 3. if the running process not finish the curr slice but is interrupted
            } else if (running_process->queue == 1 && !q0.empty()){
                exe_order.push_back(running_process);
                execution_order.push_back(to_string(running_process->id) + "(" + to_string(running_process->remaining_time) + ")" + "[" + to_string(current_time) + "]");
                running_process->queue = 1;
                q1.push_back(running_process);
                time_slice = 0;
                running_process = nullptr;
            } else if (running_process->queue == 2 && !q0.empty()) {
                exe_order.push_back(running_process);
                execution_order.push_back(
                        to_string(running_process->id) + "(" + to_string(running_process->remaining_time) + ")" + "[" +to_string(current_time) + "]");
                running_process->queue = 2;
                q2.push_back(running_process);
                time_slice = 0;
                running_process = nullptr;
            }
        }

        // if there is no running process, check queues
        if (running_process== nullptr) {
            if (!q0.empty()) {
                running_process = q0.front();
                q0.pop_front();
                time_slice = 0;
                if (running_process->start_time == -1) { //first time run
                    running_process->start_time = current_time;
                }
            } else if (!q1.empty()) {
                running_process = q1.front();
                q1.pop_front();
                time_slice = 0;
            } else if (!q2.empty()) {
                running_process = q2.front(); // 从 q2 取出
                q2.pop_front();
                time_slice = 0;
            } else { //jump to next process in processes
                if (!processes.empty()) {
                    current_time = processes.front().arrival_time - 1;
                }
            }
        }
        // run the running process
        if (running_process){
            running_process->remaining_time--;
        }
        time_slice++;
        current_time++;
    }

    // Calculate averages
    double avg_turnaround_time = turnaround_time / n;
    double avg_waiting_time = waiting_time / n;

    // Print results
    cout << fixed << setprecision(2);
    cout << "Average Turnaround Time: " << avg_turnaround_time << endl;
    cout << "Average Waiting Time: " << avg_waiting_time << endl;

    // Print execution order
    print_exe_order(exe_order);
}

// Comparator for SRTF scheduling, based on remaining time and process ID (to break ties)
struct CompareRemainingTime {
    bool operator()(const Process& a, const Process& b) {
        // If remaining times are the same, use process ID to break tie
        if (a.remaining_time == b.remaining_time) {
            return a.id > b.id;  // Smaller ID gets higher priority
        }
        return a.remaining_time > b.remaining_time;  // Smaller remaining time gets higher priority
    }
};

// Function to perform SRTF scheduling
void scheduleSRTF(deque<Process>& processes) {
    vector<int> waiting_times(processes.size(), 0);
    vector<int> turnaround_times(processes.size(), 0);
    vector<int> execution_order;
    deque<Process*> exe_order;

    int current_time = 0;
    int completed_processes = 0;
    int n = processes.size();
    int last_job_id = -1;

    while (completed_processes < n) {
        // Filter processes that have arrived and are not completed
        vector<Process*> available_processes;
        for (auto& process : processes) {
            if (process.remaining_time > 0 && process.arrival_time <= current_time) {
                available_processes.push_back(&process);
            }
        }
        // Sort available processes by remaining time, then by id
        sort(available_processes.begin(), available_processes.end(), [](Process* a, Process* b) {
            if (a->remaining_time == b->remaining_time)
                return a->id < b->id;  // Tie-breaker
            return a->remaining_time < b->remaining_time;
        });
        if (!available_processes.empty()) {
            // Execute the first available process
            Process* current_process = available_processes.front();
            if (current_process->id != last_job_id){
                execution_order.push_back(current_process->id);
                exe_order.push_back(current_process);
            }
            last_job_id = current_process->id;
            current_process->remaining_time--;
            current_time++;

            // If the process finishes
            if (current_process->remaining_time == 0) {
                current_process->finish_time = current_time;
                completed_processes++;
            }
        } else {
            // If no process is available, move the time forward
            current_time++;
        }
    }

    // Calculate turnaround time and waiting time
    for (int i = 0; i < n; ++i) {
        turnaround_times[i] = processes[i].finish_time - processes[i].arrival_time;
        waiting_times[i] = turnaround_times[i] - processes[i].burst_time;
//        cout <<"P"<< i<< " "<< processes[i].arrival_time <<" " << processes[i].burst_time << " "<< processes[i].finish_time << " " << turnaround_times[i] << " "<< waiting_times[i] << endl;
    }

    // Calculate averages
    double avg_turnaround_time = accumulate(turnaround_times.begin(), turnaround_times.end(), 0.0) / n;
    double avg_waiting_time = accumulate(waiting_times.begin(), waiting_times.end(), 0.0) / n;

    // Print results
    cout << fixed << setprecision(2);
    cout << "Average Turnaround Time: " << avg_turnaround_time << endl;
    cout << "Average Waiting Time: " << avg_waiting_time << endl;

//    cout << "Execution Order: ";
//    for (size_t i = 0; i < execution_order.size(); ++i) {
//        cout << execution_order[i];
//        if (i < execution_order.size() - 1) {
//            cout << ", ";
//        } else {
//            cout << endl;
//        }
//    }
//    cout << "Final Termination Times: ";
//    for (const auto& process : processes) {
//        cout << process.id << "(" << process.finish_time << "), ";
//    }
//    cout << endl;

    print_exe_order(exe_order);
}

void SRTF_Scheduling(deque<Process>& processes){
    // Priority queue (min-heap) for SRTF scheduling
    priority_queue<Process, deque<Process>, CompareRemainingTime> q;
    int current_time = 0;
    int completed_jobs = 0;
    vector<string> execution_order; // To store the order of execution
    deque<Process*> exe_order; //
    // Keep track of the index of the next process to be added to the ready queue
    int next_process_idx = 0;
    // Sort processes based on arrival time and ID
//    sort(processes.begin(), processes.end(), compareProcesses);
    if (processes.size()==0 or processes.empty()){
        cout<< "There is no process"<<endl;
        return;
    }
    int number_of_total_jobs = processes.size();
    Process* running_process = nullptr; // Keep track of the currently running process

    int temp = 0;

    while (completed_jobs < number_of_total_jobs) {
        cout << "-start of the sec-curr time: " << current_time << endl;
        if (running_process) {
            cout << "curr process: " << running_process->id << " " << running_process->arrival_time <<" "<< running_process->remaining_time << endl;
        }
        int min_remain = INF;
        // Add all processes that have arrived by the current time to the ready queue
        while (next_process_idx < number_of_total_jobs && processes[next_process_idx].arrival_time <= current_time) {
            q.push(processes[next_process_idx]);
            min_remain = min(min_remain, processes[next_process_idx].remaining_time);
            next_process_idx++;
        }

        // if there is a running process and need to switch
        // 1. if the current job is finished
        if(running_process) {
            if (running_process->remaining_time == 0) {
                running_process->finish_time = current_time;
                exe_order.push_back(running_process);
                execution_order.push_back(
                        to_string(running_process->id) + "(" + to_string(running_process->remaining_time) + ")" + "[" +
                        to_string(current_time) + "]");
                running_process = nullptr;
                completed_jobs++;
            } else if (min_remain < running_process->remaining_time) {
                // 2. if current job is interrupt by a new job
                q.push(*running_process);
                exe_order.push_back(running_process);
                execution_order.push_back(
                        to_string(running_process->id) + "(" + to_string(running_process->remaining_time) + ")" + "[" +
                        to_string(current_time) + "]");
                running_process = nullptr;
            }
        }
        if (!running_process && !q.empty()){
            running_process = const_cast<Process*>(&q.top());
            q.pop();
        }

        // If there is no running process and no more jobs to add, break the loop
        if (!running_process && q.empty() && next_process_idx >= number_of_total_jobs) {
            break; // No more jobs to process
        }

        if (running_process) {
            running_process->remaining_time--;
        }

        // 打印当前状态
        cout << "-end of the sec-curr time: " << current_time << endl;
        if (running_process) {
            cout << "curr process: " << running_process->id << " " << running_process->arrival_time <<" "<< running_process->remaining_time << endl;
        }

        current_time++;
        temp++;
        if (temp==12){
            break;
        }
    }
    // Print execution order
    printExecutionOrder(execution_order);
    print_exe_order(exe_order);
}

// Function to read data from file and store it in a vector of processes
deque<Process> getData(const string& filename) {
    ifstream file(filename);
    deque<Process> processes;
    unordered_set<int> seen_ids;  // Track process IDs to check for duplicates
    if (!file.is_open()) {
        cout << "Error: Unable to open file " << filename << endl;
        return processes;
    }
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string id_str, burst_str, arrival_str;
        // Read and split the line by commas
        if (getline(ss, id_str, ',') &&
            getline(ss, burst_str, ',') &&
            getline(ss, arrival_str, ',')) {
            try {
                // Convert strings to integers
                int id = stoi(id_str);
                int burst_time = stoi(burst_str);
                int arrival_time = stoi(arrival_str);
                // Input validation: check if values are valid
                if (id <= 0) {
                    throw runtime_error("Error: Invalid process ID (" + to_string(id) + ") in line: " + line);
                }
                if (burst_time <= 0) {
                    throw runtime_error("Error: Invalid burst time (" + to_string(burst_time) + ") in line: " + line);
                }
                if (arrival_time < 0) {
                    throw runtime_error("Error: Invalid arrival time (" + to_string(arrival_time) + ") in line: " + line);
                }
                if (seen_ids.find(id) != seen_ids.end()) {
                    throw runtime_error("Error: Duplicate process ID (" + to_string(id) + ") found in line: " + line);
                }
                // Add the process to the deque
                processes.emplace_back(id, burst_time, arrival_time);
            } catch (const invalid_argument& e) {
                throw runtime_error("Error: Invalid data in line: " + line);
            } catch (const out_of_range& e) {
                throw runtime_error("Error: Number out of range in line: " + line);
            }
        } else {
            throw runtime_error("Error: Malformed line: " + line);
        }
    }
    file.close();
    return processes;
}


int main(int argc, char* argv[]) {
    if (argc != 2) {  // check the command line input
        cout << "Error: Invalid command line arguments. Expected: <program> <input_file>" << endl;
        return 1;
    }
    string filename = argv[1];  // get the filename
    deque<Process> processes;
    try {
        processes = getData(filename);
        if (processes.empty()) {  // Check if the deque is empty
            cout << "Error: No valid processes found." << endl;
            return 1;
        }
    } catch (const exception& e) {
        cerr << e.what() << endl;
        return 1;
    }

    // Sort processes based on arrival time and ID
    sort(processes.begin(), processes.end(), compareProcesses);

    cout<<"--------------input--------------" << endl;
//     Print the sorted processes
    printProcesses(processes);

    cout<<"--------------MAS_Scheduling_result--------------" << endl;
    // Execute the scheduling
    MAS_Scheduling(processes);

    cout<<"--------------SRTF_Scheduling_result--------------" << endl;
//    SRTF_Scheduling(processes);
    processes = getData(filename);
    scheduleSRTF(processes);
    return 0;
}