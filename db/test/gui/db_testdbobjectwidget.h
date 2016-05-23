/*****************************************************************
 * $Id: db_testdbobjectwidget.h 1356 2011-01-28 20:29:45Z rutger $
 * Created: Dec 15, 2009 9:08:25 PM - Rutger Botermans
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_TESTDBOBJECTWIDGET_H
#define DB_TESTDBOBJECTWIDGET_H

#include "rb.h"
#include "rb_widget.h"
#include "ui_db_testdbobjectwidget.h"

class RB_MmObjectInterface;
class RB_ObjectBase;

/**
 * Complex test widget class, with all model view widgets
 */
class DB_TestDbObjectWidget : public RB_Widget, private Ui::DB_TestDbObjectWidget {

    Q_OBJECT

public:
    DB_TestDbObjectWidget(QWidget* parent = 0);
    virtual ~DB_TestDbObjectWidget();

    virtual RB_String getId() const { return RB_String(); }
    virtual RB_String getName() const { return "Test DB Object"; }
    virtual RB2::PerspectiveType getPerspectiveType() const { return RB2::PerspectiveTEST; }

    virtual void init();
    virtual bool closeWidget() { return true; }

public slots:
    // Push buttons
    virtual void on_pbRead_clicked();
    virtual void on_pbUpdate_clicked();
    virtual void on_pbDelete_clicked();

protected:
    void changeEvent(QEvent* e);

private:
    RB_MmProxy* mModel;
    RB_MmProxy* mChildModel;

    RB_ObjectBase* mObject;
};

#endif // DB_TESTDBOBJECT_H
