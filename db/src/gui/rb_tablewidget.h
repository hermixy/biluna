/*****************************************************************
 * $Id: rb_tablewidget.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Jan 31, 2016 8:34:42 PM - rutger
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/
#ifndef RB_TABLEWIDGET_H
#define RB_TABLEWIDGET_H

#include "rb_mmproxy.h"
#include "rb_mainwindow.h"
#include "rb_widget.h"
#include "ui_rb_tablewidget.h"


/**
 * Base widget using a model for the editing or selection from a table
 */
class DB_EXPORT RB_TableWidget : public RB_Widget, protected Ui::RB_TableWidget {

    Q_OBJECT

public:
    RB_TableWidget(QWidget* parent);
    virtual ~RB_TableWidget();

    virtual void init() = 0;
    virtual bool fileSave(bool withSelect);
    virtual void fileRevert();

    virtual void setHelpSubject(const RB_String& subject);

    virtual RB_ObjectBase* getCurrentObject();
    virtual void setCurrentObject();

protected:
    virtual void initEditSort(bool isFindFilter = true);
    virtual void initEditUpDown();
    virtual void initEditOnly();
    virtual void initSelectionOnly();
    virtual void initTest();

    void changeEvent(QEvent* e);

    RB_MmProxy* mModel;
    RB_String mHelpSubject;

    QModelIndex mCurrentModelIndex;
    RB_ObjectBase* mCurrentObject;

};

#endif // rB_TABLEWIDGET_H
