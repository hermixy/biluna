/*****************************************************************
 * $Id: rb_dialogwindow.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Jan 28, 2016 12:34:42 AM - rutger
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef RB_DIALOGWINDOW_H
#define RB_DIALOGWINDOW_H

#include <QDialog>
#include <QModelIndex>
#include <QObject>
#include "rb.h"
#include "db_commandevent.h"
#include "db_global.h"
#include "rb_widget.h"

class RB_Action;
class RB_MainWindow;


/**
 * Base MDI window class
 */
class DB_EXPORT RB_DialogWindow : public QDialog {

    Q_OBJECT

public:
    RB_DialogWindow(RB_MainWindow* mw, RB_Widget* mwgt);
    virtual ~RB_DialogWindow();

    virtual RB_String getId() const;
    virtual RB_String getName() const;
    virtual RB_String getSaveAsFileName() const;
    virtual int getWidgetType() const;
    virtual void setWidgetType(int type);
    virtual int getParentWidgetType() const;
    virtual void setParentWidgetType(int type);
    virtual RB2::PerspectiveType getPerspectiveType() const;
    virtual RB_Widget* getCentralWidget();

    virtual RB_ObjectBase* getCurrentObject();
    virtual RB_ObjectBase* getCurrentChild1Object();
    virtual RB_ObjectBase* getCurrentChild2Object();


    // to be overriden by drawing type windows
    virtual void commandEvent(DB_CommandEvent* /* e */) { }
    virtual RB_Action* getCurrentAction() { return NULL; }

    virtual void initHelpClose();

signals:


public slots:
    virtual void show();
    virtual int exec();

    virtual void slotChangedModification(bool dirty);

    virtual void slotHelp();
    virtual void slotOk();
    virtual void slotCancel();

protected:
    void closeEvent(QCloseEvent* e);
    // void retranslateUi(); TODO

    QGridLayout* gridLayout_dw;
    QHBoxLayout* horizontalLayout_dw;
    QPushButton* pbHelp;
    QSpacerItem* horizontalSpacer_dw;
    QPushButton* pbOk;
    QPushButton* pbCancel;

private:
    void updateWindowTitle(bool dirty);

    //! Parent mainwindow
    RB_MainWindow* mMainWindow;
    //! Central widget
    RB_Widget* mCentralWidget;
};

#endif // RB_DIALOGWINDOW_H
