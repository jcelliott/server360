/**
 * Logger
 * @Author Joshua Elliott
 * Provides simple logging features for application development
 * Current Version 0.6.2
 *
 * Version History:
 * 0.6.2 - Added a Low Level Debugging priority
 * 0.6.1 - Fixed a bug that was not displaying messages for the correct 
 *			    priority level.
 * 0.6.0 - Changed singleton model to lazy initialization (declaring a static
 *			    instance variable in the GetLog() method). This method guarantees
 *			    that the instance will be destroyed correctly. Discovered that
 *			    writing to file does not work when multiple classes are using a
 *			    Logger instance at the same time. Disabled for now - Will look into.
 * 0.5.4 - Inlined all method definitions to prevent multiple definition errors
 * 0.5.3 - Changed instantiation methods to use bool instanceLock variable to
 *			    enforce the singleton design. Checking for NULL was not reliable.
 * 0.5.2 - Added 'endl' modifier to add a newline. LogMessage() function no
 *			    longer auto-adds a newline after log (messed up formatting when 
 *			    using <<). Basic <logtype>(string) functions now add newline.
 * 0.5.1 - Modified TEST macro to use functionality from v0.4 and v0.5
 * 0.5.0 - Added basic manipulators to define priority levels that can be 
 *			    cascaded in a sequence of (<<) (eg. log << error << "This is an
 *			    error!"). This took forever to get working.
 * 0.4.0 - Added overloaded insertion operator (<<) functions for writing more 
 *			    complicated log output. Added a currentPriority variable to use
 *			    with this method.
 * 0.3.0 - Added TEST macro (end of this file) for unit testing
 * 0.2.0 - Consolidated implementation code from Logger.cpp into this file
 * 0.1.0 - 3/3/12 - Basic Functionality
 *
 * TODO: 
 *		- clean up the mess from using namespace std vs explicit std:: notation
 *		- write to any specified stream (ie. set output based on constructor
 *			and write to that output everywhere instead of clumsy if statement
 *		- use enum for priority levels instead of int
 *		- fix writing to file
 *		- put file and line number in log messages
 *    - add timestamp to log messages
 *		- LogMessage() doesn't need to be public anymore
 */

#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

using namespace std;

class Logger {
public:

  // Lazy initialization. Returns the static instance of the logger
  static Logger & GetLog() {
    static Logger instance;
    return instance;
  }

  // sets the minimum priority of messages to be logged
  void SetMinPriority(int _minPriority) {
    minPriority = _minPriority;
  }

  int GetMinPriority() {
    return minPriority;
  }

  // ignores all messages until Unmute()
  void Mute() {
    saveMinPriority = minPriority;
    saveCurPriority = currentPriority;
    minPriority = IGNORE;
  }

  // restores logger to state before Mute()
  void Unmute() {
    if(minPriority == IGNORE) {
      minPriority = saveMinPriority;
      currentPriority = saveCurPriority;
    }
  }

  // Logs a low level debug message (lowest priority)
  void DebugLow(const string & _msg) {
    string msg = "DEBUG_LOW: ";
    this->SetPriority(DEBUGLOW);
    LogMessage(msg += _msg + "\n");
  }

  // Logs a debug message (almost lowest priority)
  void Debug(const string & _msg) {
    string msg = "DEBUG: ";
    this->SetPriority(DEBUG);
    LogMessage(msg += _msg + "\n");
  }

  // Logs an informational message
  void Info(const string & _msg) {
    string msg = "INFO: ";
    this->SetPriority(INFO);
    LogMessage(msg += _msg + "\n");
  }

  // Logs a warning message
  void Warning(const string & _msg) {
    string msg = "WARNING: ";
    this->SetPriority(WARNING);
    LogMessage(msg += _msg + "\n");
  }

  // Logs an error message (highest priority)
  void Error(const string & _msg) {
    string msg = "ERROR: ";
    this->SetPriority(ERROR);
    LogMessage(msg += _msg + "\n");
  }

  // Sets log priority to DEBUGLOW. To be used with <<
  static Logger & debuglow(Logger & log) {
    log.SetPriority(DEBUGLOW);
    log.LogMessage("DEBUG_LOW: ");
    return log;
  }

  // Sets log priority to DEBUG. To be used with <<
  static Logger & debug(Logger & log) {
    log.SetPriority(DEBUG);
    log.LogMessage("DEBUG: ");
    return log;
  }

  // Sets log priority to INFO. To be used with <<
  static Logger & info(Logger & log) {
    log.SetPriority(INFO);
    log.LogMessage("INFO: ");
    return log;
  }

