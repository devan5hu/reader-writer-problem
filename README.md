# reader-writer-problem
The reader-writer problem relates to a resource such as a file that is shared between multiple processes. The first and second problem shows where a reader or writer respectively can possibly starve. This happens when when one of the two readers or the writers are given the priority to complete their respective task. Evidently, the other one starves. 

To solve this and make this a Starve-Free reader writer problem the solution idea is (Writing in psuedocode)-

The  main  idea  here  is  that  a  Writer  indicates  to  Readers  its  necessity  to  access  the working  area.  At  the  same  time  no  new  Readers  can  start  working.  Every  Reader leaving  the  working  area  checks  if  there  is  a  Writer  waiting  and  the  last  leaving Reader  signals  Writer  that  it  is  safe  to  proceed  now.  Upon  completing  access  to  the working area Writer signals waiting Readers that it finished allowing them to access the working area again. 

Considering a `reader` proccess we can formulate an overview of the solution -> 

## Overview of Solution

```C++
    if(active && waiting writers are zero)
	    signal && enter the process
    else 
        wait 
        // for the signal
	
    //Read

    if( is the last reader)
	    signal the waiting writers
```

## The Semaphore 
For a starve-free implementation, we need a semaphore that has a First-In-First-Out manner of handling the waiting processes. i.e. - the process that call wait first are the ones are the process that are given the access to semaphore first.

Before going to the semaphore first we need to implement the data structure `Queue` for the FIFO function.

Code:

```C++

struct Node{
    int data;
    Node *next;
}

struct Queue{
    Node* Front, Rear;
        void push(int data){
            Node newNode = new Node();
            newNode->data = data;
            if(Rear != NULL){
                Rear->next = newNode;
                Rear = newNode;
            }
            else{
                Front = Rear = newNode;
            }
        }
    
        int pop(){
            if(Front == NULL){
                // underflow condition
                return -1; 
                
            }
            else{
                int data = Front->data;
                Front = Front->next;
                free(Front)
                if(Front == NULL){
                    Rear = NULL;
                }
                return data;
            }
        }
}
```
Thus this implements the queue for the semaphore. Now to implement the semaphore we do the following. 

Code: 

```C++
{
    struct Semaphore{
        //value of semaphore
        int semaphore = 1;
        Semaphore(){
            semaphore = 1;
        }        
        Semaphore(int s){
            semaphore = s;
        }
        Queue* Q = new Queue();
        void wait(int process_id){
            semaphore--;
            //if all the resources are busy, push the process into the waiting queue ans block it.
            if(semaphore <= -1){
                Q->push(process_id);
                int pid = Q->rear;
                block(pid); 
                //process will be blocked until a signaled
            }
        }
        
        void signal(){
            semaphore++;
            if(semaphore < 1){
                int pid =  Q->pop();
                wakeup(pid); 
                // Assuming the pid is given, wakeup() will wakeup the process with given pid.
            }
        }
    }
}
```

Thus, a FIFO semaphore can be implemented. A queue is used to manage the waiting processes. The process gets blocked after pushing itself onto the queue and is woken up in FIFO order when some other process releases the semaphore.

Uptil now, out Semaphore structure consists of an integer semaphore with a default value of 1 and a queue Q. It also contains two functions: wait(int process_id) and signal().

### Implementing the optimized Starve Free Solution

Global Varibles -> 

``` C++
    Semaphore *mutex_in = new Semaphore();
    Semaphore *mutex_out = new Semaphore();
    Semaphore *sem_write = new Semaphore();

    //No of readers who started reading
    int start_readers = 0;
    //No of readers finished reading
    int completed_readers = 0;
    bool waiting_writers = 0;

```

## Implementation for Reader

```C++
//////////////////////// Entry Section /////////////////////////////
    bool flag = true;
        while(flag){    
            mutex_in -> wait(process_id);
            readers_started++;
            mutex_in -> signal();
        }
//////////////////////// Critical Section ////////////////////////

//////////////////////// Exit Section /////////////////////////////

    out_mutex -> wait(process_id);
    readers_completed++;
    if(waiting_writer && readers_started == readers_completed){
        sem_write -> signal();
    }
    mutex_out -> signal();

```

## Implementation for Writer

```C++
//////////////////////// Entry Section /////////////////////////////
    bool flag = true;
    while(true){
        mutex_in -> wait(process_id);
        mutex_out -> wait(process_id);
        if(started_readers == completed_readers){
            mutex_out -> signal();
        }
        else{
            waiting_writer = true;
            mutex_out -> signal();
            sem_writer -> wait();
            waiting_writer = false;
        }
//////////////////////// Critical Section ////////////////////////

//////////////////////// Exit Section /////////////////////////////

    mutex_in -> signal();
    }
```

## Explaination 
The queue of mutex_in serves as a common waiting queue for the readers and writers.

### for Writers

In the begining both readers and writers are in the mutex_in. After that it goes on to wait in mutex_out. it then compares the variables started_readers and completed_readers. If they turn out to be equal, all readers that had started reading has completed. This means achieving mutual exclusion, the writer signals the out_mutex and releases it for the critical section. If started_readers and completed_readers are not equal then this means a process is being carried out in the critical section, then the writer changes its waiting_writer to true, and then signals the mutex_out. Further the writer waits in the sem_writer, later from there it sets the waiting_writers to false and moves ahead to critical section. After the writer is done processing it signals the mutex_in stating that it does not need the resource anymore.

### for Readers

Again to ensure equal priority the reader and writers line up in the mutex_in. (Assuming a writer just completed its execution and leaves the critical section ) A reader acquires the mutex_in ,it shows its presence by increasing the variable started_readers and then immediately signals the mutex_in. As there are no conditions to keep the reader waiting except the wait in in the mutex_in, it then proceeds to critical section. Later the reader has to increment readers_completed therefore it waits for the mutex_out, once it has acquired it, it no longer needs the resource. Later waiting_writers is checked for any writers, if yes, readers are checked in the critical section. If not writer signals the mutex_out for its execution. (Same process mentioned above is repeated)

## References 

    - Abraham Silberschatz, Peter B. Galvin, Greg Gagne - Operating System Concepts
    - [arXiv:1309.4507](https://arxiv.org/abs/1309.4507)







