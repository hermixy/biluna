#ifndef RDEBUG_H
#define RDEBUG_H

#include <fstream>
#include <ostream>
#include <stdarg.h>
#include <stdio.h>

#include <QString>
#include <QTime>
#include <QMap>
#include <QFile>
#include <QMetaType>

#ifdef Q_OS_MAC
#include <CoreServices/CoreServices.h>
#include <mach/mach.h>
#include <mach/mach_time.h>
#undef check
#endif


/**
 * This class provides some static methods for printing debugging information to
 * stderr. The methods accept a format string and parameters in the same way
 * as the printf functions from standard C.
 *
 * Messages can be logged with different priorities.
 *
 * While there is no restriction as to what is logged, it is a good idea to
 * log the class name and function name first.
 *
 * \code
 * // output for development:
 * RDebug::debug("RMyClass::myFunction: i = %d", i);

 * // information about progress, state, etc:
 * RDebug::info("RMyClass::myFunction: starting optimization...");

 * // warnings ("this should not happen under normal cicumstances"):
 * RDebug::warning("RMyClass::myFunction: cannot open file '%s'", fileName);
 *
 * // indication for a bug in the application:
 * RDebug::error("RMyClass::myFunction: parent is NULL. Aborting.");
 * \endcode
 *
 * \ingroup core
 * \scriptable
 */

class RDebug {
public:
    static void init(const QString& filename = ""){
        if(!filename.isEmpty()){
            stream = fopen((const char*)filename.toUtf8(), "w");

        } else{
            stream = stderr;
        }
    }

    static void uninit(){
        if(stream!=stderr){
            fclose(stream);
        }
    }

//    static void printBacktrace();

	/**
	 * Prints development related output for debugging.
	 * \nonscriptable
	 */
	static void debug(const char* format...) {
		va_list varg;
		va_start(varg, format);
        fprintf(stream, "DEBUG: ");
		printV(format, varg);
		va_end(varg);
	}

	static void debug(const QString& str) {
		debug((const char*)str.toUtf8());
	}


	/**
	 * Prints information about progress, state, etc.
	 * \nonscriptable
	 */
	static void info(const char* format ...) {
		va_list varg;
		va_start(varg, format);
                fprintf(stream, "INFO: ");
		printV(format, varg);
		va_end(varg);
	}

	static void info(const QString& str) {
		info((const char*)str.toUtf8());
	}

	/**
	 * Prints a warning about something that should not happen normally
	 * or was not anticipated to happen. For example file IO errors.
	 * \nonscriptable
	 */
	static void warning(const char* format ...) {
		va_list varg;
		va_start(varg, format);
                fprintf(stream, "WARNING: ");
		printV(format, varg);
		va_end(varg);
	}

	static void warning(const QString& str) {
		warning((const char*)str.toUtf8());
	}

	/**
	 * Prints an error message. This should be used for things that
	 * indicate a bug in the code base, for example code segments
	 * that should never be reached.
	 * \nonscriptable
	 */
	static void error(const char* format ...) {
		va_list varg;
		va_start(varg, format);
        fprintf(stream, "ERROR: ");
		printV(format, varg);
		va_end(varg);
	}

	/**
	 * Prints an error message. This should be used for things that
	 * indicate a bug in the code base, for example code segments
	 * that should never be reached.
	 *
	 * \param str the error message
	 */
	static void error(const QString& str) {
		error((const char*)str.toUtf8());
	}

    /**
     * Starts a timer with the given ID.
     *
     * \nonscriptable
     */
	static void startTimer(int id=0);

    /**
     * Stops the timer with the given ID and prints the given message together 
     * with the time that has elapsed.
     *
     * \nonscriptable
     */
	static int stopTimer(int id, const char* format ...);

    /**
     * Stops the timer with ID 0 and prints the given message together 
     * with the time that has elapsed.
     *
     * \nonscriptable
     */
	static int stopTimer(const char* format ...) {
		va_list varg;
		va_start(varg, format);
        return stopTimer(0, format, varg);
		va_end(varg);
    }

protected:
    static void printV(const char* format, va_list varg) {
        vfprintf(stream, format, varg);
        fprintf(stream, "\n");
        fflush(stream);
        va_end(varg);
    }

private:
    static FILE* stream;
#ifdef Q_OS_MAC
    static QMap<int, uint64_t> timerMac;
#else
    static QMap<int, QTime> timer;
#endif
};

Q_DECLARE_METATYPE(RDebug*)

#endif
