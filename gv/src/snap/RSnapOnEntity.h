#ifndef RSNAPONENTITY_H
#define RSNAPONENTITY_H

#include "RSnapEntityBase.h"

class RMouseEvent;

/**
 * \brief On entity snapper implementation.
 *
 * \scriptable
 *
 * \ingroup snap
 */
class RSnapOnEntity : public RSnapEntityBase {
public:
    virtual ~RSnapOnEntity() {}

protected:
    virtual QList<RVector> snapEntity(QSharedPointer<REntity> entity, const RVector& point);
};

Q_DECLARE_METATYPE(RSnapOnEntity*)

#endif
