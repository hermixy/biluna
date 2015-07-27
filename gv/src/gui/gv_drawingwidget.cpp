/*****************************************************************
 * $Id: gv_drawingwidget.cpp 1999 2013-09-26 16:36:13Z rutger $
 * Created: Apr 19, 2010 4:14:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna GV project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "gv_drawingwidget.h"

#include "db_actionfactory.h"
#include "db_actionfilesaveas.h"
#include "db_dialogfactory.h"
#include "gv_actionmodifymove.h"
#include "gv_dialogfactory.h"
#include "gv_filterxml.h"
#include "gv_modelfactory.h"
// #include "rb_filterxml.h"

/**
 * Constructor
 */
GV_DrawingWidget::GV_DrawingWidget(QWidget *parent)
        : RB_Widget(parent) {

    setupUi(this);

    mDrawing = NULL;
    mFullFileName = "";
    mGraphicsView = NULL;
}

/**
 * Destructor
 */
GV_DrawingWidget::~GV_DrawingWidget() {
    // unique models are always deleted by the widgets,
    // can be MDI window or dialog, but also a dockWindow with table or tree
    RB_DEBUG->print("GV_DrawingWidget::~GV_DrawingWidget() OK");
}

/**
 * Initialize widget and models
 */
void GV_DrawingWidget::init() {
    // Layout widgets
    //
    QGridLayout* layout = new QGridLayout(frame); //frame->setLayout(layout);
    layout->setMargin(0);
    layout->setSpacing(0);
    mGraphicsView = new GV_GraphicsView(this);
    layout->addWidget(mGraphicsView, 0, 0, 1, 1);

    // Initialize drawing (scene)
    //
    RB_MmProxy* mModel = GV_MODELFACTORY->getModel(GV_ModelFactory::ModelDrawing);
//    mModel->setRoot(GV_MODELFACTORY->getRootId());
//    mModel->select();

//    mDrawing = dynamic_cast<GV_Drawing*>(mModel->getObject(mModel->getProxyIndex()));
    mDrawing = new GV_Drawing("test");
    mDrawing->initDrawingArea(true); // with grid = true
    mDrawing->synchronizeSceneData();
//	mDrawing->setDefaultAction(new GV_ActionBase(*mDrawing, *mGraphicsView));
//	mDrawing->setDefaultAction(new GV_ActionSelectSingle(*mDrawing, *mGraphicsView));
    mDrawing->setDefaultAction(new GV_ActionModifyMove(*mDrawing, *mGraphicsView));
//  mDrawing->setContextMenu(mw->sceneContextMenu());
    mGraphicsView->setScene(mDrawing);

    connect(mDrawing,SIGNAL(selectionChanged()),
            this, SLOT(adjustActions()));
    connect(mDrawing, SIGNAL(mousePressed()),
            this, SLOT(slotMousePressed()));
    setWindowModified(false);
    readSettings();
}

/**
 * File Save, can be called by closeWidget()
 */
bool GV_DrawingWidget::fileSave() {
    bool success = false;
    if (GV_MODELFACTORY->getDatabase().isOpen()) {
        mDrawing->synchronizeModelData();
        success = mDrawing->dbUpdate(GV_MODELFACTORY->getDatabase(), RB2::ResolveAll);
    } else if (mFullFileName.isEmpty()) {
        success = fileSaveAs();
    } else {
        success = saveFile(mFullFileName);
    }

    if (success) {
        setWindowModified(false);
    }
    return success;
}

/**
 * File Save As, the DB_ActionFileSaveAs calls saveFile directly
 */
bool GV_DrawingWidget::fileSaveAs() {
    DB_ActionFileSaveAs a;
    a.setWidget(this);
    a.trigger();
    return true;
}

/**
 * Slot File export PDF.
 * @param printer
 */
void GV_DrawingWidget::filePdf(QPrinter* printer) {
    QPainter painter(printer);
    painter.setRenderHint(QPainter::Antialiasing);
    mDrawing->render(&painter);
}

/**
 * Slot File Print.
 * @param printer
 */
void GV_DrawingWidget::filePrint(QPrinter* printer) {
    QPainter painter(printer);
    painter.setRenderHint(QPainter::Antialiasing);
    mDrawing->render(&painter);
}

/**
 * Slot File Print Preview.
 * @param printer
 */
void GV_DrawingWidget::filePrintPreview(QPrinter* printer) {
    printer->setFullPage(true);
    QPainter painter(printer);
    painter.setRenderHint(QPainter::Antialiasing);
    mDrawing->render(&painter, printer->pageRect(),
                     QRect(QPoint(0,0), QPoint(2100, 2970)), Qt::KeepAspectRatio);
}

/**
 * Format Bold
 */
