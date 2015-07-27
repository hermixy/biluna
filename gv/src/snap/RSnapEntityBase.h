#ifndef RSNAPENTITYBASE_H
#define RSNAPENTITYBASE_H

#include "RSnap.h"

class RGraphicsView;
class RMouseEvent;

/**
 * \brief EntityBase snapper implementation.
 *
 * \scriptable
 *
 * \ingroup snap
 */
class RSnapEntityBase : public RSnap {
public:
    virtual ~RSnapEntityBase() {}

    virtual RVector snap(const RVector& position, RGraphicsView& view);

protected:
    virtual QList<RVector> snapEntity(QSharedPointer<REntity> entity, const RVector& point) = 0;
};

Q_DECLARE_METATYPE(RSnapEntityBase*)

#endif
