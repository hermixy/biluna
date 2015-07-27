/*****************************************************************
 * $Id: gv_mdiwindow.cpp 941 2009-02-24 21:42:47Z rutger $
 * Created: Oct 2, 2008 5:52:56 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "gv_mdiwindow.h"

#include <iostream>
#include "gv_actionbase.h"
#include "gv_actioncomponent.h"
#include "gv_actiondeleteitem.h"
#include "gv_actiondrawline.h"
#include "gv_actiondrawsymbol.h"
#include "gv_actionmodifymergeline.h"
#include "gv_actionmodifymove.h"
#include "gv_actionmodifysplitline.h"
#include "gv_actionsetsnapmode.h"
#include "gv_actionsetsnapmodeauto.h"
#include "gv_actionsetsnapmodefree.h"
#include "gv_actionsetsnapmodegrid.h"
#include "gv_actionsetsnaprestriction.h"
#include "gv_actionsetsnaprestrictionnothing.h"
#include "gv_actionsetsnaprestrictionorthogonal.h"
#include "gv_actionviewdraft.h"
#include "gv_actionviewgrid.h"
#include "gv_actionviewstatusbar.h"
#include "gv_actionzoomauto.h"
#include "gv_actionzoomin.h"
#include "gv_actionzoomout.h"
#include "gv_actionzoompan.h"
#include "gv_actionzoomprevious.h"
#include "gv_actionzoomredraw.h"
#include "gv_actionzoomselection.h"
#include "gv_actionzoomwindow.h"

#include "gv_drawing.h"
#include "gv_gvmdi.h"
#include "gv_mainwindow.h"
// #include "gv_symbol.h"
#include "gv_text.h"
#include "rb_debug.h"
#include "rb_settings.h"
#include "rb_stringlist.h"



GV_MdiWindow::GV_MdiWindow(GV_MainWindow* mw, GV_Drawing* drawing) {
	RB_DEBUG->print("GV_MdiWindow::GV_MdiWindow()");
	
	mMainWindow = mw;
	mDrawing = drawing;
	
	if (!mDrawing) {
		RB_DEBUG->print(RB_Debug::D_ERROR, 
				"GV_MdiWindow::GV_MdiWindow() no drawing ERROR");
	} 
	
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle(mDrawing->getValue("number").toString() + "[*]");
    setWindowModified(false);
    
    // TODO: read settings

	// QMainWindow as widget
	mMainWidget = new QMainWindow(this);
	Qt::WindowFlags flags = mMainWidget->windowFlags() & ~Qt::Window;
	mMainWidget->setWindowFlags(flags | Qt::Widget);
	mGraphicsView = new GV_GvMdi(this);
	mMainWidget->setCentralWidget(mGraphicsView);
	this->setWidget(mMainWidget);

	// initialize drawing
    mDrawing->initDrawingArea(true);
    mDrawing->synchronizeSceneData();
//	mDrawing->setDefaultAction(new GV_ActionBase(*mDrawing, *mGraphicsView));
//	mDrawing->setDefaultAction(new GV_ActionSelectSingle(*mDrawing, *mGraphicsView));
	mDrawing->setDefaultAction(new GV_ActionModifyMove(*mDrawing, *mGraphicsView));
    mDrawing->setContextMenu(mw->sceneContextMenu());
    mGraphicsView->setScene(mDrawing);

	createActions();
    createConnections();    
}

GV_MdiWindow::~GV_MdiWindow() {
	RB_DEBUG->print("GV_MdiWindow::~GV_MdiWindow()");

	// The model will finally delete the scene and the 
	// remaining pipeLineList, nodeList etc. 
	mGraphicsView->setScene(NULL);
	
    RB_DEBUG->print("GV_MdiWindow::~GV_MdiWindow() OK");
}

/**
 * Enable or disable widgets if window state has changed
 */
void GV_MdiWindow::windowStateHasChanged(Qt::WindowStates oldState,
							   Qt::WindowStates newState) {
	if (newState & Qt::WindowActive) {
		refreshToggledActions();
		mMainWidget->setEnabled(true);
	} else {
		mMainWidget->setEnabled(false);
	}
}

/**
 * Create menu and toolbar actions
 */
