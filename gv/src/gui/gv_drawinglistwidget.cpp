/*****************************************************************
 * $Id: gv_drawinglistwidget.cpp 1450 2011-08-29 17:40:53Z rutger $
 * Created: Apr 21, 2010 4:47:33 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "gv_drawinglistwidget.h"

#include "gv_dialogfactory.h"
#include "gv_drawingwidget.h"
#include "gv_modelfactory.h"
#include "rb_datawidgetmapper.h"
#include "rb_mdiwindow.h"


/**
 * Constructor
 */
GV_DrawingListWidget::GV_DrawingListWidget(QWidget *parent) : RB_Widget(parent) {

    setupUi(this);

    mModel = NULL;
    // mSelectionModel = NULL;
    mSizeHint = QSize(190, 600);
}

/**
 * Destructor
 */
GV_DrawingListWidget::~GV_DrawingListWidget() {
    // unique models are always deleted by the widgets,
    // can be MDI window or dialog, but also a dockWindow with table or tree
    delete mModel;
    RB_DEBUG->print("GV_DrawingListWidget::~GV_DrawingListWidget() OK");
}


/**
 * Initialize widget and models
 */
void GV_DrawingListWidget::init() {
    QApplication::setOverrideCursor(Qt::WaitCursor);

    RB_ObjectBase* project = GV_MODELFACTORY->getRoot();
    leProjectNumber->setText(project->getValue("number").toString());
    leProjectNumber->setReadOnly(true);

    //
    // 1. Set model and connect to table view
    //
    mModel = GV_MODELFACTORY->getModel(GV_ModelFactory::ModelDrawing);
    mModel->setRoot(GV_MODELFACTORY->getRootId());

    //
    // 2. Relations
    //    mModel->setRelation(mModel->fieldIndex(field),
    //           QSqlRelation(relationTable, relationId, relationField));

    //
    // 3. Select after relations have been set, only for database models
    //
    mModel->select();
    mModel->setHeaderData(0, Qt::Horizontal, tr("Number"));
    mModel->setHeaderData(1, Qt::Horizontal, tr("Revision"));
    mModel->setHeaderData(2, Qt::Horizontal, tr("Description"));

    //
    // 4. Connect model to main view, set delegates and hide columns
    //
    setFormatTableView(tvDrawingList, mModel);

    for (int i = 0; i < mModel->columnCount(QModelIndex()); ++i) {
        if (i != RB2::HIDDENCOLUMNS) {
            tvDrawingList->hideColumn(i);
        }
    }

    // mSelectionModel = tvDrawingList->selectionModel(); // TODO: still required?

    // open window when drawing is double clicked
    connect(tvDrawingList, SIGNAL(doubleClicked(const QModelIndex&)),
            this, SLOT(slotDrawingListDoubleClicked(const QModelIndex&)));

    readSettings();
    tvDrawingList->sortByColumn(-1);
    QApplication::restoreOverrideCursor();
}

/**
 * Button add drawing clicked
 */
void GV_DrawingListWidget::on_pbAddDrawing_clicked() {
    if (!mModel) return;

    // always insert at the end
    int row = mModel->rowCount();
    mModel->insertRows(row, 1, QModelIndex());

    // NOTE: do not forget to set the default column values, specially for the
    //       relational table otherwise new row will not show!
    // QModelIndex index = mModel->index(row, mModel->fieldIndex("salestype"));
    // mModel->setData(index, "0", Qt::EditRole);
    // mModel->submit();
    // end NOTE

    tvDrawingList->setCurrentIndex(mModel->index(row, 0, QModelIndex()));
}

/**
 * Button delete drawing clicked
 */
void GV_DrawingListWidget::on_pbDeleteDrawing_clicked() {
    if (!mModel) return;

    QModelIndex index = tvDrawingList->currentIndex();
    if (index.isValid()) {
        mModel->removeRows(index.row(), 1, QModelIndex());
    }
}

/**
 * Drawing in drawinglist doubleclicked to open a drawing
 */
void GV_DrawingListWidget::slotDrawingListDoubleClicked(const QModelIndex& idx) {
    if (!idx.isValid()) return;

    // Create widget
    RB_MdiWindow* mdiWin = GV_DIALOGFACTORY->getMdiWindow(
            GV_DialogFactory::WidgetDrawing);
    mdiWin->show();

    // Drawing of grid and drawingborder is initiated by
    // GV_GraphicsView::slotSliderValueChanged(), the slider moves during
    // creation of the MDI window. The grid is on by default in action and
    // at creation of window.
}

void GV_DrawingListWidget::changeEvent(QEvent* e) {
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}
