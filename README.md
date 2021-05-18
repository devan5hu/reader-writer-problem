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
            Node *newNode = (Node*)malloc(sizeof(Node));
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
        int data = 1;
        Queue* Q = new Queue();
        void wait(semaphore **s , int pid){
            *s->data--;
            if(*s->data <= -1){
                *s->Q.push(process_id);
                block(); 
                //process will be blocked until a signaled
            }
        }
        
        void signal(semaphore **s){
            *s->data++;
            if(*s->data < 1){
                int pid = *s->Q.pop();
                wakeup(pid); 
                // Assuming the pid is given, wakeup() will wakeup the process with given pid.
            }
        }
    }
}
```

Thus, a FIFO semaphore can be implemented. A queue is used to manage the waiting processes. The process gets blocked after pushing itself onto the queue and is woken up in FIFO order when some other process releases the semaphore.