  // Sets log priority to WARNING. To be used with <<
  static Logger & warning(Logger & log) {
    log.SetPriority(WARNING);
    log.LogMessage("WARNING: ");
    return log;
  }

  // Sets log priority to ERROR. To be used with <<
  static Logger & error(Logger & log) {
    log.SetPriority(ERROR);
    log.LogMessage("ERROR: ");
    return log;
  }

  // Inserts a newline into log. To be used with <<
  static Logger & endl(Logger & log) {
    log.LogMessage("\n");
    return log;
  }

  // this lets us use the manipulator functions (eg. log << error)
  Logger & operator <<(Logger& (*pf)(Logger&)) {
    (pf) (*this);
    return (*this);
  }

  // Insertion operator for strings
  Logger & operator <<(std::string str) {
    LogMessage(str);
    return *this;
  }

  // Insertion operator for cstrings
  Logger & operator <<(const char* str) {
    LogMessage(std::string(str));
    return *this;
  }

  // Insertion operator for int
  Logger & operator <<(int str) {
    stringstream out;
    out << str;
    LogMessage(out.str());
    return *this;
  }

  // Internal method to log messages. It's public so that the << functions can use it
  void LogMessage(const string & msg) {

    if (currentPriority >= minPriority) {

      if (outputToFile) {
        logfile.open(filename.c_str());
        if (logfile.is_open()) {
          logfile << msg;
          //logfile.close();
        } else {
          cout << "Error opening logfile" << endl;
        }
      } else {
        cout << msg;
      }
    }
  }

  // sets the priority of the next message to be logged
  void SetPriority(int newPriority) {
    currentPriority = newPriority;
  }

  // returns the current priority
  int GetCurPriority() {
    return currentPriority;
  }

  // define priority levels of log messages
  static const int DEBUGLOW = 0;
  static const int DEBUG = 1;
  static const int INFO = 2;
  static const int WARNING = 3;
  static const int ERROR = 4;
  static const int IGNORE = 9;


private:

  // Constructors and assignment operator are private to prevent more than 
  // one instance being created (singleton)
  Logger() {
    minPriority = DEBUG;
    currentPriority = DEBUG;
    outputToFile = false;
    saveMinPriority = DEBUG;
  }

  Logger(string _filename) {
    minPriority = DEBUG;
    currentPriority = DEBUG;
    outputToFile = true;
    saveMinPriority = DEBUG;
    filename = _filename;
    /*************
      outputToFile = false;
    //logfile = new ofstream(filename.c_str());
    logfile.open(filename);
    if(!logfile.is_open()) {
    //instance->Warning("Could not open log file. Logging to std out.");
    }
    else {
    outputToFile = true;
    }
     ************/
  }

  Logger(Logger const&) {
  };

  void operator=(Logger const&) {
  };

  // destructor is private because instance should only die from Finalize()
  ~Logger() {
    if (logfile != NULL && logfile.is_open()) {
      logfile.close();
      // we don't need to delete logfile here because we didn't call new
    }
  }


  // stores the single instantiated Logger object
  //static Logger* instance;

  // used to check if an object has been instantiated
  //static bool instanceLock;

  // priority of the next message to be logged
  int currentPriority;

  // output file to log messages to (if not using std out)
  ofstream logfile;

  string filename;

  // whether logs will be written to a file or to std out
  bool outputToFile;

  // minimum priority of messages to be logged
  int minPriority;

  // saves the minPriority so we can restore it after Mute()
  int saveMinPriority;

  int saveCurPriority;




}; //end Logger class


#endif	// end LOGGER_H







// Define TEST macro. Every class that needs it will include Logger.h anyway.

#ifndef TEST_H
#define TEST_H


#define TEST(cond) if (!(cond)) { success = false; \
  log << Logger::error << "Test Failed [" << __FILE__ << ", " << __LINE__ \
  << "]" << Logger::endl; }

#endif

/*
   ostringstream os; \
   os << "Test Failed [" << __FILE__ << ", " << __LINE__ << "]"; \
   log.Error(os.str()); }
   */



/****************** OLD ***
// overloaded insertion operator functions:

// Insertion operator for strings
static Logger & operator <<(Logger & log, std::string str) {
log.LogMessage(str, log.GetCurPriority());
return log;
}

// Insertion operator for cstrings
static Logger & operator <<(Logger & log, const char* str) {
log.LogMessage(std::string(str), log.GetCurPriority());
return log;
}

// Insertion operator for int
static Logger & operator <<(Logger & log, int str) {
stringstream out;
out << str;
log.LogMessage(out.str(), log.GetCurPriority());
return log;
}
 **********************/

