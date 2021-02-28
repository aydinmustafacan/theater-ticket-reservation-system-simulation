///@author Mustafa Can Aydin
///@note Main idea of the project is to implement threads in order to be used for ticket synchronization project. I implement in this project
/// three teller-threads, some number of client threads determined by configuration file and a main process' thread.
/// Main thread will create all the other threads. Job of client threads is to ask for their ticket to be proceed to teller-threads
/// and the teller threads will take care of ticket reservation system for the client threads. Each client thread is responsible for
/// asking to tellers in order for their ticket to be reserved and once their ticket is reserved, client-thread will exit.
///On the other hand teller-threads will wait until all the clients' tickets are reserved and once all the clients' tickets
/// has been reserved, program will complete execution.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <unistd.h>
#include <csignal>
#include <sys/stat.h>
#include <fcntl.h>
#include <map>
#include <unordered_set>
#include <vector>
#include <pthread.h>




pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_t main_tid;


//capacity of the theatre
int cap_of_theatre;
//an unordered set to determine taken seats
std::unordered_set<int > reservedSeats;


/*
 * HERE IS THE GLOBAL VARIABLES THAT MY CLIENT THREADS WILL CHANGE AND TELLER THREAD WILL USE
 */
char *currClientsName;
int currClientsDesiredSeat;
int currClientsFinalSeat;
int currClientsServiceTime;



/*
 * delimiter helper method, used to split string by ','
 */
void tokenize(std::string const &str, const char delim,
              std::vector<std::string> &out)
{
    // construct a stream from the string
    std::stringstream ss(str);

    std::string s;
    while (std::getline(ss, s, delim)) {
        out.push_back(s);
    }
}



/*
 * Helper function  which is used to determine available seat for clients
 */
int minimumUnusedSeatNumberFinder(){
    for(int i=1;i<cap_of_theatre;i++){
        if (reservedSeats.find(i) != reservedSeats.end()){
            //meaning set includes i then we need to look for another seat number
        }
        else{
            return i;
        }

    }
    return cap_of_theatre;
}

/*
 * Helper function  which is used to determine available seat for clients
 */
bool isSeatReserved(int seat){
    bool res;
    if (reservedSeats.find(seat) != reservedSeats.end()){
        res= true;
    }
    else{
        res= false;
    }
    return res;

}

/*
 * Helper function which is used to determine available seat for clients
 */
int returnCorrectSeatNumberForTheClient(int desiredSeat){
    int res;
    if(!isSeatReserved(desiredSeat)){
        //it's not reserved thus it is available. We return it
        res=desiredSeat;
        reservedSeats.insert(desiredSeat);
    }
    else{
        res=minimumUnusedSeatNumberFinder();
        reservedSeats.insert(res);
    }
    return res;
}



//output path of the program
char* output_path;

//thread ids for teller threads
pthread_t threadID_A;
pthread_t threadID_B;
pthread_t threadID_C;



//helper variables to be used by client-threads and teller-threads
bool avail_A= true;
bool avail_B= true;
bool avail_C= true;

