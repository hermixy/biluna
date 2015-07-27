#ifndef RSNAPCENTER_H
#define RSNAPCENTER_H

#include "RSnapEntityBase.h"

class RMouseEvent;

/**
 * \brief Center snapper implementation.
 *
 * \scriptable
 *
 * \ingroup snap
 */
class RSnapCenter : public RSnapEntityBase {
public:
    virtual ~RSnapCenter() {}

protected:
    virtual QList<RVector> snapEntity(QSharedPointer<REntity> entity, const RVector& point);
};

Q_DECLARE_METATYPE(RSnapCenter*)

#endif
