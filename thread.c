/*
Malcolm Roddy 
Lab1
CECS 326
Due Date: 03/01/24

1. Pthread allows for multiple threads to be created and used.
 The main or first thread is the parent, which creates the child thread. 
Upon creation, with the thread ID and assigned routine called "child" it loops infinitly for the duration of its execution. Upon the main thread calling to cancel the child thread, it will terminate asynchronously while its cleanup handlers and destructors are called. To confirm the cancelation is completed, pthread_join waits for the thread to terminate and recover its exit status. Once confirmed, the main thread will sleep and print a message. 

2. THe second thread is able to continue in its job independently
 from the main thread because it is not dependent on the main thread.
 The main thread is the parent, which creates the child thread. Since 
there are no shared resources there are no conflucts between them. 
So as the main thread waits on the user the child thread will continue to print a message infinitly.  
*/

#include <unistd.h>
#include <stdio.h>
#include <pthread.h>

/*Second thread code to be executed upon creation. Simple infinite loop until main thread calls to terminate it*/
static void *child(void *ignored){
  /*this sets the thread to cancel instantly instead
  of waiting for sleep to execute. old type is NULL since its being initialized*/
  int code = pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
  if(code){
    fprintf(stderr, "pthread_setcancelstate failed with code %d\n", code); 
  }
  //infinite loop
  while(1){
    //continuously show message and sleep to free up cpu
    sleep(3);
    printf("Child is done sleeping 3 seconds.\n");
  }
  return NULL;
}

int main(int argc, char *argv[]){
   pthread_t child_thread;
   int code;
  //create child thread
   code = pthread_create(&child_thread, NULL, child, NULL);
   if(code){
      fprintf(stderr, "pthread_create failed with code %d\n", code);
   }
  //wait for user to press enter
  printf("Press enter to kill child thread.\n");
  while(getchar() != '\n');
  /*cancel the second thread. It is a request so the thread is not confirmed cancelled yet*/
  code = pthread_cancel(child_thread);
  if(code){
    fprintf(stderr, "pthread_cancel failed with code %d\n", code);
  }
  printf("Terminating second thread.\n");
  /*Confirm child thread has been killed. Can only use pthread_join, and void pointer to get confirmed exit status of targeted thread*/
  void* res;
  code = pthread_join(child_thread, &res);
  if(code){
    fprintf(stderr, "pthread_join failed with code %d\n", code);
  }
  //res holds exit status of terminated child thread
  if(res == PTHREAD_CANCELED){
    sleep(5);
    printf("Parent is done sleeping 5 seconds.\n");
    return 0;
  }
}
