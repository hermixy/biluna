#include "RPropertyChange.h"

QDebug operator<<(QDebug dbg, RPropertyChange& p) {
    dbg.nospace()
        << "RPropertyChange(" << p.oldValue << ", " << p.newValue << ")";
    return dbg.space();
}
