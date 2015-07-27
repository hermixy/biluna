/*****************************************************************
 * $Id: gv_mdiwindow.h 926 2009-02-04 21:04:56Z rutger $
 * Created: Oct 2, 2008 5:50:14 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef GV_MDIWINDOW_H
#define GV_MDIWINDOW_H

#include <QtGui>
#include "gv.h"
#include "rb_string.h"
class GV_Drawing;
class GV_GvMdi;
class GV_MainWindow;
class GV_Scene;
class GV_Text;
class QMenu;

/**
 * Multiple Document Interface child window
 */
class GV_MdiWindow : public QMdiSubWindow {
	
    Q_OBJECT

public:
    GV_MdiWindow(GV_MainWindow* mw, GV_Drawing* drawing);
    virtual ~GV_MdiWindow();
    
    virtual GV_GvMdi* getGraphicsView() { return mGraphicsView; }
    virtual GV_Drawing* getDrawing() { return mDrawing; }
    
    void fileNew();
        
    // graphicview
    void fileSaveScreenShot();
    void filePrint(const RB_String& fileName = RB_String());
    
    void itemDelete();
    void itemMoveToFront();
    void itemMoveToBack();

    void setFont(const QFont& font);
    void setTextColor(const QColor& color);
    void setItemStrokeColor(const QColor& color);
    void setItemFillColor(const QColor& color);
    
    void helpTest(); // test only
    
    RB_String documentId();
    RB_String documentName();
    
    void synchronizeModelData();
    
public slots:
	void windowStateHasChanged(Qt::WindowStates oldState,
							   Qt::WindowStates newState);

signals:
	
protected:
	void closeEvent(QCloseEvent* event);

private slots:
	void slotSceneChanged(const QList<QRectF>&);

private:
	void createActions();
	void createConnections();
	void refreshToggledActions();

    bool maybeSave();

    /** Current Document */
    bool isUntitled;
    
    /** parent mainwindow */
    GV_MainWindow* mMainWindow;
    
    /** Graphics View */
    GV_GvMdi* mGraphicsView;
    /** Drawing object of overall GV data model including scene */
	GV_Drawing* mDrawing;

	/** Main widget which is actually a QMainWindow */
	QMainWindow* mMainWidget;

	/** Toggled actions need to be reset when window is activated */
	QAction* mActionViewDraft;
	QAction* mActionViewGrid;
};


#endif /*GV_MDIWINDOW_H*/
