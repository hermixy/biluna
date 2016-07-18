/*****************************************************************
 * $Id: std_selectflangewidget.h 0001 2016-07-13T17:39:51 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna STD project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#ifndef STD_SELECTFLANGEWIDGET_H
#define STD_SELECTFLANGEWIDGET_H

#include <QObject>
#include "std_selectcomponentwidget.h"

namespace Ui {
class STD_SelectComponentWidget;
}

class STD_SelectFlangeWidget : public STD_SelectComponentWidget {

    Q_OBJECT

public:
    explicit STD_SelectFlangeWidget(QWidget* parent = 0);
    virtual ~STD_SelectFlangeWidget();

    QString getId() const { return RB_String(); }
    QString getName() const { return QString("Select Flange"); }
    RB2::PerspectiveType getPerspectiveType() const {
            return RB2::PerspectivePCALC;
    }

    void init();

    // Catch signals from RB_DialogWindow
    bool fileSave(bool /*withSelect*/) { return false; }
    void fileRevert() { }

protected slots:
    void slotDimensionRowChanged(int row);
    void slotTypeRowChanged(int row);

};

#endif // STD_SELECTFLANGEWIDGET_H
