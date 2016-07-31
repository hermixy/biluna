/*****************************************************************
 * $Id: std_selectboltwidget.h 0001 2016-07-31T08:39:51 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna STD project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#ifndef STD_SELECTBOLTWIDGET_H
#define STD_SELECTBOLTWIDGET_H

#include <QObject>
#include "std_selectcomponentwidget.h"

namespace Ui {
class STD_SelectComponentWidget;
}

class STD_SelectBoltWidget : public STD_SelectComponentWidget {

    Q_OBJECT

public:
    explicit STD_SelectBoltWidget(QWidget* parent = 0);
    virtual ~STD_SelectBoltWidget();

    QString getId() const { return RB_String(); }
    QString getName() const { return QString("Select Bolt"); }
    RB2::PerspectiveType getPerspectiveType() const {
            return RB2::PerspectivePCALC;
    }

    virtual RB_ObjectBase* getCurrentObject();
    void init();

    // Catch signals from RB_DialogWindow
    bool fileSave(bool /*withSelect*/) { return false; }
    void fileRevert() { }

protected slots:
    void slotStandardRowChanged(int row);
    void slotTypeRowChanged(int row);

private:
    RB_ObjectBase* mBoltObject;

};

#endif // STD_SELECTBOLTWIDGET_H
