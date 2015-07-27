#ifndef RSNAPREFERENCE_H
#define RSNAPREFERENCE_H

#include "RSnapEntityBase.h"
#include "REntity.h"

class RMouseEvent;

/**
 * \brief Reference snapper implementation.
 *
 * \scriptable
 *
 * \ingroup snap
 */
class RSnapReference : public RSnapEntityBase {
public:
    virtual ~RSnapReference() {}

protected:
    virtual QList<RVector> snapEntity(QSharedPointer<REntity> entity, const RVector& point);
};

Q_DECLARE_METATYPE(RSnapReference*)

#endif
