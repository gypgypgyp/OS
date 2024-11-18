# OS

# CPU scheduler

1/ firstly you need input file in .txt format as argument input.

2/ To compile the program, use the following command:

    g++ -g -Wall -Werror -pedantic -std=c++11 -o main main.cpp

3/ then run the executable file

    ./main <inputfilename>

4/ the program will out put the sorted jobs list and result of MAS scheduling and SRTF scheduling,
including average turnaround time, waiting time, and execution order with final termination time.

T turn around = T completion - T arrival

waiting time = T turn around - T cpu burst

result example:

./main jobs.txt
--------------input--------------
ID: 5, Burst Time: 17, Arrival Time: 0
ID: 9, Burst Time: 20, Arrival Time: 0
ID: 11, Burst Time: 18, Arrival Time: 0
ID: 15, Burst Time: 2, Arrival Time: 0
ID: 20, Burst Time: 1, Arrival Time: 0
ID: 2, Burst Time: 5, Arrival Time: 1
ID: 4, Burst Time: 1, Arrival Time: 1
ID: 12, Burst Time: 2, Arrival Time: 1
ID: 7, Burst Time: 6, Arrival Time: 2
ID: 1, Burst Time: 18, Arrival Time: 3
ID: 14, Burst Time: 2, Arrival Time: 3
ID: 16, Burst Time: 7, Arrival Time: 3
ID: 17, Burst Time: 20, Arrival Time: 4
ID: 18, Burst Time: 4, Arrival Time: 4
ID: 3, Burst Time: 14, Arrival Time: 5
ID: 13, Burst Time: 3, Arrival Time: 5
ID: 6, Burst Time: 2, Arrival Time: 6
ID: 8, Burst Time: 15, Arrival Time: 7
ID: 10, Burst Time: 6, Arrival Time: 8
ID: 19, Burst Time: 17, Arrival Time: 9
--------------MAS_Scheduling_result--------------
Average Turnaround Time: 88.30
Average Waiting Time: 79.30
5(99), 9(178), 11(123), 15(20), 20(21), 2(26), 4(27), 12(29), 7(35), 1(135), 14(43), 16(136), 17(180), 18(59), 3(156), 13(68), 6(70), 8(165), 10(82), 19(176), 5(99), 9(178), 11(123), 1(135), 16(136), 17(180), 3(156), 8(165), 19(176), 9(178), 17(180)
--------------SRTF_Scheduling_result--------------
Average Turnaround Time: 52.10
Average Waiting Time: 43.10
20(1), 4(2), 12(4), 14(6), 6(8), 15(10), 13(13), 18(17), 2(22), 7(28), 10(34), 16(41), 3(55), 8(70), 5(87), 19(104), 1(122), 11(140), 9(160), 17(180)



# VirtualMemorySystemSimulator

This is a virtual memory system simulator. It is able to 

1. Split the Address
2. Translate the Address
3. Handle Page Faults
4. Improve the Page Replacement Algorithm


1/ To compile the program, use the following command:

g++ -g -Wall -pedantic -std=c++11 *.c -o vm-sim

2/ The program file generated will be named vm-sim. It takes the name of a memory trace file as an argument, and a few such files are provided for you in the workloads/ directory. For example, to run the simulator with the basic trace file, run:

./vm-sim workloads/basic

3/ There are also several optional command line options that you can play around with, such as changing the memory size and the page size. The defaults are memory size = 16 and page size = 2. To view these options, type:

./vm-sim -h

Here are the contents of the “Basic” trace file:
@ 0 init
@ 1 mozilla s014 s028 s039
s 0 4 10 l028 s116 s129
s 1 3 42
s 1 4 11 l129 l116 l039 l028
Example lines:
“s 0 1 4”→Proceed id 0 (init) STORED 4 as data in virtual address 00001. “l 0 2 8”→Process id 0 (init) LOADED 8 as data from virtual address 00002


# Shell

This is a basic shell program named nush that can take user commands, execute them, and return appropriate status messages.
It supports features like executing commands, handling errors, and exiting the shell.

1/ firstly you need to go to the file where the .cpp is located.

2/ To compile the program, use the following command:

    g++ -g -Wall -Werror -pedantic -std=c++11 -o nush nush.cpp

3/ then run the executable file

    ./nush

4/ You will see the nush$ prompt, where you can start typing commands just like in a regular shell. For example:

   nush$ ls -l
   nush$ echo Hello
   nush$ exit

5/ To exit the shell, type exit.

   nush$ exit
   Exiting nush...

error handling:

If you type a command that does not exist, you will see an error message:

    nush$ fakecmd
    nush error: Command not found: fakecmd
    nush status: 127
    nush: Error - Command not found.

if the argument for a command line is not able to be found, you will see an error message:

nush$ ls abs
ls: cannot access 'abs': No such file or directory
nush status: 2
nush: Command returned non-zero exit code: 2

nush$ wc nosuchfile
wc: nosuchfile: No such file or directory
nush status: 1
nush: Command returned non-zero exit code: 1

notice:

there are certain test cases/situations this program does not handle:

1/ when input "wc" WITHOUT command line arguments

