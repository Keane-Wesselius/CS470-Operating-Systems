#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cstring>
#include <cmath>
#include <algorithm>
#include <unistd.h>
#include <pthread.h>

using namespace std;

pthread_mutex_t mutex;


const int MIN_OPTION = 1;
const int MAX_OPTION = 4;
const int STRUCT_DATA_SIZE = 50;
bool doneProcessing = false;
bool timeToLoadBalance = false;
bool noMoreBalancing = false;
const int TIME_QUANTUM = 2;
const int PRIORITY_QUANTUM = 10;
const int SLEEP_TIME = 200000;

vector<int> scheduleAlgos;
vector<double> percentWork;



//PCB
struct ProcessInfo {
    char priority;
    char process_name[24];
    int process_id;
    char activity_status;
    int cpu_burst_time;
    int base_register;
    long limit_register;
    int num_files;
};

//Create a vector to house the processes that are associated with each processor
vector<vector<ProcessInfo>> processorProcesses;


//Validations
bool isPositiveInteger(string userInput) {
    if (userInput.find_first_not_of("0123456789") != string::npos || userInput == "") {
        return false;
    }
    return true;
}

bool isPositiveDouble(string userInput) {
    if (userInput.find_first_not_of(".0123456789") != string::npos || userInput == "") {
        return false;
    }
    return true;
}

bool validateIntput(string userInput, int min, int max)
{
    int option = 0;
    if (isPositiveInteger(userInput)) {
        option = stoi(userInput);
        if (option >= min && option <= max) {
            return true;
        }
    }
    return false;

}

void printPCB(ProcessInfo process)
{
    std::cout << "Priority: " << static_cast<int>(process.priority) << endl;
    std::cout << "Process name: " << process.process_name << endl;
    std::cout << "Process ID: " << process.process_id << endl;
    std::cout << "Activity status: " << static_cast<int>(process.activity_status) << endl;
    std::cout << "CPU burst time: " << process.cpu_burst_time << endl;
    std::cout << "Base register: " << process.base_register << endl;
    std::cout << "Limit register: " << process.limit_register << endl;
    std::cout << "Number of files: " << process.num_files << endl;
    std::cout << endl;
}

void printAllPCB(vector<ProcessInfo> processes)
{
    for (const auto& process : processes) {
        printPCB(process);
    }
}


bool compareByPriority (const ProcessInfo& a, const ProcessInfo& b)
{
    return a.priority < b.priority;
}

void sortByPriority(vector<ProcessInfo>& vp)
{
    sort(vp.begin(), vp.end(), compareByPriority);
}

bool compareByShortest (const ProcessInfo& a, const ProcessInfo& b)
{
    return a.cpu_burst_time < b.cpu_burst_time;
}

void sortByShortest(vector<ProcessInfo>& vp)
{
    sort(vp.begin(), vp.end(), compareByShortest);
}

void sortAllPCB(vector<vector<ProcessInfo>>& vvp, vector<int> vAlgo)
{
    for (int i = 0; i < vvp.size(); i++)
    {
        if (vAlgo.at(i) == 1)
        {
            sortByPriority(vvp.at(i));
        }
        else if (vAlgo.at(i) == 2)
        {
            sortByShortest(vvp.at(i));
        }
        
    }
}


void incrementPriority(int n)
{
    for (int i = 0; i < processorProcesses.at(n).size(); i++)
    {
        processorProcesses.at(n).at(i).priority -= 1;
    }
}

void balancePrint()
{
    for (int i = 0; i < processorProcesses.size(); i++)
    {
        cout << "Processor#" << i << ": ";
        for (int j = 0; j < processorProcesses.at(i).size(); j++)
        {
            cout << processorProcesses.at(i).at(j).process_name << "|";
        }
        cout << endl;
    }
}

