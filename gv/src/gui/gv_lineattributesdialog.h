/*****************************************************************
 * $Id: gv_lineattributesdialog.h 1337 2010-12-27 23:20:25Z rutger $
 * Created: June 14, 2010 11:00:51 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef GV_LINEATTRIBUTESDIALOG_H
#define GV_LINEATTRIBUTESDIALOG_H

#include <QPen>
#include "rb_dialog.h"
#include "ui_gv_lineattributesdialog.h"

class GV_LineAttributesDialog : public RB_Dialog, private Ui::GV_LineAttributesDialog {

    Q_OBJECT

public:
    GV_LineAttributesDialog(QWidget *parent = 0);
    virtual ~GV_LineAttributesDialog();

    virtual RB_String getId() const { return RB_String(); }
    virtual RB_String getName() const { return "Line Attributes"; }
    virtual RB2::PerspectiveType getPerspectiveType() const { return RB2::PerspectiveGV; }

    virtual void init();

    virtual void setPen(const QPen& pen);
    virtual QPen getPen();

protected:
    void changeEvent(QEvent* e);

private:
    QPen mPen;

};

#endif // GV_LINEATTRIBUTESDIALOG_H