void GV_DrawingWidget::formatBold() {
    if (mDrawing->selectedItems().size() != 1) return;
    QGraphicsItem* item = mDrawing->selectedItems().first();
    if (item->type() != QGraphicsItem::UserType + 5 /* GV_Text */) return;
    QGraphicsTextItem* textItem = dynamic_cast<QGraphicsTextItem*>(item);

    if (textItem->flags() & Qt::TextEditorInteraction) {
        if (!textItem->textCursor().selection().isEmpty()) {
            QTextCharFormat fmt = textItem->textCursor().charFormat();
            fmt.setFontWeight(fmt.fontWeight() == QFont::Normal ? QFont::Bold : QFont::Normal);
            textItem->textCursor().setCharFormat(fmt);
        }
    }
}

/**
 * Format Italic
 */
void GV_DrawingWidget::formatItalic() {
    if (mDrawing->selectedItems().size() != 1) return;
    QGraphicsItem* item = mDrawing->selectedItems().first();
    if (item->type() != QGraphicsItem::UserType + 5 /* GV_Text */) return;
    QGraphicsTextItem* textItem = dynamic_cast<QGraphicsTextItem*>(item);

    if (textItem->flags() & Qt::TextEditorInteraction) {
        if (!textItem->textCursor().selection().isEmpty()) {
            QTextCharFormat fmt = textItem->textCursor().charFormat();
            fmt.setFontItalic(!fmt.fontItalic());
            textItem->textCursor().setCharFormat(fmt);
        }
    }
}

/**
 * Format Underline
 */
void GV_DrawingWidget::formatUnderline() {
    if (mDrawing->selectedItems().size() != 1) return;
    QGraphicsItem* item = mDrawing->selectedItems().first();
    if (item->type() != QGraphicsItem::UserType + 5 /* GV_Text */) return;
    QGraphicsTextItem* textItem = dynamic_cast<QGraphicsTextItem*>(item);

    if (textItem->flags() & Qt::TextEditorInteraction) {
        if (!textItem->textCursor().selection().isEmpty()) {
            QTextCharFormat fmt = textItem->textCursor().charFormat();
            fmt.setFontUnderline(!fmt.fontUnderline());
            textItem->textCursor().setCharFormat(fmt);
        }
    }
}

/**
 * Format Align Left
 */
void GV_DrawingWidget::formatAlignLeft() {
    if (mDrawing->selectedItems().size() != 1) return;
    QGraphicsItem* item = mDrawing->selectedItems().first();
    if (item->type() != QGraphicsItem::UserType + 5 /* GV_Text */) return;
    QGraphicsTextItem* textItem = dynamic_cast<QGraphicsTextItem*>(item);

    if (textItem->flags() & Qt::TextEditorInteraction) {
        textItem->textCursor().select(QTextCursor::BlockUnderCursor);
        QTextBlockFormat fmt = textItem->textCursor().blockFormat();
        fmt.setAlignment(Qt::AlignLeft);
        textItem->textCursor().setBlockFormat(fmt);
    }
}

/**
 * Format Align Center
 */
void GV_DrawingWidget::formatAlignCenter() {
    if (mDrawing->selectedItems().size() != 1) return;
    QGraphicsItem* item = mDrawing->selectedItems().first();
    if (item->type() != QGraphicsItem::UserType + 5) return;
    QGraphicsTextItem* textItem = dynamic_cast<QGraphicsTextItem*>(item);

    if (textItem->flags() & Qt::TextEditorInteraction) {
        textItem->textCursor().select(QTextCursor::BlockUnderCursor);
        QTextBlockFormat fmt = textItem->textCursor().blockFormat();
        fmt.setAlignment(fmt.alignment() == Qt::AlignHCenter ? Qt::AlignLeft : Qt::AlignHCenter);
        textItem->textCursor().setBlockFormat(fmt);
    }
}

/**
 * Format Align Right
 */
void GV_DrawingWidget::formatAlignRight() {
    if (mDrawing->selectedItems().size() != 1) return;
    QGraphicsItem* item = mDrawing->selectedItems().first();
    if (item->type() != QGraphicsItem::UserType + 5) return;
    QGraphicsTextItem* textItem = dynamic_cast<QGraphicsTextItem*>(item);

    if (textItem->flags() & Qt::TextEditorInteraction) {
        textItem->textCursor().select(QTextCursor::BlockUnderCursor);
        QTextBlockFormat fmt = textItem->textCursor().blockFormat();
        fmt.setAlignment(fmt.alignment() == Qt::AlignRight ? Qt::AlignLeft : Qt::AlignRight);
        textItem->textCursor().setBlockFormat(fmt);
    }
}

/**
 * Format Font Name
 */