//this function is for client-threads. Through this function client-threads ask to teller-threads using common
//global variables and mutex for their tickets to be reserved and once they ask and wait for teller-thread to
//reserve their tickets, client-tickets exit
void* makeReservation(void *args){

        usleep(100);
    std::string *ptr = (std::string *) args;
    std::string informationOfTheClient = *ptr;
    std::cout << informationOfTheClient << std::endl;
        const char delim = ',';
        std::vector<std::string> out;
        tokenize(informationOfTheClient, delim, out);
    std::string nameOfThisClient = out[0];
        int arrTimeOfThisClient = stoi(out[1]);
        int serviceTimeOfThisClient = stoi(out[2]);
        int desiredSeatNumberOfThisClient =stoi(out[3]);
        usleep(10000*arrTimeOfThisClient);//arrTime* cs sleep
    std::cout << " name : " << nameOfThisClient << " arrTime : " << arrTimeOfThisClient << " serTime : "<< serviceTimeOfThisClient<< " seat : "<<desiredSeatNumberOfThisClient<< std::endl;


        //lets convert name from string to char array
        char * name_of_this_client_char_array = new char[nameOfThisClient.size() + 1];
        std::copy(nameOfThisClient.begin(), nameOfThisClient.end(), name_of_this_client_char_array);
        name_of_this_client_char_array[nameOfThisClient.size()] = '\0';



        if (avail_A){
            int availableSeatNumber = returnCorrectSeatNumberForTheClient(desiredSeatNumberOfThisClient);
            pthread_mutex_lock(&mutex);
            // Start critical section
            currClientsName=name_of_this_client_char_array;
            currClientsDesiredSeat=desiredSeatNumberOfThisClient;
            currClientsFinalSeat=availableSeatNumber;
            currClientsServiceTime=serviceTimeOfThisClient;
            // End critical section
            pthread_mutex_unlock(&mutex);
            avail_A=false;
            usleep(10000*serviceTimeOfThisClient);




        }
        else if(avail_B){
            int availableSeatNumber= returnCorrectSeatNumberForTheClient(desiredSeatNumberOfThisClient);
            pthread_mutex_lock(&mutex);
            // Start critical section
            currClientsName=name_of_this_client_char_array;
            currClientsDesiredSeat=desiredSeatNumberOfThisClient;
            currClientsFinalSeat=availableSeatNumber;
            currClientsServiceTime=serviceTimeOfThisClient;
            // End critical section
            pthread_mutex_unlock(&mutex);
            avail_B=false;
            usleep(10000*serviceTimeOfThisClient);





        }
        else if(avail_C){
            int availableSeatNumber = returnCorrectSeatNumberForTheClient(desiredSeatNumberOfThisClient);
            pthread_mutex_lock(&mutex);
            // Start critical section
            currClientsName=name_of_this_client_char_array;
            currClientsDesiredSeat=desiredSeatNumberOfThisClient;
            currClientsFinalSeat=availableSeatNumber;
            currClientsServiceTime=serviceTimeOfThisClient;
            // End critical section
            pthread_mutex_unlock(&mutex);
            avail_C=false;
            usleep(10000*serviceTimeOfThisClient);





        }




        pthread_exit(NULL);

    return NULL;
}


//function for teller thread A
//this function will be in loop waiting for any client to come ask for his/her ticket to be reserved.
//After the client asks to teller than teller-thread will proceed to reserve the ticket and write corresponding informations
// to the output file.
void* tellerA( void *arg){

    threadID_A = pthread_self();

    while (1){
        usleep(60);
        if(!avail_A){

            int time_to_wait = 10000*currClientsServiceTime;
            std::cout << "Teller-A has work to do! also service time equals to "<<currClientsServiceTime<<"\n";
            char * curr_name_of_this_client = currClientsName;
            int curr_desired_seat_of_this_client= currClientsDesiredSeat;
            int curr_final_seat_of_this_client = currClientsFinalSeat;
            usleep(time_to_wait);
            pthread_mutex_lock(&mutex);
            FILE *fptr1;
            fptr1 = fopen(output_path, "a");
            fprintf(fptr1, "%s request seat %d, reserves seat %d. Signed by Teller A.\n",curr_name_of_this_client,curr_desired_seat_of_this_client,curr_final_seat_of_this_client);
            fclose(fptr1);
            pthread_mutex_unlock(&mutex);

            avail_A=true;
        }
    }
    pthread_exit(NULL);

}

//function for teller thread B
//this function will be in loop waiting for any client to come ask for his/her ticket to be reserved.
//After the client asks to teller than teller-thread will proceed to reserve the ticket and write corresponding informations
// to the output file.
void* tellerB( void *arg){


    threadID_B = pthread_self();

    while (1){
        usleep(60);
        if(!avail_B){

            int time_to_wait = 10000*currClientsServiceTime;
            std::cout << "Teller-B has work to do! also service time equals to "<<currClientsServiceTime<<"\n";
            char * curr_name_of_this_client = currClientsName;
            int curr_desired_seat_of_this_client= currClientsDesiredSeat;
            int curr_final_seat_of_this_client = currClientsFinalSeat;
            usleep(time_to_wait);
            pthread_mutex_lock(&mutex);
            FILE *fptr2;
            fptr2 = fopen(output_path, "a");
            fprintf(fptr2, "%s request seat %d, reserves seat %d. Signed by Teller B.\n",curr_name_of_this_client,curr_desired_seat_of_this_client,curr_final_seat_of_this_client);
            fclose(fptr2);
            pthread_mutex_unlock(&mutex);

            avail_B=true;
        }
    }

    return NULL;
}

