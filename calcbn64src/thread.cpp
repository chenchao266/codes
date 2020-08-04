//============================================================================
// Name        : thread.cpp
// Author      : Bernd C. Kellner
// Version     : 1.0
// Copyright   : (C) 2008 Bernd C. Kellner (bk at bernoulli dot org)
// Description : implements a simple thread container
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by the Free
// Software Foundation; either version 3 of the License, or (at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
// more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, see <http://www.gnu.org/licenses/>.
//============================================================================


#include <unistd.h>
#include <pthread.h>

#include "thread.h"

//---------------------------------------------------------------------------
// Thread
//---------------------------------------------------------------------------

void* ThreadWrapper( void* arg )
{
  Thread *Thr = (Thread*) arg;
  Thr->State = stRun;
  Thr->Owner->ThreadFunc( Thr->Sel, Thr->Data );
  Thr->State = stReady;
  return NULL;
}

Thread::Thread( ThreadContainer *owner, int sel, void *data )
{
  Owner = owner;
  Sel = sel;
  Data = data;
  ID = 0;
  State = stInit;
  pthread_create( &ID, NULL, ThreadWrapper, this );
}

Thread::~Thread()
{
  if ( State != stReady )
    pthread_cancel( ID );

  pthread_join( ID, NULL );
}

bool Thread::IsReady()
{
  return State == stReady;
}

//---------------------------------------------------------------------------
// ThreadContainer
//---------------------------------------------------------------------------

ThreadContainer::ThreadContainer()
{
  numThreads = 0;
  maxThreads = LimitOfThreads;
  threads = new Thread*[ maxThreads ];

  pthread_mutexattr_init( &mutex_attr );
  pthread_mutexattr_settype( &mutex_attr, PTHREAD_MUTEX_RECURSIVE_NP );
  pthread_mutex_init( &mutex, &mutex_attr );
}

ThreadContainer::~ThreadContainer()
{
  for ( int i=0; i < numThreads; i++ )
    delete threads[i];
  delete[] threads;
  pthread_mutex_destroy( &mutex );
}

void ThreadContainer::WaitAccess()
{
  pthread_mutex_lock( &mutex );
}

void ThreadContainer::ReleaseAccess()
{
  pthread_mutex_unlock( &mutex );
}

bool ThreadContainer::AddThread( int sel, void *data )
{
  if ( numThreads >= maxThreads )
    return false;

  threads[numThreads++] = new Thread( this, sel, data );
  return true;
}

bool ThreadContainer::IsReady()
{
  for ( int i=0; i < numThreads; i++ )
    if ( !threads[i]->IsReady() )
      return false;
  return true;
}

bool ThreadContainer::WaitFor( long sleeptime, long waittime )
{
  if ( waittime < 0 )
  {
    while ( !IsReady() )
      usleep( sleeptime );

    return true;
  }

  while ( waittime > 0 && !IsReady() )
  {
    usleep( sleeptime );
    waittime -= sleeptime;
  }

  return IsReady();
}

