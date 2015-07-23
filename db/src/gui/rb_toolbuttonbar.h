/*****************************************************************
 * $Id: rb_toolbuttonbar.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Mar 18, 2014 8:27 PM - rutger
 *
 * Copyright (C) 2014 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef RB_TOOLBUTTONBAR_H
#define RB_TOOLBUTTONBAR_H

#include <QtWidgets/QWidget>
#include "db_global.h"
#include "rb_debug.h"

#include "ui_rb_toolbuttonbar.h"

/**
 * Set permissions of database users
 */
class DB_EXPORT RB_ToolButtonBar : public QWidget, private Ui::RB_ToolButtonBar {

    Q_OBJECT

public:
    explicit RB_ToolButtonBar(QWidget *parent = 0);
    ~RB_ToolButtonBar();

    virtual void initSlimTable(bool isEdit = true, bool isFindFilter = false);

    virtual void initEdit(bool isTreeModel = false,
                          bool isSaveRevert = true,
                          bool isFindFilter = false);
    virtual void initDetailEdit(bool isSaveRevert = true);
    virtual void setText(const RB_String& txt);
    virtual RB_String getFindFilterText();
    virtual void setFilterOffEnabled(bool enabled);

public slots:
    virtual void on_pbAdd_clicked();
    virtual void on_pbAddChild_clicked();
    virtual void on_pbDelete_clicked();
    virtual void on_pbEdit_clicked();
    virtual void on_pbSave_clicked();
    virtual void on_pbRevert_clicked();

    virtual void on_pbPrevious_clicked();
    virtual void on_pbNext_clicked();
    virtual void on_pbFind_clicked();
    virtual void on_pbFilterOn_clicked();
    virtual void on_pbFilterOff_clicked();

    virtual void on_pbDemote_clicked();
    virtual void on_pbPromote_clicked();
    virtual void on_pbUp_clicked();
    virtual void on_pbDown_clicked();

signals:
    void addClicked();
    void addChildClicked();
    void deleteClicked();
    void editClicked();
    void saveClicked();
    void revertClicked();

    void previousClicked();
    void nextClicked();
    void findClicked();
    void filterOnClicked();
    void filterOffClicked();

    void demoteClicked();
    void promoteClicked();
    void upClicked();
    void downClicked();

protected:
    void changeEvent(QEvent *e);

private:
    void setupSlimTable(bool isEdit, bool isFindFilter);

    // Only used in setupSlimTable() for dockwidget for example
//    QGridLayout* gridLayout;
//    QToolButton* pbAdd;
//    QToolButton* pbDelete;
//    QSpacerItem* hsAddDelete;
//    QToolButton* pbFind;
//    QLineEdit* leFilter;
//    QToolButton* pbFilterOn;
//    QToolButton* pbFilterOff;
//    QSpacerItem* hsFilter;
};

#endif // RB_TOOLBUTTONBAR_H
