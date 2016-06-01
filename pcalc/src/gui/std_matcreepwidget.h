/*****************************************************************
 * $Id: std_matcreepwidget.h 0 2016-06-01 10:23:20Z rutger $
 * Created: June 1, 2016 10:23:42 AM - rutger
 *
 * Copyright (C) 2015 Red-Bag. All rights reserved.
 * This file is part of the Biluna PCALC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef STD_MATCREEPWIDGET_H
#define STD_MATCREEPWIDGET_H

#include "rb_widget.h"

namespace Ui {
class STD_MatCreepWidget;
}

class STD_MatCreepWidget : public RB_Widget {

    Q_OBJECT

public:
    explicit STD_MatCreepWidget(QWidget *parent = 0);
    virtual ~STD_MatCreepWidget();

    RB_String getName() const;
    RB2::PerspectiveType getPerspectiveType() const;
    void init();

    bool fileSave(bool withSelect);
    void fileRevert();

private:
    Ui::STD_MatCreepWidget* ui;
    RB_MmProxy* mModel;
    RB_MmProxy* mRp10T100khModel;
    RB_MmProxy* mRp10T10khModel;
    RB_MmProxy* mRpT100khModel;
    RB_MmProxy* mRpT10khModel;
    RB_MmProxy* mRpT200khModel;
};

#endif // STD_MATCREEPWIDGET_H