void loadBalance()
{
    cout << "-----Load Balancing-----" << endl;
    cout << endl;
    cout << "Initial load before balancing:" << endl;
    balancePrint();
    cout << endl;


    int totalProcesses = 0;
    for (int i = 0; i < processorProcesses.size(); i++)
    {
        totalProcesses += processorProcesses.at(i).size();
        
    }

    int averageLoad = floor(totalProcesses / processorProcesses.size());

    vector<ProcessInfo> temp;

    //No need to load balance at this point
    if (totalProcesses == 1)
    {
        // cout << "Case 1: No need to load balance" << endl;
        // fflush(stdout);
        noMoreBalancing = true;
    }


    
    //This case is that there is a need to load balance this time and then never again
    else if (totalProcesses <= processorProcesses.size())
    {
        // cout << "Case 2: Last load balance" << endl;
        // fflush(stdout);
        for (int i = 0; i < processorProcesses.size(); i++)
        {
            if (processorProcesses.at(i).size() > 1)
            {
                temp.push_back(processorProcesses.at(i).back());
                processorProcesses.at(i).pop_back();
            }    
        }

        for (int i = 0; i < processorProcesses.size(); i++)
        {
            if (processorProcesses.at(i).size() == 0 && temp.size() > 0)
            {
                processorProcesses.at(i).push_back(temp.back());
                temp.pop_back();
                
            }    
        }
        noMoreBalancing = true;
    }

    else
    {
        
        for (int i = 0; i < processorProcesses.size(); i++)
        {
            
            while (processorProcesses.at(i).size() > averageLoad)
            {
                
                temp.push_back(processorProcesses.at(i).back());
                processorProcesses.at(i).pop_back();
            }    
        }
    

        for (int i = 0; i < processorProcesses.size(); i++)
            {
                
                while (processorProcesses.at(i).size() < averageLoad && temp.size() > 0)
                {
                    processorProcesses.at(i).push_back(temp.back());
                    temp.pop_back();
                    
                }    
            }
        
        for (int i = 0; i < processorProcesses.size(); i++)
        {
            if (temp.size() == 0)
            {
                break;
            }
            processorProcesses.at(i).push_back(temp.back());
            temp.pop_back();
        }
        
        
    }

    cout << endl;
    cout << "Load after balancing:" << endl;
    balancePrint();
    fflush(stdout);
    cout << endl;
    


    if (!noMoreBalancing)
    {
        cout << "Now sorting for Priority and Shortest Job first where applicable:" << endl;
        sortAllPCB(processorProcesses, scheduleAlgos);
        balancePrint();
        fflush(stdout);
    }
    cout << endl;
    cout << "-----End Balancing-----" << endl;
}


void * priority(void* arg)
{
    int vectorId = *(int*) arg;
    int timeQ = 0;
    while (processorProcesses.at(vectorId).size() > 0)
    {
        pthread_mutex_lock(&mutex);
        cout << "Processor#" << vectorId << "(Priority) is working on " << processorProcesses.at(vectorId).front().process_name << " | Burst time left " << processorProcesses.at(vectorId).front().cpu_burst_time << " | Priority " << static_cast<int>(processorProcesses.at(vectorId).front().priority) << " | Processes left in processor " << processorProcesses.at(vectorId).size() << endl;
        fflush(stdout);
        if (processorProcesses.at(vectorId).front().cpu_burst_time < TIME_QUANTUM)
        {
            processorProcesses.at(vectorId).front().cpu_burst_time = 0;
        }
        else
        {
            processorProcesses.at(vectorId).front().cpu_burst_time -= TIME_QUANTUM;
        }

        if(processorProcesses.at(vectorId).front().cpu_burst_time == 0)
        {
            processorProcesses.at(vectorId).erase(processorProcesses.at(vectorId).begin());
        }
        
        if(processorProcesses.at(vectorId).size() == 0)
        {  
            if (!noMoreBalancing)
            {
        
                loadBalance();
            }
        }

        timeQ += TIME_QUANTUM;

        if (timeQ == PRIORITY_QUANTUM)
        {
            cout << "Increasing priority for the priority queue" << endl;
            incrementPriority(vectorId);
            timeQ  = 0;
        }
        pthread_mutex_unlock(&mutex);
        usleep(SLEEP_TIME);
    }

    return NULL;
}

