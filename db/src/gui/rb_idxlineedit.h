/*****************************************************************
 * $Id: rb_idxlineedit.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Dec 14, 2011 1:08:25 PM - Rutger Botermans
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef RB_IDXLINEEDIT_H
#define RB_IDXLINEEDIT_H

#include <QLineEdit>
#include <QWidget>
#include "ui_rb_idxlineedit.h"

#include "db_global.h"
#include "rb_string.h"

class RB_DialogFactory;
class RB_MmProxy;

/**
 * Widget class for so-called _idx fields. These are relational
 * fields in the database which cannot be used with a QComboBox
 * due to the amount of relations. The widget contains a read-only
 * QLineEdit and a QPushButton. The QLineEdit will show the
 * content of xxxx_idx field stripped from the first id (Uuid
 * 38) characters. The QPushButton is for the opening of a
 * selection dialog.
 * The QLineEdit name is 'idxLineEdit' and should not be changed,
 * because it is used in RB_DataWidgetMapper to distinguish between
 * a regular QLineEdit
 */
class DB_EXPORT RB_IdxLineEdit : public QWidget, private Ui::RB_IdxLineEdit {

    Q_OBJECT

public:
    explicit RB_IdxLineEdit(QWidget *parent = 0);
    ~RB_IdxLineEdit();

    QLineEdit* getLineEdit();
    QString text() const;
    void setText(const QString& txt);

    void setSection(int section);
    void setModel(RB_MmProxy* model);
    void setDefaultDialog(RB_DialogFactory* dialogFactory,
                          int dialogType,
                          const RB_String& targetFieldName,
                          const RB_String& firstSourceFieldName,
                          const RB_String& secondSourceFieldName = "");

public slots:
    void setEnabled(bool enable);

signals:
    void clicked(bool checked = false);
    void pressed();
    void released();
    void toggled(bool checked);
    void clear();

protected slots:
    void slotClicked(bool checked = false);
    void slotPressed();
    void slotReleased();
    void slotToggled(bool checked);
    void slotClear();

private:
    int mSection;
    RB_MmProxy* mModel;
    RB_DialogFactory* mDialogFactory;
    int mDialogType;
    RB_String mTargetFieldName;
    RB_String mFirstSourceFieldName;
    RB_String mSecondSourceFieldName;
};

#endif // RB_IDXLINEEDIT_H
