/*****************************************************************
 * $Id: db_spreadsheetwidget.h 2248 2016-08-09 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_SPREADSHEETWIDGET_H
#define DB_SPREADSHEETWIDGET_H

#include <QWidget>
#include "db_global.h"
#include "rb_widget.h"

namespace Ui {
class DB_SpreadSheetWidget;
}

class DB_EXPORT DB_SpreadSheetWidget : public RB_Widget {

    Q_OBJECT

public:
    explicit DB_SpreadSheetWidget(QWidget* parent = 0);
    ~DB_SpreadSheetWidget();

    virtual RB_String getName() const;
    virtual RB2::PerspectiveType getPerspectiveType() const {
        return RB2::PerspectiveDB;
    }

    virtual void init();

private:
    Ui::DB_SpreadSheetWidget* ui;
};

#endif // DB_SPREADSHEETWIDGET_H
