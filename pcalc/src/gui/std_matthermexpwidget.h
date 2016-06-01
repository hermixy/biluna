/*****************************************************************
 * $Id: std_matthermexpwidget.cpp 0 2016-06-01 08:23:20Z rutger $
 * Created: June 1, 2016 8:23:42 AM - rutger
 *
 * Copyright (C) 2015 Red-Bag. All rights reserved.
 * This file is part of the Biluna PCALC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef STD_MATTHERMEXPWIDGET_H
#define STD_MATTHERMEXPWIDGET_H

#include "rb_widget.h"

namespace Ui {
class STD_MatThermExpWidget;
}

class STD_MatThermExpWidget : public RB_Widget {

    Q_OBJECT

public:
    explicit STD_MatThermExpWidget(QWidget *parent = 0);
    virtual ~STD_MatThermExpWidget();

    RB_String getName() const;
    RB2::PerspectiveType getPerspectiveType() const;
    void init();

    bool fileSave(bool withSelect);
    void fileRevert();

private:
    Ui::STD_MatThermExpWidget* ui;
    RB_MmProxy* mModel;
    RB_MmProxy* mChildModel;
};

#endif // STD_MATTHERMEXPWIDGET_H
