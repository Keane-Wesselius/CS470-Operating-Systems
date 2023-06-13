#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <cstring>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/time.h>

using namespace std;

pthread_mutex_t mutex;

const int numCookTimes = 2;
int maxBurgers = 25;
int burgersMade = 0;
int burgersAvailable = 0;
const int MAX_CHEFS = 100;
int numChefs = 3;
int timeToCook[numCookTimes] = { 2, 4 };
int server_socket;
int clients = 0;
int chefs = 0;


void* makeBurgers(void* arg)
{
    pthread_mutex_lock(&mutex);
    int chef = chefs;
    chefs++;
    pthread_mutex_unlock(&mutex);
    int chefBurgers = 0;
    while (burgersMade < maxBurgers)
    {
        bool didMake = false;
        int thisTime = timeToCook[rand() % numCookTimes];
        sleep(thisTime);
        pthread_mutex_lock(&mutex);
        if (burgersMade < maxBurgers)
        {
            didMake = true;
            burgersMade++;
            burgersAvailable++;
        }
        pthread_mutex_unlock(&mutex);

        if (didMake)
        {
            chefBurgers++;
            cout << "Chef#" << chef << ":" << endl;
            cout << "Total burgers made: " << chefBurgers << endl;
            cout << "Recent burger took: " << thisTime << " seconds" << endl;
            cout << "Burgers remaining: " << (maxBurgers - burgersMade) << endl << endl;
        }

    }
    return NULL;
}

void* handle_client(void* arg) {
    cout << "Found client" << endl;

    pthread_mutex_lock(&mutex);
    string clientNum = to_string(clients);
    clients++;
    pthread_mutex_unlock(&mutex);

    int client_socket = *(int*)arg;

    const char* clientN = clientNum.c_str();
    int result = send(client_socket, clientN, sizeof(clientN), 0);
    if (result == -1)
    {
        cout << "Client#" << clientNum << " stopped recieving messages" << endl;
        fflush(stdout);
        close(client_socket);
        return NULL;
    }
    else if (result == 0)
    {
        cout << "Client#" << clientNum << " has disconnected" << endl;
        fflush(stdout);
        close(client_socket);
        return NULL;
    }

    while (burgersMade < maxBurgers || burgersAvailable > 0)
    {
        //cout << "about to recieve" << endl;
        char recieveMessage[256];
        int result = recv(client_socket, recieveMessage, sizeof(recieveMessage), 0);
        if (result == -1)
        {
            cout << "Client#" << clientNum << " stopped recieving messages" << endl;
            fflush(stdout);
            close(client_socket);
            return NULL;
        }
        else if (result == 0)
        {
            cout << "Client#" << clientNum << " has disconnected" << endl;
            fflush(stdout);
            close(client_socket);
            return NULL;
        }
        //cout << "recieved" << endl;
        //cout << recieveMessage << endl;
        if (strcmp(recieveMessage, "Ready for burger") == 0)
        {
            if (burgersAvailable > 0)
            {
                pthread_mutex_lock(&mutex);
                burgersAvailable--;
                int left = burgersAvailable;
                pthread_mutex_unlock(&mutex);
                cout << "Served client#" << clientNum << " Burgers left to serve: " << left << endl;

                char message[] = "Here's your burger";
                result = send(client_socket, message, sizeof(message), 0);
                if (result == -1){
                    cout << "Client#" << clientNum << " stopped recieving messages" << endl;
                    fflush(stdout);
                    close(client_socket);
                    return NULL;
                }
                else if (result == 0)
                {
                    cout << "Client#" << clientNum << " has disconnected" << endl;
                    fflush(stdout);
                    close(client_socket);
                    return NULL;
                }
            }
            else
            {
                char message[] = "Gotta wait";
                result = send(client_socket, message, sizeof(message), 0);
                if (result == -1){
                    cout << "Client#" << clientNum << " stopped recieving messages" << endl;
                    fflush(stdout);
                    close(client_socket);
                    return NULL;
                }
                else if (result == 0)
                {
                    cout << "Client#" << clientNum << " has disconnected" << endl;
                    fflush(stdout);
                    close(client_socket);
                    return NULL;
                }
            }
        }
        else if (strcmp(recieveMessage, "I'm full") == 0)
        {
            close(client_socket);
            return NULL;
        }
    }

    if (burgersMade == maxBurgers || burgersAvailable == 0)
    {
        char message[] = "No more burgers";
        result = send(client_socket, message, sizeof(message), 0);
        if (result == -1){
            cout << "Client#" << clientNum << " stopped recieving messages" << endl;
            fflush(stdout);
            close(client_socket);
            return NULL;
        }
        else if (result == 0)
        {
            cout << "Client#" << clientNum << " has disconnected" << endl;
            fflush(stdout);
            close(client_socket);
            return NULL;
        }
    }

    close(client_socket);
    return NULL;
}



