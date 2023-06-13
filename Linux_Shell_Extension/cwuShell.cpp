#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <fstream>
#include <unistd.h>




using namespace std;


//tokenizes the user input and places them into a vector
//s is the text entered into the shell by the user
//returns vector that contains tokenized user input
vector<string> getTokens(string s) {
    vector<string> v;
    string delimiter = " ";

    size_t pos = 0;
    string token;
    while ((pos = s.find(delimiter)) != string::npos) {
        token = s.substr(0, pos);
        if (token != "" && token != " ") {
            v.push_back(token);
        } 
        s.erase(0, pos + delimiter.length());
    }
    v.push_back(s);
    return v;
}


//Takes the Tokens and converts them into a standardized format
//Example -l = -l   OR -lai = -l -a -i
void splitSwitches(string s, vector<string>& v) {
    if (s.length() > 1) {
        if (s.at(0) == '-') {
            for (int i = 1; i < s.length(); i++) {
                string s2;
                s2 = s.at(i);
                s2 = "-" + s2;
                v.push_back(s2);
            }
        }
    }
    
}


//Takes in a vector v and pulls the first string out of it, 
//removes that string from the vector
//then returns the string
string getNextToken(vector<string>& v) {
    if (!v.size() > 0) {
        return "error";
    }
    else {
        string nextToken = v.front();
        v.erase(v.begin());
        return nextToken;
    }
}


//Takes a vector of the command line options
//splits them apart into a standarized format (-l)
//returns a vector containing the command line options in a standardized format
vector<string> getSwitches(vector<string>& v) {
    vector<string> v2;
    int numberOfTokens = v.size();
    for (int i = 0; i < numberOfTokens; i++) {
        string currentToken = getNextToken(v);
        splitSwitches(currentToken, v2);
        
    }
    return v2;
}

//Called when using the 'exit' command
//Either exits with 0 if no exitcode was given
//or exits with the exitcode the user inputs
//if the user inputted exitcode is not valid, exits with code 1
void exitShell(vector<string>& tokens, int exitcode) {
    if (tokens.size() > 0) {
        string exitcode = getNextToken(tokens);
        if (exitcode == "-h" || exitcode == "-help") {
            cout << "Usage: " << "exit " << " [EXITCODE]" << endl;
            cout << "Exits the shell with the exitcode [EXITCODE]" << endl;
            cout << "If no [EXITCODE] is given, exits with code 0" << endl;
            cout << "If [EXITCODE] is erroneous, exits with code 1" << endl;
            return;
        }

        else if (exitcode.find_first_not_of("-0123456789") == string::npos) {
            int n = stoi(exitcode);
            exit(n);
               
        }
    }
    else {
       exit(exitcode);
    }

    cout << "Integer required for exit command arguement, exiting with 1 instead" << endl;
    exit(1);
}