void GV_MdiWindow::createActions() {
	RB_DEBUG->print("GV_MdiWindow::createActions()");

	if (!mMainWidget) {
		RB_DEBUG->print(RB_Debug::D_ERROR,
						"GV_MdiWindow::createActions() mMainWidget ERROR");
	}

	QAction* action = NULL;
	QMenu* menu = NULL;
	QMenu* subMenu = NULL;
	QToolBar* tb = NULL;
    //
    // Screen actions: Grid / View / Zoom
	//
	menu = new QMenu(mMainWidget);
	tb = new QToolBar(mMainWidget);
	tb->setFixedHeight(30);
	tb->setIconSize(QSize(16,16));
	//tb->setMaximumHeight(20);
    tb->setObjectName("toolbar_screen");
    tb->setWindowTitle(tr("Screen"));

	mActionViewGrid = GV_ActionViewGrid::createGuiAction();
	menu->addAction(mActionViewGrid);
	tb->addAction(mActionViewGrid);

	RB_SETTINGS->beginGroup("/Appearance");
	RB_SETTINGS->writeEntry("/DraftMode", 0);
	RB_SETTINGS->endGroup();

	mActionViewDraft = GV_ActionViewDraft::createGuiAction();
	menu->addAction(mActionViewDraft);
	tb->addAction(mActionViewDraft);

	action = GV_ActionZoomRedraw::createGuiAction();
	menu->addAction(action);
	tb->addAction(action);

	action = GV_ActionZoomIn::createGuiAction();
	menu->addAction(action);
	tb->addAction(action);

	action = GV_ActionZoomOut::createGuiAction();
	menu->addAction(action);
	tb->addAction(action);

	action = GV_ActionZoomAuto::createGuiAction();
	menu->addAction(action);
	tb->addAction(action);

	action = GV_ActionZoomSelection::createGuiAction();
	menu->addAction(action);
	tb->addAction(action);

	action = GV_ActionZoomPrevious::createGuiAction();
	menu->addAction(action);
	tb->addAction(action);

	action = GV_ActionZoomWindow::createGuiAction();
	menu->addAction(action);
	tb->addAction(action);

	action = GV_ActionZoomPan::createGuiAction();
	menu->addAction(action);
	tb->addAction(action);

    menu->setTitle(tr("&Screen"));
	mMainWidget->menuBar()->addMenu(menu);
	mMainWidget->addToolBar(Qt::TopToolBarArea, tb);

    RB_DEBUG->print("GV_MdiWindow::createActions(): drawing");
    //
    // Drawing actions:
    //
    menu = new QMenu(mMainWidget);
    tb = new QToolBar(mMainWidget);
    tb->setFixedHeight(30);
    tb->setIconSize(QSize(16,16));
    //tb->setMaximumHeight(20);
    tb->setObjectName("toolbar_draw");
    tb->setWindowTitle(tr("Draw"));

    action = GV_ActionDrawLine::createGuiAction();
    menu->addAction(action);
    tb->addAction(action);

    action = GV_ActionDrawSymbol::createGuiAction();
    menu->addAction(action);
    tb->addAction(action);

    menu->addSeparator();

    action = GV_ActionDeleteItem::createGuiAction();
    menu->addAction(action);
    tb->addAction(action);

    menu->setTitle(tr("&Draw"));
    mMainWidget->menuBar()->addMenu(menu);
    mMainWidget->addToolBar(Qt::TopToolBarArea, tb);

    RB_DEBUG->print("GV_MdiWindow::createActions(): modify");
    //
    // Modification actions:
    //
    menu = new QMenu(mMainWindow);

    action = GV_ActionModifySplitLine::createGuiAction();
    menu->addAction(action);

    action = GV_ActionModifyMergeLine::createGuiAction();
    menu->addAction(action);

//    action = GV_ActionModifyMove::createGuiAction();
//    menu->addAction(action);
//    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));

//    action = RS_ActionModifyRotate::createGuiAction();
//    menu->addAction(action);
//    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));
//
//    action = RS_ActionModifyScale::createGuiAction();
//    menu->addAction(action);
//    connect(this, SIGNAL(windowsChanged(bool)), action, SLOT(setEnabled(bool)));

    menu->setTitle(tr("&Modify"));
    mMainWidget->menuBar()->addMenu(menu);


    RB_DEBUG->print("GV_MdiWindow::createActions(): snapping");
    //
    // Snapping actions:
    //
    menu = new QMenu(mMainWidget);
    action = GV_ActionSetSnapModeFree::createGuiAction();
    menu->addAction(action);

    action = GV_ActionSetSnapModeGrid::createGuiAction();
    menu->addAction(action);

    action = GV_ActionSetSnapModeAuto::createGuiAction();
    menu->addAction(action);

    action->setChecked(true);
    menu->addSeparator();
    action = GV_ActionSetSnapRestrictionNothing::createGuiAction();
    menu->addAction(action);

    action = GV_ActionSetSnapRestrictionOrthogonal::createGuiAction();
    menu->addAction(action);

    menu->setTitle(tr("&Snap"));
    mMainWidget->menuBar()->addMenu(menu);

    RB_DEBUG->print("GV_MdiWindow::createActions(): OK");
}

/**
 * Create signal slot connections
 */
