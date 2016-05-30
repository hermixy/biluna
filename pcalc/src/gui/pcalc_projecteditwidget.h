/*****************************************************************
 * $Id: peng_projecteditwidget.h 1831 2012-12-15 15:07:57Z rutger $
 * Created: Apr 29, 2015 8:34:42 PM - rutger
 *
 * Copyright (C) 2015 Red-Bag. All rights reserved.
 * This file is part of the Biluna PCALC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/
#ifndef PCALC_PROJECTEDITWIDGET_H
#define PCALC_PROJECTEDITWIDGET_H

#include "rb_projecteditwidget.h"


/**
 * Edit DB project dialog
 */
class PCALC_ProjectEditWidget : public RB_ProjectEditWidget {

    Q_OBJECT

public:
    PCALC_ProjectEditWidget(QWidget* parent);
    virtual ~PCALC_ProjectEditWidget();

    virtual RB_String getName() const;
    virtual RB2::PerspectiveType getPerspectiveType() const;
    virtual void init();

};

#endif // PCALC_PROJECTEDITWIDGET_H