bool isPositiveInteger(string userInput) {
    if (userInput.find_first_not_of("0123456789") != string::npos || userInput == "") {
        return false;
    }
    return true;
}


int main(int argc, char* argv[])
{
    pthread_mutex_init(&mutex, NULL);
    srand(time(NULL));

    string clArguement = "";
    if (argc == 3)
    {

        clArguement = argv[1];
        if (!isPositiveInteger(clArguement))
        {
            cout << "Error: Max burgers must be a positive integer" << endl;
            return -1;
        }
        maxBurgers = stoi(argv[1]);

        clArguement = argv[2];
        if (!isPositiveInteger(clArguement))
        {
            cout << "Error: Number of servers must be a positive integer" << endl;;
            return -1;
        }
        numChefs = stoi(argv[2]);
        if (numChefs > 100)
        {
            cout << "Too many cooks in the kitchen! (Only 100 chefs allowed)" << endl;
            return -1;
        }
    }

    else if (argc != 1)
    {
        cout << "Error commands are: The maximum number of burgers to be made and the number of chefs (in this particular order!)" << endl;
        cout << "OR" << endl;
        cout << "Provide no arguements to start with default settings" << endl;
        return -1;
    }


    cout << "max burgers " << maxBurgers << " numchefs" << numChefs << endl;

    /// Create and initilize the server socket
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    server_socket = socket(AF_INET, SOCK_STREAM, 0);  // create a socket for the server
    if (server_socket == -1)
    {
        cout << "Error creating socket" << endl;
        return -1;
    }

    struct sockaddr_in server_address;  // define the server's address
    server_address.sin_family = AF_INET;  // specify the address family (IPv4)
    server_address.sin_addr.s_addr = INADDR_ANY;  // specify the IP address of the server (any interface)
    server_address.sin_port = htons(54321);  // specify the port number to listen on

    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) // bind the socket to the server address
    {
        cout << "Error binding the socket" << endl;
        return -1;
    }

    if (listen(server_socket, 10) == -1)
    {
        cout << "Error listening to socket" << endl;
        return -1;
    }// start listening for incoming connections (with a backlog of up to 5)
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


    pthread_t chefThreads[MAX_CHEFS];
    for (int i = 0; i < numChefs; i++)
    {
        pthread_create(&chefThreads[i], NULL, makeBurgers, NULL);

    }


    struct timeval timeout;
    timeout.tv_sec = 60; // set the timeout to 60 seconds
    timeout.tv_usec = 0;





    fd_set readfds;

    int currentServer = 0;
    pthread_t serverThreads[MAX_CHEFS];
    while (burgersMade < maxBurgers || burgersAvailable > 0) {
        FD_ZERO(&readfds); //zero out the fd set (reset it)
        FD_SET(server_socket, &readfds); //add the server_socket to the fd set

        int result = select(server_socket + 1, &readfds, NULL, NULL, &timeout);
        if (result == -1) {
            cout << "Error select malfunctioned" << endl;
            return -1;
        }
        else if (result == 0) {
            //select timed out, nothing to display to user 
        }
        else {
            int client_socket = accept(server_socket, NULL, NULL);
            if (client_socket != -1) {
                cout << "Found Client" << endl;
                pthread_create(&serverThreads[currentServer], NULL, handle_client, (void*)&client_socket);
                currentServer++;
            }
        }
    }


    for (int i = 0; i < numChefs; i++)
    {
        pthread_join(chefThreads[i], NULL);

    }

    for (int i = 0; i < currentServer; i++)
    {
        pthread_join(serverThreads[i], NULL);
    }


    cout << "All out of burgers, closing up shop" << endl;
    fflush(stdout);
    close(server_socket);  // close the server socket
    return 0;


}