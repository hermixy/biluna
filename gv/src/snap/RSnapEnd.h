#ifndef RSNAPEND_H
#define RSNAPEND_H

#include "RSnapEntityBase.h"
#include "REntity.h"

class RMouseEvent;

/**
 * \brief End snapper implementation.
 *
 * \scriptable
 *
 * \ingroup snap
 */
class RSnapEnd : public RSnapEntityBase {
public:
    virtual ~RSnapEnd() {}

protected:
    virtual QList<RVector> snapEntity(QSharedPointer<REntity> entity, const RVector& point);
};

Q_DECLARE_METATYPE(RSnapEnd*)

#endif
