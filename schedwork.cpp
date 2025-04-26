#ifndef RECCHECK
#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
// add or remove necessary headers as you please

#endif

#include "schedwork.h"

using namespace std;

// a constant that can be used to indicate an INVALID 
// worker ID if that is useful to your implementation.
// Feel free to not use or delete.
static const Worker_T INVALID_ID = (unsigned int)-1;


// Add prototypes for any helper functions here
bool canScheduleWorker(const AvailabilityMatrix& avail,
                       const size_t maxShifts,
                       DailySchedule& sched,
                       size_t day,
                       Worker_T worker);

bool scheduleHelper(const AvailabilityMatrix& avail,
                    const size_t dailyNeed,
                    const size_t maxShifts,
                    DailySchedule& sched,
                    vector<size_t>& shiftsPerWorker,
                    size_t day,
                    size_t slot);


// Add your implementation of schedule() and other helper functions here

bool schedule(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched
)
{
    if(avail.size() == 0U){
        return false;
    }
    sched.clear();
    // Add your code below
    sched.resize(avail.size(), vector<Worker_T>(dailyNeed, INVALID_ID));
    vector<size_t> shiftsPerWorker(avail[0].size(), 0);

    return scheduleHelper(avail, dailyNeed, maxShifts, sched, shiftsPerWorker, 0, 0);


}

//recursive helper for backtracking
bool scheduleHelper(const AvailabilityMatrix& avail,
                    const size_t dailyNeed,
                    const size_t maxShifts,
                    DailySchedule& sched,
                    vector<size_t>& shiftsPerWorker,
                    size_t day,
                    size_t slot)
{
  const size_t numDays = avail.size();
  const size_t numWorkers = avail[0].size();

  //base case: all days/slots filled
  if(day >= numDays){
    return true;
  }
  //all shifts for day filled->move to next day
  if(slot >= dailyNeed){
    return scheduleHelper(avail, dailyNeed, maxShifts, sched, shiftsPerWorker, day + 1, 0);
  }
  //try to schedule all workerss
  for(Worker_T worker = 0; worker < numWorkers; worker++){
    //check if current worker has available shifts
    if(avail[day][worker] && shiftsPerWorker[worker] < maxShifts){
      //check if current worker is already shifted for current day
      bool alreadySched = false;
      for (size_t i = 0; i < slot; i++){
        if(sched[day][i] == worker){
          alreadySched = true;
          break;
        }
      }
      //if not already scheduled for that day, assign that slot to worker
      if(!alreadySched){
        sched[day][slot] = worker;
        shiftsPerWorker[worker]++;
        //recursively try to fill all spots
        if(scheduleHelper(avail, dailyNeed, maxShifts, sched, shiftsPerWorker, day, slot + 1)){
          return true;
        }
        //if it doesn't work set back to normal
        sched[day][slot] = INVALID_ID;
        shiftsPerWorker[worker]--;
      }
    }
  }
  //if it gets here, one spot didn't work and then you backtrack
  return false;


}