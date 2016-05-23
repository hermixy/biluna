/*****************************************************************
 * $Id: db_testtreewidget.h 1419 2011-05-16 13:43:55Z rutger $
 * Created: Nov 5, 2009 9:08:25 PM - Rutger Botermans
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_TESTTREEWIDGET_H
#define DB_TESTTREEWIDGET_H

#include <QStringListModel>
#include "rb_datawidgetmapper.h"
#include "rb_widget.h"
#include "ui_db_testtreewidget.h"

class RB_MmObjectInterface;


class DB_TestTreeWidget : public RB_Widget, private Ui::DB_TestTreeWidget {

    Q_OBJECT

public:
    DB_TestTreeWidget(QWidget* parent = 0);
    virtual ~DB_TestTreeWidget();

    virtual RB_String getId() const { return RB_String(); }
    virtual RB_String getName() const { return "Test Tree View"; }
    virtual RB2::PerspectiveType getPerspectiveType() const { return RB2::PerspectiveTEST; }

    virtual void init();
    virtual bool fileSave();
    virtual void fileRevert();

    virtual void editCut();
    virtual void editCopy();
    virtual void editPaste();
    virtual void editUndo();
    virtual void editRedo();

    virtual void formatBold();
    virtual void formatItalic();
    virtual void formatUnderline();
    virtual void formatStrikethrough();
    virtual void formatAlignLeft();
    virtual void formatAlignCenter();
    virtual void formatAlignRight();
    virtual void formatAlignJustify();
    virtual void formatIncreaseIndent();
    virtual void formatDecreaseIndent();
    virtual void formatNumberedList();
    virtual void formatBulletedList();
    virtual void formatFontName();
    virtual void formatFontSize();
    virtual void formatTextColor();
    virtual void formatBackgroundColor();
    virtual void formatRemove();

    virtual bool closeWidget();

public slots:
    void on_pbTreeAdd_clicked();
    void on_pbTreeAddChild_clicked();
    void on_pbTreeDelete_clicked();
//    void on_pbTreeUp_clicked();
//    void on_pbTreeDown_clicked();
    void on_pbFind_clicked();
//    void on_pbFilter_clicked();
//    void on_pbFilterClear_clicked();
    void on_pbTableAddChild_clicked();
    void on_pbTableDeleteChild_clicked();
    void on_pbTableUpChild_clicked();
    void on_pbTableDownChild_clicked();
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

#endif // DB_TESTTREEWIDGET_H