void GV_DrawingWidget::formatFontAll() {
    if (mDrawing->selectedItems().size() != 1) return;
    QGraphicsItem* item = mDrawing->selectedItems().first();
    if (item->type() != QGraphicsItem::UserType + 5) return;
    QGraphicsTextItem* textItem = dynamic_cast<QGraphicsTextItem*>(item);

    if ((textItem->flags() & Qt::TextEditorInteraction)
                && !textItem->textCursor().selection().isEmpty()) {

        QFont font = textItem->textCursor().charFormat().font();
        bool ok = false;
        QFont newFont = QFontDialog::getFont(&ok, font, this);

        if (ok) {
            QTextCharFormat fmt;
            fmt.setFont(newFont);
            textItem->textCursor().mergeCharFormat(fmt);
        }
    } else {
        QFont font = textItem->font();
        bool ok = false;
        QFont newFont = QFontDialog::getFont(&ok, font, this);

        if (ok) {
            textItem->setFont(newFont);
        }
    }
}

/**
 * Format Font Name
 */
void GV_DrawingWidget::formatFontName() {
    if (mDrawing->selectedItems().size() != 1) return;
    QGraphicsItem* item = mDrawing->selectedItems().first();
    if (item->type() != QGraphicsItem::UserType + 5) return;
    QGraphicsTextItem* textItem = dynamic_cast<QGraphicsTextItem*>(item);

    if ((textItem->flags() & Qt::TextEditorInteraction)
                && !textItem->textCursor().selection().isEmpty()) {
        QString family = textItem->textCursor().charFormat().fontFamily();
        QStringList families = QFontDatabase().families();
        int current = families.indexOf(family, 0);
        bool success = false;
        QString newFamily = QInputDialog::getItem(this, tr("Font"), tr("Select font:"),
                                               families, current, false, &success);
        if (success) {
            QTextCharFormat fmt = textItem->textCursor().charFormat();
            fmt.setFontFamily(newFamily);
            textItem->textCursor().setCharFormat(fmt);
        }
    }
}

/**
 * Format Font Size
 */
void GV_DrawingWidget::formatFontSize() {
    if (mDrawing->selectedItems().size() != 1) return;
    QGraphicsItem* item = mDrawing->selectedItems().first();
    if (item->type() != QGraphicsItem::UserType + 5) return;
    QGraphicsTextItem* textItem = dynamic_cast<QGraphicsTextItem*>(item);

    if ((textItem->flags() & Qt::TextEditorInteraction)
                && !textItem->textCursor().selection().isEmpty()) {
        int size = (int)textItem->textCursor().charFormat().fontPointSize();
        if (size < 1) size = 12; // default text size
        QList<int> sizes = QFontDatabase::standardSizes();
        bool success = false;
        int newSize = QInputDialog::getInt(this, tr("Font Size"), tr("Select font size:"),
                                                size, sizes.first(), sizes.last(), 1, &success);
        if (success) {
            QTextCharFormat fmt = textItem->textCursor().charFormat();
            fmt.setFontPointSize(newSize);
            textItem->textCursor().setCharFormat(fmt);
        }
    }
}

/**
 * Format Text Color
 */
void GV_DrawingWidget::formatTextColor() {
    if (mDrawing->selectedItems().size() != 1) return;
    QGraphicsItem* item = mDrawing->selectedItems().first();
    if (item->type() != QGraphicsItem::UserType + 5) return;
    QGraphicsTextItem* textItem = dynamic_cast<QGraphicsTextItem*>(item);

    if ((textItem->flags() & Qt::TextEditorInteraction)
                && !textItem->textCursor().selection().isEmpty()) {
        QColor color = textItem->textCursor().charFormat().foreground().color();
        QColor newColor = QColorDialog::getColor(color, this);
        if (newColor.isValid()) {
            QTextCharFormat fmt = textItem->textCursor().charFormat();
            fmt.setForeground(QBrush(newColor));
            textItem->textCursor().setCharFormat(fmt);
        }
    } else {
        QColor color = textItem->defaultTextColor();
        QColor newColor = QColorDialog::getColor(color, this);
        if (newColor.isValid()) {
            textItem->setDefaultTextColor(newColor);
        }
    }
}

/**
 * Remove formatting and style
 */
void GV_DrawingWidget::formatRemove() {
//    webView->triggerPageAction(QWebPage::RemoveFormat);
}

/**
 * Load file
 * @param fn filename
 * @return true at success
 */
bool GV_DrawingWidget::loadFile(const QString& fn) {
    // Check if file can be read
    QFile file(fn);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        DB_DIALOGFACTORY->requestWarningDialog(
            tr("Cannot read file %1:\n%2.").arg(fn).arg(file.errorString()));
        return false;
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);

    // example from GV_DrawingWidget
//    QByteArray data = file.readAll();
//    webView->setContent(data, "text/html");
//    webView->page()->setContentEditable(true);
//    webView->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);

    QApplication::restoreOverrideCursor();

    setCurrentFileName(fn);
    return true;
}

