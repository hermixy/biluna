/*****************************************************************
 * $Id: db_testtabletreewidget.h 1419 2011-05-16 13:43:55Z rutger $
 * Created: Nov 5, 2009 9:08:25 PM - Rutger Botermans
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_TESTTABLETREEWIDGET_H
#define DB_TESTTABLETREEWIDGET_H

#include <QStringListModel>
#include "rb_datawidgetmapper.h"
#include "rb_widget.h"
#include "ui_db_testtabletreewidget.h"

class RB_MmObjectInterface;


class DB_TestTableTreeWidget : public RB_Widget, private Ui::DB_TestTableTreeWidget {

    Q_OBJECT

public:
    DB_TestTableTreeWidget(QWidget* parent = 0);
    virtual ~DB_TestTableTreeWidget();

    virtual RB_String getId() const { return RB_String(); }
    virtual RB_String getName() const { return "Test Tree View"; }
    virtual RB2::PerspectiveType getPerspectiveType() const { return RB2::PerspectiveTEST; }

    virtual void init();
    virtual bool fileSave();
    virtual void fileRevert();

    virtual bool closeWidget();

public slots:
    void on_pbTableAdd_clicked();
    void on_pbTableDelete_clicked();
//    void on_pbTreeUp_clicked();
//    void on_pbTreeDown_clicked();
    void on_pbFind_clicked();
//    void on_pbFilter_clicked();
//    void on_pbFilterClear_clicked();
    void on_pbTreeAdd_clicked();
    void on_pbTreeAddChild_clicked();
    void on_pbTreeDelete_clicked();
    void on_pbPromote_clicked();
    void on_pbDemote_clicked();
    void on_pbTest_clicked();

protected:
    void changeEvent(QEvent* e);

private:
    //! Data model
    RB_MmProxy* mModel;
    //! Child data model
    RB_MmProxy* mChildModel;
    //! Model for combobox
    QStringListModel* mComboBoxModel;
    //! Data widget mapper
    RB_DataWidgetMapper* mMapper;

};

#endif // DB_TESTTABLETREEWIDGET_H
