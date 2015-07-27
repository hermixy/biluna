/*****************************************************************
 * $Id: gv_onetimedialog.h 1309 2010-10-18 21:58:58Z rutger $
 * Created: Feb 4, 2009 9:55:58 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef GV_ONETIMEDIALOG_H
#define GV_ONETIMEDIALOG_H

#include "rb_dialog.h"
#include "rb_string.h"

#include "ui_gv_onetimedialog.h"

/**
 * Warning dialog that offers the user a check box to never show 
 * the warning again.
 */
class GV_OneTimeDialog : public RB_Dialog, private Ui::GV_OneTimeDialog {

	Q_OBJECT

public:
    GV_OneTimeDialog(QWidget* parent = 0);
    virtual ~GV_OneTimeDialog();

    bool getDontShowAgain();

public slots:
    void setText(const RB_String& text);

};

#endif

