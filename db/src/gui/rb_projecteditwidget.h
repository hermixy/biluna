/*****************************************************************
 * $Id: rb_projecteditwidget.h 1965 2013-08-08 16:33:15Z rutger $
 * Created: Dec 14, 2012 8:34:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef RB_PROJECTEDITWIDGET_H
#define RB_PROJECTEDITWIDGET_H

#include "rb_widget.h"

namespace Ui {
class RB_ProjectEditWidget;
}

/**
 * @brief Base class to edit the root object of each plugin including DB
 */
class DB_EXPORT RB_ProjectEditWidget : public RB_Widget {

    Q_OBJECT

public:
    explicit RB_ProjectEditWidget(QWidget* parent = 0);
    virtual ~RB_ProjectEditWidget();

    virtual RB_String getName() const = 0;
    virtual RB2::PerspectiveType getPerspectiveType() const = 0;
    virtual void init() = 0;

    virtual bool fileSave(bool withSelect);
    virtual void fileRevert();

    virtual void setHelpSubject(const RB_String& subject);

    virtual RB_ObjectBase* getCurrentObject();
    virtual void setCurrentObject();

public slots:
    virtual void pbAddclicked();

protected:
    Ui::RB_ProjectEditWidget* ui;

    RB_String mHelpSubject;

    RB_MmProxy* mModel;
    RB_DataWidgetMapper* mMapper;
    QModelIndex mCurrentModelIndex;
    RB_ObjectBase* mCurrentObject;
};

#endif // RB_PROJECTEDITWIDGET_H
