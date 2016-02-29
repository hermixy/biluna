/*****************************************************************
 * $Id: rb_uomlineedit.h 2248 2016-02-29 09:13:00Z rutger $
 * Created: Feb 29, 2016 7:50:25 AM - Rutger Botermans
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef RB_UOMLINEEDIT_H
#define RB_UOMLINEEDIT_H

#include <QDoubleValidator>
#include <QLineEdit>
#include <QWidget>
#include "ui_rb_uomlineedit.h"

#include "db_global.h"
#include "rb_string.h"

class RB_DialogFactory;
class RB_MmProxy;

/**
 * Widget line edit class for data with a unit of measurement label
 * The QLineEdit name is 'uomLineEdit' and should not be changed,
 * because it is used in RB_DataWidgetMapper to distinguish between
 * a regular QLineEdit
 */
class DB_EXPORT RB_UomLineEdit : public QWidget, private Ui::RB_UomLineEdit {

    Q_OBJECT

public:
    explicit RB_UomLineEdit(QWidget *parent = 0);
    ~RB_UomLineEdit();

    QLineEdit* getLineEdit();
    QString text() const;
    void setText(const QString& txt);

    void setSection(int section);
    void setModel(RB_MmProxy* model);
    void setValidator(const QValidator* validator);
    void setUom();

public slots:
    void setEnabled(bool enable);

private:
    int mSection;
    RB_MmProxy* mModel;
};

#endif // RB_UOMLINEEDIT_H