//called when the using the 'cpuinfo' command
//depending on the options the user inputs different cpu information will be displayed
//if no valid option is inputed or one of the inputs is invalid
//will display a message to the user to on how to get help with the command
int cpuinfo(vector<string>& tokens, string& command) {
    //vector<string> switches;
    if (tokens.size() > 0) {
        tokens = getSwitches(tokens);
    }

    else {
        cout << "Enter a switch for the command " + command << endl;
        cout << "Switches for " + command + " are -c -t -n" << endl;
        cout << "Try \"cpuinfo -help\" for more information" << endl;
        return 1;
    }

    if (tokens.size() == 0) {
        cout << "Enter a switch for the command " + command << endl;
        cout << "Switches for " + command + " are -c -t -n" << endl;
        cout << "Try \"cpuinfo -help\" for more information" << endl;
        return 1;
    }


    while (tokens.size() > 0) {
        string switchToken = getNextToken(tokens);

        if (switchToken == "-h") {
            cout << "Usage: " << command << " OPTION" << endl;
            cout << "Prints information about the CPU depending on what OPTION is picked" << endl;
            cout << "OPTIONS:" << endl;
            cout << "-n        Displays the number of cores in the CPU" << endl;
            cout << "-c        Displays the clock speed in MHz of the CPU" << endl;
            cout << "-t        Displays the type of CPU in the system" << endl;
            return 0;
        }

        else if (switchToken == "-c") {
            ifstream myfile("/proc/cpuinfo");
            if (myfile.is_open())
            {
                string line;
                while (getline(myfile, line))
                {
                    if (line.find("cpu MHz") != string::npos) {
                        vector<string> cpuinfo = getTokens(line);
                        string name;
                        for (int i = 0; i < 3; i++) {
                            name = getNextToken(cpuinfo);
                        }
                        float clockSpeed = stof(name);
                        clockSpeed = clockSpeed / 1000;

                        cout << "Clock speed: " << clockSpeed << " GHz" << endl;
                        break;
                    }
                }
                myfile.close();
            }
        }

        else if (switchToken == "-t") {
            ifstream myfile("/proc/cpuinfo");
            if (myfile.is_open())
            {
                string line;
                while (getline(myfile, line))
                {
                    if (line.find("model name") != string::npos) {
                        vector<string> cpuinfo = getTokens(line);
                        string name;
                        for (int i = 0; i < 3; i++) {
                            name = getNextToken(cpuinfo);
                        }
                        cout << "CPU Type: " + name << endl;
                        break;
                    }
                    
                }
                myfile.close();
            }
        }
      

        else if (switchToken == "-n") {
            int cores = thread::hardware_concurrency();
            string c = to_string(cores);
            cout << "Number of cores: " + c << endl;
        }

        

        else {
            cout << command << ": invalid option -- '" << switchToken << "'" << endl;
            cout << "Try '" << command << " -help' for more information" << endl;
            return 1;
        }
    }
    return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//called when the using the 'meminfo' command
//depending on the options the user inputs different memory information will be displayed
//if no valid option is inputed or one of the inputs is invalid
//will display a message to the user to on how to get help with the command
int meminfo(vector<string>& tokens, string& command) {
    //vector<string> switches;
    if (tokens.size() > 0) {
        tokens = getSwitches(tokens);
    }

    else {
        cout << "Enter a switch for command " + command << endl;
        cout << "Switches for " + command + " are -c -t -u" << endl;
        cout << "Try \"meminfo -help\" for more information" << endl;
        return 1;
    }

    if (tokens.size() == 0)
    {
        cout << "Enter a switch for command " + command << endl;
        cout << "Switches for " + command + " are -c -t -u" << endl;
        cout << "Try \"meminfo -help\" for more information" << endl;
        return 1;
    }

    

    while (tokens.size() > 0) {
        string switchToken = getNextToken(tokens);


        if (switchToken == "-h") {
            cout << "Usage: " << command << " OPTION" << endl;
            cout << "Prints information about the CPU depending on what OPTION is picked" << endl;
            cout << "OPTIONS:" << endl;
            cout << "-c        Displays the cache size in bytes of the CPU cores" << endl;
            cout << "-t        Displays the total amount of available RAM in bytes" << endl;
            cout << "-u        Displays the current used RAM in bytes" << endl;
            return 0;
        }

        else if (switchToken == "-c") {
            long l2 = sysconf(_SC_LEVEL2_CACHE_SIZE); //returns in kb
            cout << "L2 Cache per core in bytes: " << l2 << endl;
        }

        else if (switchToken == "-t") {
            ifstream myfile("/proc/meminfo");
            if (myfile.is_open())
            {
                string line;
                while (getline(myfile, line))
                {
                    if (line.find("MemAvailable") != string::npos) {
                        vector<string> meminfo = getTokens(line);
                        string name;
                        for (int i = 0; i < 2; i++) {
                            name = getNextToken(meminfo);
                        }
                        unsigned long kbytes = stoi(name);
                        unsigned long bytes = kbytes * 1000;
                        cout << "Total number of available bytes in RAM: " << bytes << endl;
                        break;
                    }
                }
                myfile.close();
            }
        }

        else if (switchToken == "-u") {
            ifstream myfile("/proc/meminfo");
            if (myfile.is_open())
            {
                string line;
                string total;
                string free;
                while (getline(myfile, line))
                {
                    if (line.find("MemTotal") != string::npos) {
                        vector<string> cpuinfo = getTokens(line);
                        for (int i = 0; i < 2; i++) {
                            total = getNextToken(cpuinfo);
                        }
                    }
                    if (line.find("MemFree") != string::npos) {
                        vector<string> cpuinfo = getTokens(line);
                        for (int i = 0; i < 2; i++) {
                            free = getNextToken(cpuinfo);
                        }
                    }

                }
                myfile.close();
                unsigned long totalKbytes = stoi(total);
                unsigned long freeKbytes = stoi(free);
                unsigned long usedBytes = (totalKbytes - freeKbytes) * 1000;
                cout << "Number of used bytes in RAM: " << usedBytes << endl;
            }
        }

        

        else {
            cout << command << ": invalid option -- '" << switchToken << "'" << endl;
            cout << "Try '" << command << " -help' for more information" << endl;
            return 1;
        }
    }
    return 0;

}

//called when using the 'prompt' command
//if there is no arguement following the prompt command, changes the prompt back to cwushell>
//if there is an arguement following the prompt command, changes the prompt to the arguement
int changePrompt(vector<string>& tokens, string& prompt) {
    if (tokens.size() > 0) {
        string newPrompt = getNextToken(tokens);

        if (newPrompt == "-h" || newPrompt == "-help") {
            cout << "Usage: " << "prompt" << " NEW_PROMPT" << endl;
            cout << "Changes the prompt (" << prompt << ") to NEW_PROMPT " << endl;
            cout << "Or" << endl;
            cout << "Usage: prompt" << endl;
            cout << "Changes the current prompt back to the original prompt (cwushell>)" << endl;
            return 0;
        }

        else if (newPrompt == "" || newPrompt == " ")
        {
            while (tokens.size() > 0)
            {
                newPrompt = getNextToken(tokens);
            }

            if (newPrompt != "") {
                prompt = newPrompt;
                return 0;
            }
        }

        else {
            prompt = newPrompt;
            return 0;
        }
        
    }
    prompt = "cwushell> ";
    return 0;
}


//called when using the manual command
//reads the help.txt file containing information about the commands
//and prints them out
void manual() {
    cout << "----------------------------------------------------------------------" << endl;
    cout << endl;
    cout << "NAME" << endl;
    cout <<  "exit - exit out of cwushell" << endl;
    cout << endl;
    cout <<   "SYNOPSIS" << endl;
    cout <<   "exit [EXITCODE]" << endl;
    cout << endl;
    cout <<    "DESCRIPTION" << endl;
    cout <<    "Exits out of cwushell. If no[EXITCODE] is given exits with the most recent exitcode." << endl;
    cout <<    "[EXITCODE] must be an integer if it is the system will return the given [EXITCODE]." << endl;
    cout <<    "if a erroneous [EXITCODE] is given, the system exits with code 1." << endl;
    cout <<        "----------------------------------------------------------------------" << endl;
    cout << endl;
    cout <<        "NAME" << endl;
    cout <<        "prompt - changes the prompt of the cwu shell" << endl;
    cout << endl;
    cout <<       "SYNOPSIS" << endl;
    cout <<     "prompt [NEW_PROMPT]" << endl;
    cout << endl;
    cout <<       "DESCRIPTION" << endl;
    cout <<        "The default prompt is \"cwushell>\", calling this command with a[NEW_PROMPT] arguement will change the default prompt to[NEW_PROMPT]." << endl;
    cout <<        "Invoking this command without[NEW_PROMPT] will change the prompt back to the default prompt \"cwushell>\"." << endl;
    cout <<        "----------------------------------------------------------------------" << endl;
    cout << endl;
    cout <<        "NAME" << endl;
    cout <<        "cpuinfo - displays info about the CPU" << endl;
    cout << endl;
    cout <<        "SYNOPSIS" << endl;
    cout <<        "cpuinfo SWITCH" << endl;
    cout << endl;
    cout <<        "DESCRIPTION" << endl;
    cout <<        "cpuinfo must include a SWITCH when the command is called, multiple SWITCHES can be used at once." << endl;
    cout <<        "Depending on the SWITCH, different CPU information will be displayed." << endl;
    cout << endl;
    cout <<        "SWITCHES:" << endl;
    cout << endl;
    cout << "- c" << endl;
    cout <<    "displays the CPU clock speed" << endl;
    cout << endl;
    cout <<   "- t" << endl;
    cout <<    "displays the type of the CPU" << endl;
    cout << endl;
    cout <<    "- n" << endl;
    cout <<    "displays the number of cores on the CPU" << endl;
    cout <<    "----------------------------------------------------------------------" << endl;
    cout << endl;
    cout <<    "NAME" << endl;
    cout <<    "meminfo - displays info about the system memory" << endl;
    cout << endl;
    cout <<     "SYNOPSIS" << endl;
    cout <<    "meminfo SWITCH" << endl;
    cout << endl;
    cout <<    "DESCRIPTION" << endl;
    cout <<    "meminfo must include a SWITCH when the command is called, multiple SWITCHES can be used at once." << endl;
    cout <<    "Depending on the SWITCH, different memory information will be displayed." << endl;
    cout << endl;
    cout <<    "SWITCHES:" << endl;
    cout << endl;
    cout << "- c" << endl;
    cout <<    "displays the size of the L2 cache on a processor core" << endl;
    cout << endl;
    cout <<    "- t" << endl;
    cout <<    "displays the amount of available RAM on the system" << endl;
    cout << endl;
    cout <<    "- u" << endl;
    cout <<    "displays the amount of used RAM on the system" << endl;
    cout <<    "----------------------------------------------------------------------" << endl;
}
















int main() {
    int exitcode = 0;
    string prompt = "cwushell> ";
    vector<string> tokens;
    
    //Shell loop
    while (true){
        //prints prompt
        cout << prompt;
        string s = "";

        //gets the input from the user
        getline(cin, s);

        //takes input and tokenizes it
        if (s.length() > 0) {
            tokens = getTokens(s);
        }
        else {
            continue;
        }


        //gets the command the user entered
        string command = "";
        if (tokens.size() > 0) {
            command = getNextToken(tokens);
            while (command == "" || command == " ")
            {
                command = getNextToken(tokens);
            }

        }


        if (command == "exit") {
            exitShell(tokens, exitcode);
        }//end of exit
    


        else if (command == "prompt") {
            exitcode = changePrompt(tokens, prompt);
        }//end of prompt


        else if (command == "cpuinfo") {
            exitcode = cpuinfo(tokens, command);
        }//end of cpuinfo

        else if (command == "meminfo"){
            exitcode = meminfo(tokens, command);
        }

        else if (command == "manual") {
            manual();
        }

        else {
            exitcode = system(s.c_str());
            //cout << exitcode << endl;
        }

    }

    return 0;
}

