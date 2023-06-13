#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
using namespace std;

const int numEatTimes = 3;
string ipa = "127.0.0.1";
string port = "54321";
int maxBurger = 10;
int burgersAte = 0;
int timeToEat [numEatTimes] = {1, 3, 5};
string clientNum = "";
const int MAX_CONNECT_ATTEMPTS = 10;

bool isPositiveInteger(string userInput) {
    if (userInput.find_first_not_of("0123456789") != string::npos || userInput == "") {
        return false;
    }
    return true;
}

bool isIpAddress(string userInput) {
    if (userInput.find_first_not_of(".0123456789") != string::npos || userInput == "") {
        return false;
    }
    return true;
}

bool validateInput(string userInput, int min, int max)
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


int main(int argc, char* argv[])
{
    srand(time(NULL));
    string clArguement = "";
    if (argc == 4)
    {
        clArguement = argv[1];
        if (!isIpAddress(clArguement))
        {
            cout << "Error: invalid IP address given" << endl;
            return -1;
        }
        ipa = argv[1];

        clArguement = argv[2];
        if (!isPositiveInteger(clArguement))
        {
            cout << "Error: invalid port given" << endl;
            return -1;
        }
        port = argv[2];

        clArguement = argv[3];
        if (!isPositiveInteger(clArguement))
        {
            cout << "Error: Number of burgers must be a positive integer" << endl;
            return -1;
        }
        maxBurger = stoi(argv[3]);
    }

    else if (argc != 1)
    {
        cout << "Error commands are: Connection IP address, port number, and the maximum number of burgers(in this particular order!)" << endl;
        cout << "OR" << endl;
        cout << "Provide no arguements to start with default settings" << endl;
        return -1;
    }

    //cout << "ipa " << ipa << " port " << port << " maxBurger" << maxBurger << endl;







    cout << "Attempting to connect" << endl;

    /// Create and initialize client socket
    /// ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);  // create a socket for the client
    if (client_socket == -1)
    {
        cout << "Error creating socket" << endl;
        return -1;
    }

    struct sockaddr_in server_address;  // define the server's address
    server_address.sin_family = AF_INET;  // specify the address family (IPv4)
    server_address.sin_addr.s_addr = inet_addr(ipa.c_str());  // specify the IP address of the server (localhost)
    server_address.sin_port = htons(54321);  // specify the port number to connect to

    for (int i = 0; i < MAX_CONNECT_ATTEMPTS; i++)
    {
        //connect to server socket
        if (connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {// connect to the server
            cout << "Error connecting" << endl;
            sleep(3);
            if (i == MAX_CONNECT_ATTEMPTS - 1)
            {
                cout << "Unable to connect to server" << endl;
                return -1;
            }
        }
        else
        {
            break;
        }

    }
    

    char whatClient[256];
    int isgood = recv(client_socket, whatClient, sizeof(whatClient), 0);
    if (isgood == -1)
    {
        cout << "Bad response" << endl;
        return -1;
    }
    else if (isgood == 0)
    {
        cout << "Server disconnected" << endl;
        return -1;

    }
    else
    {
        clientNum = whatClient;
    }
    /// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    while (burgersAte < maxBurger)
    {
        //cout << "trying to send" << endl;
        char sendMessage[] = "Ready for burger";
        send(client_socket, sendMessage, sizeof(sendMessage), 0);

        //cout << "send Message";

        char recieveMessage[256];
        int bytesBack = recv(client_socket, recieveMessage, sizeof(recieveMessage), 0);  // receive the message from the server
        if (bytesBack == -1)
        {
            cout << "Bad response" << endl;
        }
        else if (bytesBack == 0)
        {
            cout << "Server disconnected" << endl;
            return -1;
        }
        else if (strcmp(recieveMessage, "Here's your burger") == 0)
        {

            int thisTime = timeToEat[rand() % numEatTimes];
            sleep(thisTime);
            burgersAte += 1;
            cout << "Client#" << clientNum << ":" << endl;
            cout << "Eaten: " << burgersAte << " burgers" << endl;
            cout << "Recent burger took: " << thisTime << " seconds to eat" << endl;
            cout << "Burgers left to eat: " << (maxBurger - burgersAte) << endl;
            
        }
        else if (strcmp(recieveMessage, "No more burgers") == 0)
        {
            cout << "Client#" << clientNum << " has no more burgers to eat" << endl;
            fflush(stdout);
            break;
        }
        else if (strcmp(recieveMessage, "Gotta wait") == 0)
        {
            continue;
        }
    }

    if (maxBurger == burgersAte)
    {
        char sendMessage[] = "I'm full";
        send(client_socket, sendMessage, sizeof(sendMessage), 0);
        cout << "Client#" << clientNum << " is full" << endl;
    }
    
    
    close(client_socket);
    
}