void GV_MdiWindow::createConnections() {
	if (!mDrawing || !mMainWindow) return;
	
	// insert item, reset after insertion
//	connect(mDrawing, SIGNAL(itemInserted(QGraphicsItem*)),
//            this, SLOT(itemInserted(QGraphicsItem*)));
//	connect(mDrawing, SIGNAL(itemInserted(QGraphicsItem*)),
//            mMainWindow, SLOT(itemInserted(QGraphicsItem*)));

	// insert text, reset after insertion
//	connect(mDrawing, SIGNAL(textInserted(GV_Text*)),
//        this, SLOT(textInserted(GV_Text*)));
//    connect(mDrawing, SIGNAL(textInserted(GV_Text*)),
//        mMainWindow, SLOT(textInserted(GV_Text*)));
    
    // select part of text item
    connect(mDrawing, SIGNAL(itemSelected(QGraphicsItem*)),
        mMainWindow, SLOT(itemSelected(QGraphicsItem*)));
    
    // scene changed signal for global save action
    connect(mDrawing, SIGNAL(changed(const QList<QRectF>&)),
			this, SLOT(slotSceneChanged(const QList<QRectF>&)));

	// to enable or disable widget if window activated state changes
	connect(this, SIGNAL(windowStateChanged(Qt::WindowStates, Qt::WindowStates)),
			this, SLOT(windowStateHasChanged(Qt::WindowStates, Qt::WindowStates)));
}

/**
 * Hack: Reset toggled actions, because function pointer for factory()
 * is static and toggles all similar actions on MDI windows. This is because
 * actions can be started from the command line and the action has to find out
 * which is the active window in for example GV_ActionViewGrid::factory()
 * and execute the action on that particular drawing and window.
 */
void GV_MdiWindow::refreshToggledActions() {
//	mActionViewDraft->setChecked(mDrawing->isGridOn());
	mActionViewGrid->setChecked(mDrawing->isGridOn());
}

/**
 * Close window and write all data to drawing model
 */
void GV_MdiWindow::closeEvent(QCloseEvent *event) {
	if (isWindowModified()) {
		if (mDrawing) {
			mDrawing->synchronizeModelData();
		} else {
			RB_DEBUG->print(RB_Debug::D_ERROR,
				"GV_MdiWindow::closeEvent() drawing = NULL ERROR");
		}
	}
	
    event->accept();
}

RB_String GV_MdiWindow::documentId() {
	if (!mDrawing) return RB_String();
    return mDrawing->getId();
}

RB_String GV_MdiWindow::documentName() {
	if (!mDrawing) return RB_String();
    return mDrawing->getValue("name").toString();
}

void GV_MdiWindow::slotSceneChanged(const QList<QRectF>&) {
	setWindowModified(true);
	mDrawing->setSceneChanged();
}

/**
 * Save screen shot
 */
void GV_MdiWindow::fileSaveScreenShot() {
    QFileDialog dialog(this,"GV - save screenshot as", "");
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    
    QString currentFileName = "scene.png";
    
    if (dialog.exec() == QDialog::Accepted) {
    	RB_StringList fileNames = dialog.selectedFiles();
		currentFileName = fileNames.at(0);
    } else {
    	return; // false;
    }

    if (currentFileName.isEmpty()) {
        QMessageBox::warning(this, tr("GV Save As ..."),
                             tr("File not saved,\nfile name was empty."));
        return; // false;
    }
	
    // size required otherwise null pixmap
	// QPixmap pixmap(200, 200);
    QPixmap pixmap((int)mDrawing->width(), (int)mDrawing->height());
	QPainter painter(&pixmap);
	painter.setRenderHint(QPainter::Antialiasing);
	mDrawing->render(&painter);
	painter.end();
	
	pixmap.save(currentFileName);
}

/**
 * Print graphicsview of current window
 */
void GV_MdiWindow::filePrint(const RB_String& /* fileName */) {
	RB_DEBUG->print("GV_MdiWindow::filePrint()");
	
	// TODO: enter fileName and settings into QPrintDialog refer to 
	// QCad qg_mainwindow.cpp
	
	QPrinter printer;
	
	if (QPrintDialog(&printer).exec() == QDialog::Accepted) {
	     QPainter painter(&printer);
	     painter.setRenderHint(QPainter::Antialiasing);
	     mDrawing->render(&painter);
	}	
}


/**
 * Delete item(s) from scene, only GV_Polygon items are deleted. The relevant
 * text or connecting lines are deleted with the GV_Polygon item
 */
