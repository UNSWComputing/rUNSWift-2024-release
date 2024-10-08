#include "ThreadManager.hpp"
#include <execinfo.h>

class Blackboard;
class ThreadManager;

ConcurrentMap<pthread_t, jmp_buf*> jumpPoints;

using namespace std;

ThreadManager::ThreadManager(const char *name, int padMicroseconds) {
   this->name = name;
   this->padMicroseconds = padMicroseconds;
   running = false;
   // register thread name

}

void ThreadManager::join() {
   if (running) {
      pthread_join(pthread, NULL);
   }
   running = false;
}

ThreadManager::~ThreadManager() {
   join();
}


void overtimeAlert(int) {
   // Thread::name is thread local, and the signal handler always
   // happens in main thread, even if perception thread has frozen
   // TODO (jayen): use sigmask to have this happen in the thread itself, i think
   SAY(std::string(Thread::name) + " thread has frozen");
   system("aplay /opt/aldebaran/share/naoqi/wav/fall_jpj.wav");
   // TODO (jayen): use sigaction() to pass data to this function
   signal(SIGALRM, overtimeAlert);
   // TODO (jayen): see if using alarm() works and is simpler
   struct itimerval itval5;
   itval5.it_value.tv_sec = 5;
   itval5.it_value.tv_usec = 0;
   itval5.it_interval.tv_sec = 0;
   itval5.it_interval.tv_usec = 0;
   setitimer(ITIMER_REAL, &itval5, NULL);
}

/**
 * The signal handler. Handles the signal and flag that the thread has died
 * and allow the watcher thread to restart it.
 * @param sigNumber The POSIX signal identifier
 * @param info Signal info struct for the signal
 * @see registerSignalHandler
 */
void handleSignals(int sigNumber, siginfo_t* info, void*) {
   // End the rUNSWift module [CTRL-C]. Call all destructors
   if (sigNumber == SIGINT) {
      cerr << endl;
      cerr << "###########################" << endl;
      cerr << "##    SIGINT RECEIVED    ##" << endl;
      cerr << "##  ATTEMPTING SHUTDOWN  ##" << endl;
      cerr << "###########################" << endl;
      attemptingShutdown = true;
   } else if (sigNumber == SIGTERM) {
      cerr << endl;
      cerr << "###########################" << endl;
      cerr << "##   SIGTERM RECEIVED    ##" << endl;
      cerr << "##  ATTEMPTING SHUTDOWN  ##" << endl;
      cerr << "###########################" << endl;
      attemptingShutdown = true;
   } else {
      // re-register the signal handler
	  SAY("crash detected");
      registerSignalHandlers(sigNumber);
      pthread_t thread = pthread_self();

      cerr <<  string(Thread::name) << " with id " << thread <<
      " received signal " << sigNumber << " and is restarting" << endl;
      llog(ERROR) << string(Thread::name) << " with id "
                  << thread << " received signal "
                  << sigNumber << " and is restarting" << endl;
      // signal stacktrace
      void *array[10];
      size_t size;

      // get void*'s for all entries on the stack
      size = backtrace(array, 10);

      // print out all the frames to stderr
      backtrace_symbols_fd(array, size, STDERR_FILENO);

      longjmp(*jumpPoints[thread], 1);
   }
   return;
}


/**
 * @param signal default param is ALL_SIGNALS which is -1
 */
void registerSignalHandlers(int signal) {
   // setup the sigaction
   struct sigaction act;
   act.sa_sigaction = handleSignals;
   sigemptyset(&act.sa_mask);
   act.sa_flags = SA_SIGINFO | SA_RESETHAND;

   // register the signal handlers
   if (signal == SIGINT || signal == ALL_SIGNALS)
      sigaction(SIGINT, &act, NULL);   // CTRL-C termination
   if (signal == SIGTERM || signal == ALL_SIGNALS)
      sigaction(SIGTERM, &act, NULL);   // kill -15 termination
   if (signal == SIGSEGV || signal == ALL_SIGNALS)
      sigaction(SIGSEGV, &act, NULL);  // seg fault
   if (signal == SIGFPE || signal == ALL_SIGNALS)
      sigaction(SIGFPE, &act, NULL);   // floating point exception
   if (signal == SIGSTKFLT || signal == ALL_SIGNALS)
      sigaction(SIGSTKFLT, &act, NULL);   // stack faults
   if (signal == SIGHUP || signal == ALL_SIGNALS)
      sigaction(SIGHUP, &act, NULL);   // lost controlling terminal
}

