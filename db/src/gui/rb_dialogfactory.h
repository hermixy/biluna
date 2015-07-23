/*****************************************************************
 * $Id: rb_dialogfactory.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Nov 3, 2009 6:07:28 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef RB_DIALOGFACTORY_H
#define RB_DIALOGFACTORY_H

#include <QDockWidget>
#include <QObject>
#include <QMenu>
#include <QModelIndex>
#include "db_global.h"
#include "rb_dialog.h"
#include "rb_string.h"
#include "rb_stringlist.h"
#include "rb_widget.h"

class RB_DockWidget;
class RB_MainWindow;
class RB_MdiWindow;
class RB_MmProxy;
class RB_ObjectBase;
class RB_ToolBar;


/**
 * Interface for objects that can create and show dialogs.
 */
class DB_EXPORT RB_DialogFactory : public QObject {

    Q_OBJECT

public:
    RB_DialogFactory(RB_MainWindow* mw);
    virtual ~RB_DialogFactory();

    virtual RB_MdiWindow* getActiveMdiWindow();
    virtual RB_Widget* getActiveMdiCentralWidget();
    virtual RB_MdiWindow* getMdiWindow(int type, const RB_String& docName = "",
                                       bool isNewWidget = false);
    virtual RB_ToolBar* getToolBar(int widgetType, const RB_String& tbName,
                                   int tbArea);
    virtual RB_DockWidget* getDockWidget(int widgetType, const RB_String& dockName,
                                         int dockArea);
    virtual RB_MainWindow* getMainWindow();

    // To be overridden by implementation, not a slot because of return value
    virtual RB_Widget* getWidget(int type, QWidget* parent);
    virtual bool removeWidget(int type);
    virtual bool isWidgetActive(int type);
    //! To be overridden by implementation, not a slot because of return value
    virtual RB_Dialog* getDialog(int type) = 0;

    virtual void requestInformationDialog(const RB_String& info);
    virtual RB_String requestTextInputDialog(const RB_String& dialogTitle,
                                             const RB_String& labelName,
                                             const RB_String& defaultText);
    virtual void requestWarningDialog(const RB_String& warn);
    virtual int requestYesNoDialog(const RB_String& dialogTitle,
                                          const RB_String& question);
    virtual int requestYesNoCancelDialog(const RB_String& dialogTitle,
                                          const RB_String& question);
    virtual bool requestOneTimeDialog(const RB_String& text, const RB_String& id);

    virtual void commandMessage(const RB_String& message);
    virtual void commandMessage(const RB_StringList& messageList);

    // Shortcuts to mainwindow functions
    virtual void statusBarMessage(const RB_String& text, int timeout);

    //! Update signal/slot connection based on state of dialogs or widgets
    virtual void updateUiConnections() = 0;
    virtual int getCurrentPerspective();
    virtual void setPerspective(int perspectiveType);

    virtual void closeAllMdiWindows();
    virtual void deleteAllMdiWindows();
    virtual void closeAllDockWidgets();
    virtual void deleteAllDockWidgets();
    virtual void closeAllToolBars();
    virtual void deleteAllToolBars();

    // For debugging only
    virtual void setSignalSpyTarget(QObject* target);

public slots:
    void slotSettingChanged(int dialogWidgetType);

signals:
    void stateChanged();
    void settingChanged(int dialogWidgetType);

protected:
    virtual RB_Widget* findWidget(int type);

    //! Main window
    RB_MainWindow* mMainWindow;
    //! List of unique widgets, for dockwidgets and retrieval of other unique widgets
    std::map<int, RB_Widget*> mWidgetList;
    //! Dialog factory perspective
    int mPerspective;


};

#endif // RB_DIALOGFACTORY_H
