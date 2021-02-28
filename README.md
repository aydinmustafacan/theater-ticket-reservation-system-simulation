


# Ticket Simulator Project by Mustafa Can Aydin




Main idea of the project is to implement threads in order to be used for ticket synchronization project. I implement in this project
 three teller-threads, some number of client threads determined by configuration file and a main process' thread.
Main thread will create all the other threads. Job of client threads is to ask for their ticket to be proceed to teller-threads
 and the teller threads will take care of ticket reservation system for the client threads. Each client thread is responsible for
 asking to tellers in order for their ticket to be reserved and once their ticket is reserved, client-thread will exit.
On the other hand teller-threads will wait until all the clients' tickets are reserved and once all the clients' tickets
has been reserved, program will complete execution.



# Compiling and Running 


In order to run program one must run make file first to compile the program into executanle called simulation
~~~~~~~~~~~~~~~{.cpp}
    make
    
~~~~~~~~~~~~~~~
After compilation is done, one has to run simulation  as shown below 

~~~~~~~~~~~~~~~{.cpp}
    ./simulation configuration_path output_path
~~~~~~~~~~~~~~~
