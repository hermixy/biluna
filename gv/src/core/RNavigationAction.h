#ifndef RNAVIGATIONACTION_H
#define RNAVIGATIONACTION_H

#include "RActionAdapter.h"
#include "RMouseEvent.h"
#include "RGraphicsView.h"
#include "RGuiAction.h"

/**
 * \ingroup core
 * \scriptable
 */
class RNavigationAction : public RActionAdapter {
public:
    RNavigationAction(RGuiAction* guiAction=NULL);

    virtual void mousePressEvent(RMouseEvent& event);
    virtual void mouseReleaseEvent(RMouseEvent& event);
    virtual void mouseMoveEvent(RMouseEvent& event);

private:
    bool panning;
    RVector panOrigin;
};

Q_DECLARE_METATYPE(RNavigationAction*)

#endif
