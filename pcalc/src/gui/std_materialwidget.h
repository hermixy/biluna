/*****************************************************************
 * $Id: std_materialwidget.h 0 2016-06-01 15:23:20Z rutger $
 * Created: June 1, 2016 15:23:42 AM - rutger
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna PCALC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef STD_MATERIALWIDGET_H
#define STD_MATERIALWIDGET_H

#include "rb_widget.h"

namespace Ui {
class STD_MaterialWidget;
}

class STD_MaterialWidget : public RB_Widget {

    Q_OBJECT

public:
    explicit STD_MaterialWidget(QWidget *parent = 0);
    ~STD_MaterialWidget();

    RB_String getName() const;
    RB2::PerspectiveType getPerspectiveType() const;
    void init();

    bool fileSave(bool withSelect);
    void fileRevert();

private:
    void initModelMapping();

    Ui::STD_MaterialWidget *ui;
    RB_MmProxy* mModel;
    RB_MmProxy* mRp02Model;
    RB_MmProxy* mRp10Model;
    RB_MmProxy* mRmMinModel;
};

#endif // STD_MATERIALWIDGET_H
