#ifndef RSNAPMIDDLE_H
#define RSNAPMIDDLE_H

#include "RSnapEntityBase.h"

class RMouseEvent;

/**
 * \brief Middle snapper implementation.
 *
 * \scriptable
 *
 * \ingroup snap
 */
class RSnapMiddle : public RSnapEntityBase {
public:
    virtual ~RSnapMiddle() {}

protected:
    virtual QList<RVector> snapEntity(QSharedPointer<REntity> entity, const RVector& point);
};

Q_DECLARE_METATYPE(RSnapMiddle*)

#endif
