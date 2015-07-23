/*****************************************************************
 * $Id: rb_mainwindow.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Nov 4, 2009 12:34:42 AM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef RB_MAINWINDOW_H
#define RB_MAINWINDOW_H

#include <QApplication>
#include <QCloseEvent>
#include <QFileDialog>
#include <QMainWindow>
#include <QMdiArea>
#include <QMessageBox>
#include <QMenu>
#include <QMenuBar>
#include <QProcess>
#include <QSignalMapper>
#include <QStatusBar>

#include "db_global.h"
#include "rb_string.h"
#include "rb_stringlist.h"

class RB_ActionFactory;
class RB_DialogFactory;
class RB_DockWidget;
class RB_GuiAction;
class RB_ModelFactory;
class RB_MdiWindow;
class RB_Widget;


/**
 * Base class for mainwindows that contain a MDI central widget and basic
 * functions such XML file opening/saving, connection to database and help.
 */
class DB_EXPORT RB_MainWindow : public QMainWindow {

    Q_OBJECT

public:
    RB_MainWindow(QWidget* parent = 0, Qt::WindowFlags flags = 0);
    virtual ~RB_MainWindow();

    virtual QMdiArea* getMdiArea();
    virtual RB_MdiWindow* getActiveMdiWindow();
    virtual void setActiveMdiWindow(RB_MdiWindow* w);
    virtual RB_MdiWindow* findMdiWindow(const RB_String& idFileName,
                                        int widgetType = -1);

    virtual void closeAllMdiWindows();
    virtual void closeAllDockWidgets();

    virtual void setSignalSpyTarget(QObject* target);

public slots:
    // TODO remove part of MDI windows/widgets, slots usually do not have a return value
    virtual void slotFileNew();
    virtual bool slotFileOpen();
    virtual bool slotFileSave();
    virtual bool slotFileSaveAs(const RB_String& fileName = RB_String());
    virtual void slotFilePdf();
    virtual void slotFilePrint();
    virtual void slotFilePrintPreview(bool);
    virtual void slotFileQuit();

    virtual void slotHelpAbout();
    virtual void slotHelpIndex();
    virtual void slotHelpSubject(const RB_String& subject);
    virtual void slotHelpCredits();
    virtual void slotHelpFinished(int, QProcess::ExitStatus);

    virtual void slotDebugPlugins();
    virtual void slotDebugSignalSpy();
    virtual void slotDebugExportModel();

    virtual void closeEvent(QCloseEvent* event);

signals:
    void subWindowActive(int active);
    void subWindowChanged(int type);
    void databaseStatusChanged();

protected:
    virtual void initWindowActions();

    virtual void readSettings();
    virtual void writeSettings();
    virtual bool maybeSave();
    virtual bool loadFile(RB_ModelFactory* model,
                          const RB_String& fileName,
                          const RB_String& fileType);
    virtual bool saveModel(RB_ModelFactory* model,
                           const RB_String& fileName);
    virtual bool isValidFileType(const RB_String& /* type */) { return true; } // = 0;

    //! MDI area is central widget
    QMdiArea* mMdiArea;
    //! MDI window signal mapper
    QSignalMapper* mWindowMapper;

    // Menus that can be re-initialized
    QMenu* mWindowMenu;

    // Actions that need to be re-initialized at menu about to be shown
    RB_GuiAction* mWindowCascadeAction;
    RB_GuiAction* mWindowCloseAction;
    RB_GuiAction* mWindowCloseAllAction;
    RB_GuiAction* mWindowNextAction;
    RB_GuiAction* mWindowPreviousAction;
    QAction* mWindowSeparatorAction;
    RB_GuiAction* mWindowTabAction;
    RB_GuiAction* mWindowTileAction;

    // For Help > About dialog
    //! Edition can be such as [Professional], [Demo], [Student], etc.
    RB_String mEdition;
    //! List of activated modules such as DB, LDT, etc.
    RB_StringList mModules;
    //! QtAssistant process
    QProcess* mHelpProcess;

    //! Signal spy target
    QObject* mSignalSpyTarget;

protected slots:
    virtual void slotSetActiveSubWindow(QWidget* window);
    virtual void slotUpdateMenus(QMdiSubWindow*);
    virtual void slotUpdateWindowMenu();

    virtual void cascadeSubWindows();
    virtual void tabSubWindows();
    virtual void tileSubWindows();


};

#endif // RB_MAINWINDOW_H
