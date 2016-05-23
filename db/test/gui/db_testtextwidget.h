/*****************************************************************
 * $Id: db_testtextwidget.h 1356 2011-01-28 20:29:45Z rutger $
 * Created: Nov 5, 2009 9:08:25 PM - Rutger Botermans
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_TESTTEXTWIDGET_H
#define DB_TESTTEXTWIDGET_H

#include "rb_string.h"
#include "rb_widget.h"
#include "ui_db_testtextwidget.h"



class DB_TestTextWidget : public RB_Widget, private Ui::DB_TestTextWidget {

    Q_OBJECT

public:
    DB_TestTextWidget(QWidget* parent = 0);

    virtual RB_String getId() const { return RB_String(); }
    virtual RB_String getName() const { return "Test Text"; }
    virtual RB2::PerspectiveType getPerspectiveType() const { return RB2::PerspectiveTEST; }

    virtual void init(int, int, int) {
        // catch close() signal from MDI window,
        // not in constructor because parent is not yet set
        RB_Widget::init();
    }

    virtual void appendText(const RB_String& str);
    virtual bool closeWidget() { return true; }

public slots:
    virtual void slotClearText();
    virtual void slotTextIsChanged();

protected:
    void changeEvent(QEvent* e);
};

#endif // DB_TESTTEXTWIDGET_H
