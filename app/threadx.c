#include <stdint.h>
#include <stdio.h>
#include "common.h"
#include "console.h"
#include "tx_api.h"


#define THREAD1_PRIO         2
#define THREAD1_STACK_SIZE   1024
static  TX_THREAD thread1;
uint8_t thread1_stack[THREAD1_STACK_SIZE];

#define THREAD2_PRIO         1
#define THREAD2_STACK_SIZE   1024
static  TX_THREAD thread2;
uint8_t thread2_stack[THREAD2_STACK_SIZE];

volatile uint32_t thread1_cnt=0;
volatile uint32_t thread2_cnt=0;

void my_thread1_entry(ULONG thread_input)
{
  /* Enter into a forever loop. */
  while(1)
  {
    console_printf("threadx 1 application running %d times, thread2_cnt:%d",++thread1_cnt, thread2_cnt);
    tx_thread_relinquish();
    //tx_thread_sleep(100);
  }
}

void my_thread2_entry(ULONG thread_input)
{
  /* Enter into a forever loop. */
  while(1)
  {
    console_printf("threadx 2 application running %d times",++thread2_cnt);

    tx_thread_sleep(10);
  }
}

void tx_application_define(void *first_unused_memory)
{
  /* Create thread */

  //tx_thread_create(&thread0, "thread 0", my_thread0_entry, 0, &thread0_stack[0], THREAD0_STACK_SIZE, THREAD0_PRIO, THREAD0_PRIO, TX_NO_TIME_SLICE, TX_AUTO_START);
  tx_thread_create(&thread1, "thread 1", my_thread1_entry, 0, &thread1_stack[0], THREAD1_STACK_SIZE, THREAD1_PRIO, THREAD1_PRIO, TX_NO_TIME_SLICE, TX_AUTO_START);

  tx_thread_create(&thread2, "thread 2", my_thread2_entry, 0, &thread2_stack[0], THREAD2_STACK_SIZE, THREAD2_PRIO, THREAD2_PRIO, TX_NO_TIME_SLICE, TX_AUTO_START);
}

int main(void)
{
  console_printf("\nBuild in %s(%s)\n", __DATE__,__TIME__);
  console_printf("threadX RTOS start\n");

  tx_kernel_enter( );
  hang();
}
