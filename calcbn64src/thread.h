// thread.h

#ifndef THREAD_H_
#define THREAD_H_

//---------------------------------------------------------------------------
// Thread
//---------------------------------------------------------------------------

enum
{
  stNone, stInit, stRun, stSleep, stReady,
  LimitOfThreads = 32,
  DefaultSleepTime = 10000
};

class ThreadContainer;

class Thread
{
private:
  ThreadContainer *Owner;
  pthread_t ID;
  int State;
  int Sel;
  void* Data;

public:
  Thread( ThreadContainer *owner, int sel, void *data );
  ~Thread();
  bool IsReady();

  friend void* ThreadWrapper( void* arg );
};

//---------------------------------------------------------------------------
// ThreadContainer
//---------------------------------------------------------------------------

class ThreadContainer
{
private:
  int numThreads;
  int maxThreads;
  Thread **threads;
  pthread_mutex_t mutex;
  pthread_mutexattr_t mutex_attr;

protected:
  virtual void ThreadFunc( int sel, void* data ) = 0;

public:
  ThreadContainer();
  ~ThreadContainer();

  // thread handling
  bool AddThread( int sel, void* data = NULL );
  bool IsReady();
  bool WaitFor( long sleeptime = DefaultSleepTime, long waittime = -1 );

  // access handling
  void WaitAccess();
  void ReleaseAccess();

  friend void* ThreadWrapper( void* arg );
};

#endif