/**
 * Save file
 * @param fn filename
 * @return true if success
 */
bool GV_DrawingWidget::saveFile(const RB_String &fn) {
    QFile file(fn);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        DB_DIALOGFACTORY->requestWarningDialog(
            tr("Cannot write file %1:\n%2.").arg(fn).arg(file.errorString()));
        DB_DIALOGFACTORY->statusBarMessage(tr("File save ERROR"), 2000);
        return false;
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);

    // Temporarily add drawing to project drawing list for XML export
    RB_ObjectBase* project = GV_MODELFACTORY->getRoot();
    RB_ObjectContainer* drawingList = project->getContainer("GV_DrawingList");
    drawingList->addObject(mDrawing);

    mDrawing->synchronizeModelData();
    GV_FilterXml* filter = new GV_FilterXml();
//    filter->fileExport(GV_MODELFACTORY, fn); only now because filter is to be updated

    // Remove from list
    drawingList->remove(mDrawing, false);

    QApplication::restoreOverrideCursor();

    setCurrentFileName(fn);
    DB_DIALOGFACTORY->statusBarMessage(tr("File saved"), 2000);
    emit modificationChanged(false);
    return true;
}

/**
 * Set current file name
 */
void GV_DrawingWidget::setCurrentFileName(const RB_String& fileName) {
    mFullFileName = QFileInfo(fileName).canonicalFilePath();
}

bool GV_DrawingWidget::closeWidget() {
    if (maybeSave()) {
//        writeSettings();
        emit modificationChanged(false);
        return true;
    }

    return false;
}

/**
 * Notify user of unsaved data and ask whether data should be saved.
 */
bool GV_DrawingWidget::maybeSave() {
    if (isWindowModified()/*mDrawing->isDrawingChanged()*/) {
        int ret = DB_DIALOGFACTORY->requestYesNoCancelDialog(
                tr("Document changed ..."),
                tr("The document has been modified.\n"
                   "Do you want to keep your changes?"));
        if (ret == QMessageBox::Yes) {
            if (!fileSave()) { // if saved message by action
                DB_DIALOGFACTORY->statusBarMessage(tr("File not saved"), 2000);
            } else {
                DB_DIALOGFACTORY->statusBarMessage(tr("File saved"), 2000);
//                mDrawing->setDrawingChanged(false);
                setWindowModified(false);
            }
        } else if (ret == QMessageBox::No) {
            DB_DIALOGFACTORY->statusBarMessage(tr("File not saved"), 2000);
        } else if (ret == QMessageBox::Cancel) {
            return false;
        }
    }

    return true;
}

/**
 * Adjust text item actions
 */
void GV_DrawingWidget::adjustActions() {
    if (mDrawing->selectedItems().size() != 1) {
        DB_ACTIONFACTORY->setFormatAlignCenter(false);
        DB_ACTIONFACTORY->setFormatAlignLeft(false);
        DB_ACTIONFACTORY->setFormatAlignRight(false);
        DB_ACTIONFACTORY->setFormatBoldChecked(false);
        DB_ACTIONFACTORY->setFormatItalicChecked(false);
        DB_ACTIONFACTORY->setFormatUnderlineChecked(false);
    } else {
        QGraphicsItem* item = mDrawing->selectedItems().first();
        if (item->type() == QGraphicsItem::UserType + 5 /* GV_Text */) {
            QGraphicsTextItem* textItem = dynamic_cast<QGraphicsTextItem*>(item);

            if (textItem->flags() & Qt::TextEditorInteraction) {
                QTextBlockFormat bfmt = textItem->textCursor().blockFormat();
                DB_ACTIONFACTORY->setFormatAlignCenter(bfmt.alignment() == Qt::AlignHCenter);
                DB_ACTIONFACTORY->setFormatAlignLeft(bfmt.alignment() == Qt::AlignLeft);
                DB_ACTIONFACTORY->setFormatAlignRight(bfmt.alignment() == Qt::AlignRight);
                QTextCharFormat cfmt = textItem->textCursor().charFormat();
                DB_ACTIONFACTORY->setFormatBoldChecked(cfmt.fontWeight() == QFont::Bold);
                DB_ACTIONFACTORY->setFormatItalicChecked(cfmt.fontItalic());
                DB_ACTIONFACTORY->setFormatUnderlineChecked(cfmt.fontUnderline());
            }
        }
    }
}

/**
 * Slot to transfer drawing changed state to update window title with asterisk
 */
void GV_DrawingWidget::slotMousePressed() {
    setWindowModified(true);
}

/**
 * Change event, for example translation
 */
void GV_DrawingWidget::changeEvent(QEvent *e) {
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}
