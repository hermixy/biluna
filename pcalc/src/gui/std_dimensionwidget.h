/*****************************************************************
 * $Id: std_dimensionwidget.h 0001 2016-06-29T10:39:19 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna STD project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#ifndef STD_DIMENSIONWIDGET_H
#define STD_DIMENSIONWIDGET_H

#include "rb_widget.h"
#include "std_dimensiontablehelper.h"

namespace Ui {
class STD_DimensionWidget;
}

class STD_DimensionWidget : public RB_Widget, STD_DimensionTableHelper {

    Q_OBJECT

public:
    explicit STD_DimensionWidget(QWidget* parent = 0);
    ~STD_DimensionWidget();

    QString getId() const { return RB_String(); }
    QString getName() const { return QString("Component Dimensions"); }
    RB2::PerspectiveType getPerspectiveType() const {
            return RB2::PerspectivePCALC;
    }

    void init();

    // Catch signals from RB_DialogWindow
    bool fileSave(bool withSelect);
    void fileRevert();


protected slots:
    void slotDetailTables(const QModelIndex& current,
                          const QModelIndex& previous);

private:
    Ui::STD_DimensionWidget* ui;
};

#endif // STD_DIMENSIONWIDGET_H