void * shortest(void* arg)
{
    int vectorId = *(int*) arg;
    while (processorProcesses.at(vectorId).size() > 0)
    {
        pthread_mutex_lock(&mutex);
        cout << "Processor#" << vectorId << "(Shortest) is working on " << processorProcesses.at(vectorId).front().process_name << " | Burst time left " << processorProcesses.at(vectorId).front().cpu_burst_time << " | Processes left in processor " << processorProcesses.at(vectorId).size() << endl;
        fflush(stdout);
        if (processorProcesses.at(vectorId).front().cpu_burst_time < TIME_QUANTUM)
        {
            processorProcesses.at(vectorId).front().cpu_burst_time = 0;
        }
        else
        {
            processorProcesses.at(vectorId).front().cpu_burst_time -= TIME_QUANTUM;
        }

        if(processorProcesses.at(vectorId).front().cpu_burst_time == 0)
        {
            processorProcesses.at(vectorId).erase(processorProcesses.at(vectorId).begin());
        }
        
        if(processorProcesses.at(vectorId).size() == 0)
        {
            if (!noMoreBalancing)
            {
                loadBalance();
            }  
        }
        pthread_mutex_unlock(&mutex);
        usleep(SLEEP_TIME);
    }
    return NULL;
}

void * roundRobin(void* arg)
{
    int vectorId = *(int*) arg;
    while (processorProcesses.at(vectorId).size() > 0)
    {
        pthread_mutex_lock(&mutex);
        cout << "Processor#" << vectorId << "(RR) is working on " << processorProcesses.at(vectorId).front().process_name << " | Burst time left " << processorProcesses.at(vectorId).front().cpu_burst_time << " | Processes left in processor " << processorProcesses.at(vectorId).size() << endl;
        fflush(stdout);
        if (processorProcesses.at(vectorId).front().cpu_burst_time < TIME_QUANTUM)
        {
            processorProcesses.at(vectorId).front().cpu_burst_time = 0;
        }
        else
        {
            processorProcesses.at(vectorId).front().cpu_burst_time -= TIME_QUANTUM;
        }

        if(processorProcesses.at(vectorId).front().cpu_burst_time == 0)
        {
            processorProcesses.at(vectorId).erase(processorProcesses.at(vectorId).begin());
        }
        else
        {
            processorProcesses.at(vectorId).push_back(processorProcesses.at(vectorId).front());
            processorProcesses.at(vectorId).erase(processorProcesses.at(vectorId).begin());

        }
        
        if(processorProcesses.at(vectorId).size() == 0)
        {
            if (!noMoreBalancing)
            {
                loadBalance();
            }  
        }
        pthread_mutex_unlock(&mutex);
        usleep(SLEEP_TIME);
    }
    return NULL;
}

void * fcfs(void* arg)
{
    int vectorId = *(int*) arg;
     while (processorProcesses.at(vectorId).size() > 0)
    {
        pthread_mutex_lock(&mutex);
        cout << "Processor#" << vectorId << "(FCFS) is working on " << processorProcesses.at(vectorId).front().process_name << " | Burst time left " << processorProcesses.at(vectorId).front().cpu_burst_time << " | Processes left in processor " << processorProcesses.at(vectorId).size() << endl;
        fflush(stdout);

        if (processorProcesses.at(vectorId).front().cpu_burst_time < TIME_QUANTUM)
        {
            processorProcesses.at(vectorId).front().cpu_burst_time = 0;
        }
        else
        {
            processorProcesses.at(vectorId).front().cpu_burst_time -= TIME_QUANTUM;
        }

        if(processorProcesses.at(vectorId).front().cpu_burst_time == 0)
        {
            processorProcesses.at(vectorId).erase(processorProcesses.at(vectorId).begin());
        }
        
        if(processorProcesses.at(vectorId).size() == 0)
        {
            if (!noMoreBalancing)
            {
                loadBalance();
            }  
        }

        pthread_mutex_unlock(&mutex);
        usleep(SLEEP_TIME);
    }

    return NULL;
}




