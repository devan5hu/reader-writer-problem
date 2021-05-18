// Reading Processes: 
#include <semaphore.h> 

Wait(mutex);

    if(a_writers == 0 && w_writers == 0){
        Signal(readsem);
        a_readers = a_readers + 1;
    }
    else{
        w_writers = w_writers + 1;
    }