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

bool solveScheduleHelper(
    size_t day,
    size_t workerSlot,
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched,
    std::vector<size_t>& shiftsWorked,
    size_t n,
    size_t k
);


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

    size_t n = avail.size(); // num days

    if (n == 0) {

        // no days to schedule
         sched.clear();
         return true;
    }

    size_t k = 0; // num workers

    if (n > 0) {
        k = avail[0].size(); 
    }

    // edge case: need workers per day, but there are no workers
    if (k == 0 && dailyNeed > 0) {
        return false;
    }

    // edge case: need more workers per day than the total number of workers available
    if (dailyNeed > k) {
         return false;
     }

    // edge Case: no workers needed per day
    if (dailyNeed == 0) {

        sched.assign(n, std::vector<Worker_T>());
        return true; // valid schedule
    }
    
    // set up for backtracking

    // initialize the schedule matrix structure
    sched.assign(n, std::vector<Worker_T>(dailyNeed)); 

    // initialize shift counts for each worker to zero.
    std::vector<size_t> shiftsWorked(k, 0);

    // begin trying to fill the schedule from day 0, worker 0
    return solveScheduleHelper(0, 0, avail, dailyNeed, maxShifts, sched, shiftsWorked, n, k);
}


bool solveScheduleHelper(
    size_t day,
    size_t workerSlot,
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched,
    std::vector<size_t>& shiftsWorked,
    size_t n,
    size_t k
) {

    // base case: we have successfully scheduled all days
    if (day == n) {
        return true; 
    }

    size_t nextDay = day;
    size_t nextSlot = workerSlot + 1;

    // if the current day's slots are all filled move to the next day
    if (nextSlot == dailyNeed) {
        nextDay = day + 1;
        nextSlot = 0;
    }

    // recursive call: terate through all possible workers
    for (Worker_T workerId = 0; workerId < k; ++workerId) {

        if (!avail[day][workerId]) {
            continue;
        }

        if (shiftsWorked[workerId] >= maxShifts) {
            continue;
        }

        bool alreadyScheduledToday = false;
        
        for (size_t slot = 0; slot < workerSlot; ++slot) { 
            if (sched[day][slot] == workerId) {
                alreadyScheduledToday = true;
                break;
            }
        }

        if (alreadyScheduledToday) {
            continue;
        }

        sched[day][workerSlot] = workerId;
        shiftsWorked[workerId]++;

        // call helper function to fill the next slot
        if (solveScheduleHelper(nextDay, nextSlot, avail, dailyNeed, maxShifts, sched, shiftsWorked, n, k)) {
            return true; 
        }

        // back track
        shiftsWorked[workerId]--;
    }

    // failed
    return false;
}

