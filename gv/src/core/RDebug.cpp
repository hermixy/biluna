//#include <execinfo.h>
#include <stdlib.h>

extern "C" {
#include "backtrace.h"
}

#include "RDebug.h"

FILE* RDebug::stream=stderr;

#ifdef Q_OS_MAC
QMap<int, uint64_t> RDebug::timerMac;
#else
QMap<int, QTime> RDebug::timer;
#endif
/*
void RDebug::printBacktrace() {
    void *array[10];
    size_t size;
    char **strings;
    size_t i;

    size = backtrace(array, 10);
#ifdef RBACKTRACE
    strings = backtrace_symbols_2(array, size);
#else
    strings = backtrace_symbols(array, size);
#endif

    debug("Obtained %zd stack frames.\n", size);

    for(i = 0; i < size; i++) {
        debug("%s\n", strings[i]);
    }

    free(strings);
}
*/
void RDebug::startTimer(int id) {
    //timer[id].start();
#ifdef Q_OS_MAC
    //timerMac[id] = mach_absolute_time();
#endif
}


int RDebug::stopTimer(int id, const char* format ...) {
#ifdef Q_OS_MAC
//    Nanoseconds elapsedNano;
//    uint64_t end = mach_absolute_time();
//    uint64_t elapsed = end - timerMac[id];
//    elapsedNano = AbsoluteToNanoseconds( *(AbsoluteTime *) &elapsed );
//    int t = (unsigned int)((* (uint64_t *) &elapsedNano) / 1000000);
//    timerMac.remove(id);
#else
    int t = timer[id].elapsed();
    timer.remove(id);
#endif
//    va_list varg;
//    va_start(varg, format);
//    fprintf(stream, "TIMER: %d ms ", t);
//    printV(format, varg);
//    va_end(varg);
//    return t;
    return 0;
}
