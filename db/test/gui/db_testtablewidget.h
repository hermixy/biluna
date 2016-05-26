/*****************************************************************
 * $Id: db_testtablewidget.h 1771 2012-09-06 21:13:20Z rutger $
 * Created: Nov 5, 2009 9:08:25 PM - Rutger Botermans
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_TESTTABLEWIDGET_H
#define DB_TESTTABLEWIDGET_H

#include <QStringListModel>
#include "rb_datawidgetmapper.h"
#include "rb_widget.h"
#include "ui_db_testtablewidget.h"

class RB_MmObjectInterface;

/**
 * Complex test widget class, with all model view widgets
 */
class DB_TestTableWidget : public RB_Widget, private Ui::DB_TestTableWidget {

    Q_OBJECT

public:
    DB_TestTableWidget(QWidget* parent = 0);
    virtual ~DB_TestTableWidget();

    virtual RB_String getId() const { return RB_String(); }
    virtual RB_String getName() const { return "Test Table View"; }
    virtual RB2::PerspectiveType getPerspectiveType() const {
        return RB2::PerspectiveDB;
    }

    virtual void init();
    virtual bool fileSave(bool withSelect);
    virtual void fileRevert();

    virtual bool closeWidget();

protected slots:
    // Push buttons
    virtual void on_pbSelectRel_clicked();
    virtual void on_pbPrevious_clicked();
    virtual void on_pbNext_clicked();

    virtual void on_pbTest_clicked();
    void reject();

protected:
    void changeEvent(QEvent* e);

private:
    bool maybeSave();

    //! Data model
    RB_MmProxy* mModel;
    //! Child data model
    RB_MmProxy* mChildModel;
    //! Model for combobox
    QStringListModel* mComboBoxModel;
    //! Data widget mapper
    RB_DataWidgetMapper* mMapper;


};

#endif // DB_TESTTABLEWIDGET_H
