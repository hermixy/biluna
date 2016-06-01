/*****************************************************************
 * $Id: peng_projecteditwidget.h 1831 2012-12-15 15:07:57Z rutger $
 * Created: Dec 14, 2012 8:34:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef PENG_PROJECTEDITWIDGET_H
#define PENG_PROJECTEDITWIDGET_H

#include "rb_projecteditwidget.h"

class PENG_ProjectEditWidget : public RB_ProjectEditWidget {

    Q_OBJECT

public:
    PENG_ProjectEditWidget(QWidget* parent);
    virtual ~PENG_ProjectEditWidget();

    virtual RB_String getName() const;
    virtual RB2::PerspectiveType getPerspectiveType() const;
    virtual void init();

};

#endif // PENG_PROJECTEDITWIDGET_H
