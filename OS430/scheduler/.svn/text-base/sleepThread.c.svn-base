#include "scheduler/scheduler.h"

void sleepThread(long int time) {
  /*long int targetSchedulerTime;

  targetSchedulerTime = time + schedulerTime();

  while (schedulerTime() - targetSchedulerTime < 0)
    yieldThread();*/

  long int targetSchedulerTime, currentSchedulerTime;

  currentSchedulerTime = schedulerTime();
  targetSchedulerTime = time + currentSchedulerTime;

  while (currentSchedulerTime - targetSchedulerTime < 0) {
    yieldThread();
    currentSchedulerTime = schedulerTime();
  }
}
