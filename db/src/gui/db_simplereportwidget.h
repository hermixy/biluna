/*****************************************************************
 * $Id: db_simplereportwidget.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Sept 27, 2012 9:00:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_SIMPLEREPORTWIDGET_H
#define DB_SIMPLEREPORTWIDGET_H

#include "rb_simplereportwidget.h"
#include "ui_db_simplereportwidget.h"

class RB_ObjectAtomic;
class RB_ObjectContainer;
class DB_SqlHighlighter;

/**
 * Simple database report widget
 */
class DB_EXPORT DB_SimpleReportWidget : public RB_SimpleReportWidget,
        private Ui::DB_SimpleReportWidget {

    Q_OBJECT

public:
    explicit DB_SimpleReportWidget(QWidget *parent = 0);
    virtual ~DB_SimpleReportWidget();

    virtual RB_String getId() const { return RB_String(); }
    virtual RB_String getName() const { return "Simple Database Report"; }
    virtual RB2::PerspectiveType getPerspectiveType() const { return RB2::PerspectiveDB; }

    virtual void init();
    virtual bool fileSave(bool withSelect);
    virtual void fileRevert();
    virtual RB_String getHelpSubject() const;

public slots:
    virtual void on_pbAdd_clicked();
    virtual void on_pbDelete_clicked();
    virtual void on_pbSave_clicked();
    virtual void on_pbFilterOn_clicked();
    virtual void on_pbFilterOff_clicked();
    virtual void on_pbRefresh_clicked();

protected:
    void changeEvent(QEvent *e);

private:
    void setComboBoxes();

    void setColumnWidthsAndTitle();

    //! Report data model
    RB_MmProxy* mModel;
    //! Report data widget mapper
    RB_DataWidgetMapper* mMapper;
    //! SQL code highlighter
    DB_SqlHighlighter* mCodeHighlighter;
};

#endif // DB_SIMPLEREPORTWIDGET_H
