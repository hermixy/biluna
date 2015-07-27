#ifndef RSNAPDISTANCE_H
#define RSNAPDISTANCE_H

#include "REntity.h"
#include "RSnapEntityBase.h"

class RMouseEvent;

/**
 * \brief Distance snapper implementation.
 *
 * \scriptable
 * \generateScriptShell
 *
 * \ingroup snap
 */
class RSnapDistance : public RSnapEntityBase {
public:
    RSnapDistance();
    virtual ~RSnapDistance() {}

    virtual void showUiOptions();
    virtual void hideUiOptions();
    virtual void setDistance(double d);

protected:
    virtual QList<RVector> snapEntity(QSharedPointer<REntity> entity, const RVector& point);

private:
    double distance;
};

Q_DECLARE_METATYPE(RSnapDistance*)

#endif