//function for teller thread C
//this function will be in loop waiting for any client to come ask for his/her ticket to be reserved.
//After the client asks to teller than teller-thread will proceed to reserve the ticket and write corresponding informations
// to the output file.
void* tellerC( void *arg){
    threadID_C = pthread_self();

    while (1){
        usleep(60);
        if(!avail_C){

            int time_to_wait = 10000*currClientsServiceTime;
            std::cout << "Teller-C has work to do! also service time equals to "<<currClientsServiceTime<<"\n";
            char * curr_name_of_this_client = currClientsName;
            int curr_desired_seat_of_this_client= currClientsDesiredSeat;
            int curr_final_seat_of_this_client = currClientsFinalSeat;
            usleep(time_to_wait);
            pthread_mutex_lock(&mutex);
            FILE *fptr3;
            fptr3 = fopen(output_path, "a");
            fprintf(fptr3, "%s request seat %d, reserves seat %d. Signed by Teller C.\n",curr_name_of_this_client,curr_desired_seat_of_this_client,curr_final_seat_of_this_client);
            fclose(fptr3);
            pthread_mutex_unlock(&mutex);

            avail_C=true;
        }
    }

    return NULL;
}


///@param argc Number of arguments
///@param argv[] Vector of arguments
int main(int argc, char* argv[]) {

    if (argc != 3) {   /* check for valid number of command-line arguments     */
        printf("Wrong number of arguments \n");
        return 1;
    }
    main_tid= pthread_self();

    output_path = argv[2];
    char *config_path = argv[1];
    std::string inputPath = (std::string) config_path;
    int numberOfClients;// number of clients
    std::string nameOfTheTheatre;//name of the theatre

    FILE *fptrcleaning;
    fptrcleaning = fopen(output_path, "w");
    fclose(fptrcleaning);


    FILE *fptr;
    fptr = fopen(output_path, "a");
    fprintf(fptr, "Welcome to the Sync-Ticket!\n");
    fprintf(fptr, "Teller A has arrived.\n");
    fprintf(fptr, "Teller B has arrived.\n");
    fprintf(fptr, "Teller C has arrived.\n");
    fclose(fptr);



    /*
     * Teller threads should be created here
     */
    pthread_create(&threadID_A, NULL, tellerA, NULL);
    pthread_create(&threadID_B, NULL, tellerB, NULL);
    pthread_create(&threadID_C, NULL, tellerC, NULL);



    std::vector<std::string> client_information_vector;
    std::string info;


    // name of the theatre and num of clients will be read
    std::string inst, token1, token2, token3,token4;
    std::ifstream inst_file(inputPath);
    if (inst_file.is_open()) {
        int cntr = 1;
        while (getline(inst_file, inst)) {
            std::stringstream splitline(inst);

            if (cntr == 1) {
                splitline >> token1;
                std::cout << "current instruction is " << token1 << std::endl;
                nameOfTheTheatre = token1;
                cntr++;
            } else if (cntr == 2) {
                splitline >> token1;
                std::cout << "current instruction is " << token1 << std::endl;
                numberOfClients= stoi(token1);
                cntr++;
            } else {
                splitline>>token1>>token2>>token3>>token4;
                info= token1;

                std::cout << "information: "<<info<<std::endl;
                client_information_vector.push_back(info);



            }
        }
    }

    std::cout << "name of theatre is "<<nameOfTheTheatre << std::endl;
    std::cout << "number of clients is "<<numberOfClients << std::endl;

// name of the theatre and num of clients have been read :)

    if (nameOfTheTheatre == "OdaTiyatrosu") {
        cap_of_theatre = 60;
    } else if (nameOfTheTheatre == "UskudarStudyoSahne") {
        cap_of_theatre = 80;
    } else if (nameOfTheTheatre == "KucukSahne") {
        cap_of_theatre = 200;
    }

    std::cout << "capacity of the theatre is : " << cap_of_theatre << std::endl;
    //now clients will be coming to the queue :)
        for(int i=0;i<numberOfClients;i++){
            pthread_t threadID;

            std::string temp = client_information_vector[i];
            std::cout << "information of the client : " << temp << std::endl ;

            pthread_create(&threadID, NULL, makeReservation,&temp);
            usleep(10200);
            if(i==0){
                usleep(10000);
            }


        }











    usleep(500000*numberOfClients);


        return 0;
    }


