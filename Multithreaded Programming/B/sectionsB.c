// Dai Nguyen
// sectionsB.c
// 02/09/2018
// PART B: Mutual Exclusion on Multithreading

/*  sectionsA.c: mutual exclusion model sections (Part b)

    Copyright (C) 2013  Eric Larson and Rochelle Palting
    elarson@seattleu.edu

    This file is part of the MDAT infrastructure.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include "sections.h"
#include "mdat.h"

// TODO: Declare shared variables here
pthread_mutex_t modifyLock;         // ensure mutual exclusion
pthread_mutex_t appendLock;         // ensure 1 append allow
pthread_mutex_t appendLockModify;   // ensure no append in modify
pthread_mutex_t searchLockModify;   // ensure no search in modify
pthread_mutex_t searchCountLock;    // allow mutiple search concurent

// semaphore use to prevent if there are more than 3 searches at the same time
sem_t waitSignal;

int searchCount = 0;                // allow mutiple search concurent

void
sectionInitGlobals()
{
  // TODO: Complete this function
  mdat_mutex_init("modifyLock", &modifyLock, NULL);
  mdat_mutex_init("appendLock", &appendLock, NULL);
  mdat_mutex_init("appendLockModify", &appendLockModify, NULL);
  mdat_mutex_init("searchLockModify", &searchLockModify, NULL);
  mdat_mutex_init("searchCountLock", &searchCountLock, NULL);
  
  mdat_sem_init("waitSignal", &waitSignal, 0, 3);
}

void
sectionEntrySection(enum AccessType accessType)
{
  // LEAVE THIS STATEMENT
  mdat_enter_section("Entry");

  // TODO: Complete this function
  if (accessType == SEARCH){
    // wait until if there are less than 3 searches
    mdat_sem_wait(&waitSignal);
    
    mdat_mutex_lock(&searchCountLock);
    searchCount++;
    
    if (searchCount == 1){
      mdat_mutex_lock(&searchLockModify);
    }
    mdat_mutex_unlock(&searchCountLock);
  
  } else if (accessType == APPEND){
      mdat_mutex_lock(&appendLock);
      mdat_mutex_lock(&appendLockModify);
  
  } else if (accessType == MODIFY){
    //order is really matter
    mdat_mutex_lock(&searchLockModify);
    mdat_mutex_lock(&appendLockModify);
    mdat_mutex_lock(&modifyLock);
  }
}

void
sectionExitSection(enum AccessType accessType)
{
  // LEAVE THIS STATEMENT
  mdat_enter_section("Exit");

  // TODO: Complete this function
  if (accessType == SEARCH){
    // if less than 3 searches, allow to call another search
    mdat_sem_post(&waitSignal);
    
    mdat_mutex_lock(&searchCountLock);
    searchCount--;
    
    if (searchCount == 0){
      mdat_mutex_unlock(&searchLockModify);
    }
    mdat_mutex_unlock(&searchCountLock);
    
  } else  if (accessType == APPEND){
      mdat_mutex_unlock(&appendLock);
      mdat_mutex_unlock(&appendLockModify);
    
  } else if(accessType == MODIFY){
    //order is really matter
    mdat_mutex_unlock(&searchLockModify);
    mdat_mutex_unlock(&appendLockModify);
    mdat_mutex_unlock(&modifyLock);
  }
}

// DO NOT TOUCH THIS FUNCTION

void
sectionRemainderSection(enum AccessType accessType)
{
  mdat_enter_section("Remainder");
}

// DO NOT TOUCH THIS FUNCTION

void
sectionCriticalSection(enum AccessType accessType)
{
  int x, y;

  mdat_enter_section("Critical");

  // The code here is garbage - just to force the threads to spend
  // a little time in the critical section.  This makes it easier
  // to catch mutual exclusion problems.
  x = 3;
  y = x * 7;
  x--;
  y++;
  y = y - 1;
}
