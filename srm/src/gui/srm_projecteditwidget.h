/*****************************************************************
 * $Id: srm_projecteditwidget.h 1831 2012-12-15 15:07:57Z rutger $
 * Created: Dec 14, 2012 8:34:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna SCAN project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef SRM_PROJECTEDITWIDGET_H
#define SRM_PROJECTEDITWIDGET_H

#include "rb_projecteditwidget.h"

class SRM_ProjectEditWidget : public RB_ProjectEditWidget {

    Q_OBJECT

public:
    SRM_ProjectEditWidget(QWidget* parent);
    virtual ~SRM_ProjectEditWidget();

    virtual RB_String getName() const;
    virtual RB2::PerspectiveType getPerspectiveType() const;
    virtual void init();

};

#endif // SRM_PROJECTEDITWIDGET_H
