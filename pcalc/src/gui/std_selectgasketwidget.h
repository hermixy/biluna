/*****************************************************************
 * $Id: std_selectgasketwidget.h 0001 2016-08-03 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna STD project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#ifndef STD_SELECTGASKETWIDGET_H
#define STD_SELECTGASKETWIDGET_H

#include <QObject>
#include "std_selectcomponentwidget.h"

namespace Ui {
class STD_SelectComponentWidget;
}

class STD_SelectGasketWidget : public STD_SelectComponentWidget {

    Q_OBJECT

public:
    explicit STD_SelectGasketWidget(QWidget* parent = 0);
    virtual ~STD_SelectGasketWidget();

    QString getId() const { return RB_String(); }
    QString getName() const { return QString("Select Gasket"); }
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
    void slotRatingRowChanged(int row);

private:
    RB_ObjectBase* mGasketObject;

};

#endif // STD_SELECTGASKETWIDGET_H