int main(int argc, char* argv[])
{
    pthread_mutex_init(&mutex, NULL);
    string fileName = "";


    if (argc < 4)
    {
        cout << "Not enough arguments provided" << endl;
        cout << "Minimum arguments: Filename, 1-4(scheduling algorithm), percent(represented as a decimal)" << endl;
        return -1;
    }
    if (argc % 2 != 0)
    {
        cout << "Invalid amount of arguments; must be the form of:" << endl;
        cout << "Filename, 1-4(scheduling algorithm), percent(represented as a decimal), 1-4(scheduling algorithm), percent(represented as a decimal)..... depending on how many different emulated CPUs" << endl;
        return -1;
    }



    //File handling
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //File name handling making sure it exists
    fileName = argv[1];
    if (fileName.substr(fileName.length() - 3) != "bin" && fileName.substr(fileName.length() - 3) != "dat")
    {
        cout << "Error: File must be a .bin file" << endl;
        return -1;
    }

    //Try to open the file in binary mode
    ifstream file(fileName, std::ios::binary);
    if (!file) {
        cout << "Failed to open file" << endl;
        return 1;
    }

    // Determine the file size
    file.seekg(0, std::ios::end);
    std::size_t bSize = file.tellg();
    //Set iterator back to the beginning of the file 
    file.seekg(0, std::ios::beg);
    

    //Get the size of the PBC block
    //cout << "Struct size " << STRUCT_DATA_SIZE << endl;

    //The file should match up as a multiple of the struct size if all the data is there
    if (bSize % STRUCT_DATA_SIZE != 0) {
        std::cerr << "File size is not a multiple of struct size" << std::endl;
        return -1;
    }

    
    //Grab all the processes from the bin file and put them in a struct
    std::vector<ProcessInfo> processes;
    for (std::size_t i = 0; i < bSize; i += STRUCT_DATA_SIZE) {
        ProcessInfo info;
        
        file.read(reinterpret_cast<char*>(&info.priority), sizeof(info.priority));
        file.read(reinterpret_cast<char*>(&info.process_name), sizeof(info.process_name));
        file.read(reinterpret_cast<char*>(&info.process_id), sizeof(info.process_id));
        file.read(reinterpret_cast<char*>(&info.activity_status), sizeof(info.activity_status));
        file.read(reinterpret_cast<char*>(&info.cpu_burst_time), sizeof(info.cpu_burst_time));
        file.read(reinterpret_cast<char*>(&info.base_register), sizeof(info.base_register));
        file.read(reinterpret_cast<char*>(&info.limit_register), sizeof(info.limit_register));
        file.read(reinterpret_cast<char*>(&info.num_files), sizeof(info.num_files));
                
        processes.push_back(info);
    }


    //Printing out the PCBs
    printAllPCB(processes);
    cout << "Total number of processes: " << processes.size() << endl;
    int filesOpen = 0;
    for (int i = 0; i < processes.size(); i++)
    {
        filesOpen += processes.at(i).num_files;
    }
    cout << "Files open: " << filesOpen << endl;

    int totalMem = 0;
    for (int i = 0; i < processes.size(); i++)
    {
        totalMem += processes.at(i).limit_register - processes.at(i).base_register;
    }
    cout << "Total memory allocation: " << totalMem << endl;
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    


    
    for (int i = 2; i < argc; i++)
    {
        if (i % 2 == 0)
        {
            if (validateIntput(argv[i], MIN_OPTION, MAX_OPTION))
            {
                scheduleAlgos.push_back(stoi(argv[i]));
            }
            else
            {
                cout << "Invalid Input: Options for scheduling algorithm are from " << MIN_OPTION << "-" << MAX_OPTION << endl;
                return -1;
            }
        }
        else
        {
            if (isPositiveDouble(argv[i]))
            {
                if (stod(argv[i]) > 0)
                {
                percentWork.push_back(stod(argv[i]));
                }
                else
                {
                    cout << "Enter an amount of work higher than zero for the processor loads" << endl;
                    return -1;
                }
            }
            else
            {
                cout << "Invalid Input: Must provide a positive double for the percent amount of work for the scheduling algorithm, not including zero" << endl;
                return -1;
            }
        }

    }

    double total = 0;
    for (int i = 0; i < percentWork.size(); i++)
    {
        total += percentWork.at(i);
    }
    //cout << total << endl;
    //cout << total << endl;
    if (total > 1.000001 || total < 0.99999999)
    {
        cout << "Total amount of percent work per scheduling algorithm must add up to 1" << endl;
        return -1;
    }


    //Get the amount of processes each "processor" will handle
    vector<int> numProcesses;
    for (int i = 0; i < scheduleAlgos.size(); i++)
    {
        numProcesses.push_back(floor(processes.size() * percentWork.at(i)));
    }

    //Add up the processes so we can check if got them all
    int totalWork = 0;
    for (int i = 0; i < scheduleAlgos.size(); i++)
    {
        totalWork += numProcesses.at(i);
    }

    //Because of floor of decimals we probably didnt get all the processes assigned
    //Go through and add an additional process to each processors workload until we accounted for all of them
    int leftoverCounter = 0;
    while (totalWork != processes.size())
    {
        numProcesses.at(leftoverCounter) += 1;
        totalWork += 1;
        leftoverCounter += 1;
    }

    //cout << "total work:" << totalWork << endl;
    //cout << "actual process count" << processes.size() << endl;

    for (int i = 0; i < numProcesses.size(); i++)
    {
        cout << "Processor #" << i << " processes " << numProcesses.at(i) << endl;
    }
    fflush(stdout);

    

    //set up initial processor processes
    //cout << "NUMProcesses.size() " << numProcesses.size() << endl;
    //cout << "Processes.size() " << processes.size() << endl;
    for (int i = 0; i < numProcesses.size(); i++)
    {  
        vector<ProcessInfo> pi;
        processorProcesses.push_back(pi);
        for (int j = 0; j < numProcesses.at(i); j++)
        {
            processorProcesses.at(i).push_back(processes.back());
            processes.pop_back();
        }

    }

    //Passes all the PCBs with their associated algorithms to be sorted (only priority and shortest job will be sorted)
    sortAllPCB(processorProcesses, scheduleAlgos);

    cout << "Initial load of system before beginning execution:" << endl;
    balancePrint();
    cout << endl;

    
   

    pthread_t threads[processorProcesses.size()];
    for (int i = 0; i < processorProcesses.size(); i++)
    {
        if (scheduleAlgos.at(i) == 1)
        {
            int* a = new int(i);
            pthread_create(&threads[i], NULL, priority, (void*)a);
        }
        else if (scheduleAlgos.at(i) == 2)
        {
            int* a = new int(i);
            pthread_create(&threads[i], NULL, shortest, (void*)a);
        }
        else if (scheduleAlgos.at(i) == 3)
        {
            int* a = new int(i);
            pthread_create(&threads[i], NULL, roundRobin, (void*)a);
        }
        else if (scheduleAlgos.at(i) == 4)
        {
            int* a = new int(i);
            pthread_create(&threads[i], NULL, fcfs, (void*)a);
        }
    }

    for (int i = 0; i < processorProcesses.size(); i++)
    {
        pthread_join(threads[i], NULL);
    }

    cout << "All processes are done executing" << endl;



}