void GV_MdiWindow::itemDelete() {
	
    foreach (QGraphicsItem* item, mDrawing->selectedItems()) {
    	
    	if (item->type() > item->UserType) {
	    	GV_Component* comp = dynamic_cast<GV_Component*>(item);
//
	    	GV_Node* node = comp->origineNode();
	    	bool deleteObj = true;
	    	
	    	if (node && node->connectComponent() == comp 
	    			&& node->parentNodeType() != GL2::NodeOrigine) {
	    		mDrawing->removeItem(node);
	    		
	    		RB_ObjectContainer* nodeList = node->getParent();
	    		nodeList->remove(node, deleteObj);
	    		// not: delete node; !!
	    	} else if (node->parentNodeType() == GL2::NodeOrigine) {
	    		
	    	}
//	        
	    	mDrawing->removeItem(item);
    		RB_ObjectContainer* compList = comp->getParent();
    		compList->remove(comp, deleteObj);
	        // not: delete item; !!
    	}
    }
}

/**
 * Move selected item to front
 */
void GV_MdiWindow::itemMoveToFront() {
    if (mDrawing->selectedItems().isEmpty())
        return;

    QGraphicsItem* selectedItem = mDrawing->selectedItems().first();
    QList<QGraphicsItem*> overlapItems = selectedItem->collidingItems();

    qreal zValue = 0;
    
    foreach (QGraphicsItem* item, overlapItems) {
        if (item->zValue() >= zValue /*&& item->type() == GV_Polygon::Type */)
            zValue = item->zValue() + 0.1;
    }
    
    selectedItem->setZValue(zValue);

}

/**
 * Move selected item to back
 */
void GV_MdiWindow::itemMoveToBack() {
    if (mDrawing->selectedItems().isEmpty())
        return;

    QGraphicsItem* selectedItem = mDrawing->selectedItems().first();
    QList<QGraphicsItem*> overlapItems = selectedItem->collidingItems();

    qreal zValue = 0;
    
    foreach (QGraphicsItem* item, overlapItems) {
        if (item->zValue() <= zValue /* && item->type() == GV_Polygon::Type */ )
            zValue = item->zValue() - 0.1;
    }
    
    selectedItem->setZValue(zValue);
}

/**
 * Set font
 * @param font font type
 */
void GV_MdiWindow::setFont(const QFont& font) {
	mDrawing->setFont(font);
}

/**
 * Set text color
 * @param color
 */
void GV_MdiWindow::setTextColor(const QColor& color) {
	std::cout << "passed: GV_MdiWindow::setTextColor()" << std::endl;
    mDrawing->setTextColor(color);
	RB_DEBUG->print("mDrawing* %p", mDrawing);
    std::cout << "passed: GV_MdiWindow::setTextColor() OK" << std::endl;
}

/**
 * Set item stroke color
 * @param color
 */
void GV_MdiWindow::setItemStrokeColor(const QColor& color) {
    mDrawing->setLineColor(color);
}

/**
 * Set item fill color
 * @param color
 */
void GV_MdiWindow::setItemFillColor(const QColor& color) {
    mDrawing->setItemColor(color);
}

/**
 * 
 */
void GV_MdiWindow::helpTest() {
	RB_DEBUG->print("GV_MdiWindow::helpTest()");
	
	QList<QGraphicsItem*> itemList;
	itemList = mDrawing->selectedItems();
	QGraphicsItem* item = NULL;
	GV_Symbol* symbol = NULL;
	GV_Line* line = NULL;
	int count = itemList.count(); 
	
	if (count > 0) {
		item = itemList.at(0);
		
		if (item) {
			if (item->type() == item->UserType + 1) {
				RB_DEBUG->print("GV_MdiWindow::helpTest() passed path ... ");
				RB_DEBUG->printObject((GV_Symbol*)item);
				
			} else if (item->type() == item->UserType + 2) {
				RB_DEBUG->print("GV_MdiWindow::helpTest() passed line... ");
				RB_DEBUG->printObject((GV_Line*)item);
			} else {
				RB_DEBUG->print("GV_MdiWindow::helpTest() Unknown type");
			}
		} else {
			RB_DEBUG->print("GV_MdiWindow::helpTest() ERROR");
		}
	} else {
		RB_DEBUG->print("GV_MdiWindow::helpTest() No item selected");
	}

	
//	RB_DEBUG->print("GV_MdiWindow::helpTest() passed drawing ... ");
//	RB_DEBUG->printObject(mDrawing);
	
}

void GV_MdiWindow::synchronizeModelData() {
	RB_DEBUG->print("GV_MdiWindow::synchronizeModelData()");
	
	if (mDrawing) {
		mDrawing->synchronizeModelData();
	} else {
		RB_DEBUG->print(RB_Debug::D_ERROR,
				"GV_MdiWindow::synchronizeModelData() drawing = NULL ERROR");
	}
	
	// selection needs to be cleared otherwise setWindowModified(false) 
	// does nothing
	mDrawing->clearSelection();
	setWindowModified(false);
}

