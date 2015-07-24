/*****************************************************************
 * $Id: crm_templatewidget.cpp 2121 2014-03-06 11:23:12Z rutger $
 * Created: Mar 06, 2012 3:14:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "crm_templatewidget.h"

#include "crm_dialogfactory.h"
#include "crm_modelfactory.h"
#include "db_actionfactory.h"
#include "db_dialogfactory.h"
#include "rb_datawidgetmapper.h"
#include "rb_mdiwindow.h"
#include "rb_sqlrelationaldelegate.h"
#include "rb_textpreview.h"


/**
 * Constructor
 */
CRM_TemplateWidget::CRM_TemplateWidget(QWidget* parent)
                                    : RB_Widget(parent) {
    setupUi(this);

    mTextPreview = new RB_TextPreview(this);
    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(mTextPreview);

    mModel = NULL;
    mMapper = NULL;
}

/**
 * Destructor
 */
CRM_TemplateWidget::~CRM_TemplateWidget() {
    delete mModel; // also mMapper is deleted
    RB_DEBUG->print("CRM_TemplateWidget::~CRM_TemplateWidget() OK");
}

/**
 * Get proposed saveas filename
 * @returns filename
 */
RB_String CRM_TemplateWidget::getSaveAsFileName() {
    if (!mModel->getCurrentIndex().isValid()) {
        return QString();
    }

    RB_String str = mModel->getCurrentValue("reference").toString();
    return str;
}

/**
 * Initialize widget and models
 */
void CRM_TemplateWidget::init() {

    DB_ACTIONFACTORY->enableEditMenu(getWidgetType());
    DB_ACTIONFACTORY->enableFormatMenu(getWidgetType());

    //
    // 1. Set model for customer mapper
    //
    mModel = CRM_MODELFACTORY->getModel(CRM_ModelFactory::ModelTemplate); // shared
    mModel->setRoot(CRM_MODELFACTORY->getRootId());

    //
    // 2. Get mapper for line edits etc.
    //
    mMapper = mModel->getMapper();

    RB_StringList items;
    items << tr("Email") << tr("Mail");
    cbType->addItems(items);
    mMapper->addMapping(cbType, mModel->fieldIndex("type_id"),
                        "currentIndex");
    mMapper->addMapping(leReference, mModel->fieldIndex("reference"));
    mMapper->addMapping(chbStandard, mModel->fieldIndex("standard"));
    mMapper->addMapping(leDescription, mModel->fieldIndex("description"));
    mMapper->addMapping(dteLastUsed, mModel->fieldIndex("lastused"));
    mMapper->addMapping(tePageHeader, mModel->fieldIndex("pageheader"));
    mMapper->addMapping(teDocContent, mModel->fieldIndex("doccontent"));
    mMapper->addMapping(tePageFooter, mModel->fieldIndex("pagefooter"));

    cbPaperSize->addItems(mTextPreview->getPaperSizeList());
    mMapper->addMapping(cbPaperSize, mModel->fieldIndex("papersize"), "currentIndex");

    mMapper->addMapping(leLeft, mModel->fieldIndex("marginleft"));
    mMapper->addMapping(leRight, mModel->fieldIndex("marginright"));
    mMapper->addMapping(leTop, mModel->fieldIndex("margintop"));
    mMapper->addMapping(leBottom, mModel->fieldIndex("marginbottom"));

    mMapper->addMapping(lePageHeaderHeight, mModel->fieldIndex("pageheaderheight"));
    mMapper->addMapping(lePageFooterHeight, mModel->fieldIndex("pagefooterheight"));

    //
    // 3. Select after relations have been set, only for database models
    //
    mModel->select();

    //
    // 4. Connect model to main view
    //
    readSettings();
    setFormatTableView(tvTemplate, mModel);
    dteLastUsed->setDisplayFormat("yyyy-MM-dd");

    int count = mModel->columnCount(QModelIndex());

    for (int i = 0; i < count; ++i) {
        if (i != mModel->fieldIndex("type_id")
                && i != mModel->fieldIndex("reference")
                && i != mModel->fieldIndex("description")
                && i != mModel->fieldIndex("lastused")) {
            tvTemplate->hideColumn(i);
        } else {
            tvTemplate->showColumn(i);

            if (tvTemplate->columnWidth(i) < 5) {
                tvTemplate->setColumnWidth(i, 100);
            }
        }
    }

    connect(tabWidget, SIGNAL(currentChanged(int)),
            this, SLOT(slotTabChanged(int)));
    connect(mModel, SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            this, SLOT(slotCurrentItemChanged(QModelIndex,QModelIndex)));
}

/**
 * Save data to database
 */
bool CRM_TemplateWidget::fileSave(bool withSelect) {
    QApplication::setOverrideCursor(Qt::WaitCursor);
    // HACK to emit editingFinished() for current QLineEdit:
    beforeFileSave();

    if (withSelect && mModel) {
        if (mModel->submitAllAndSelect()) {

            setWindowModified(false);
            QApplication::restoreOverrideCursor();
            return true;
        }
    } else if (mModel) {
        if (mModel->submitAll()) {
            setWindowModified(false);
            QApplication::restoreOverrideCursor();
            return true;
        }
    }
    QApplication::restoreOverrideCursor();
    return false;
}

/**
 * File menu PDF clicked
 * @param printer
 */
void CRM_TemplateWidget::filePdf(QPrinter* printer) {
    slotTabChanged(2); // refresh preview
    mTextPreview->print(printer);
}

/**
 * File menu print clicked
 * @param printer
 */
void CRM_TemplateWidget::filePrint(QPrinter* printer) {
    slotTabChanged(2); // refresh preview
    mTextPreview->print(printer);
}

/**
 * File menu print preview clicked
 * @param printer
 */
void CRM_TemplateWidget::filePrintPreview(QPrinter* printer) {
    slotTabChanged(2); // refresh preview
    mTextPreview->print(printer);
}

/**
 * Edit Cut.
 */
void CRM_TemplateWidget::editCut() {
    if (getTextEdit()) {
        getTextEdit()->cut();
    }
}

/**
 * Edit Copy.
 */
void CRM_TemplateWidget::editCopy() {
    if (getTextEdit()) {
        getTextEdit()->copy();
    }
}

/**
 * Edit Paste.
 */
void CRM_TemplateWidget::editPaste() {
    if (getTextEdit()) {
        getTextEdit()->paste();
    }
}

/**
 * Edit Undo.
 */
void CRM_TemplateWidget::editUndo() {
    if (getTextEdit()) {
        getTextEdit()->undo();
    }
}

/**
 * Edit Redo.
 */
void CRM_TemplateWidget::editRedo() {
    if (getTextEdit()) {
        getTextEdit()->redo();
    }
}

/**
 * Select All.
 */
void CRM_TemplateWidget::editSelectAll() {
    if (getTextEdit()) {
        getTextEdit()->selectAll();
    }
}

/**
 * Format Bold
 */
void CRM_TemplateWidget::formatBold() {
    if (getTextEdit()) {
        getTextEdit()->slotTextBold();
    }
}

/**
 * Format Italic
 */
void CRM_TemplateWidget::formatItalic() {
    if (getTextEdit()) {
        getTextEdit()->slotTextItalic();
    }
}

/**
 * Format Underline
 */
void CRM_TemplateWidget::formatUnderline() {
    if (getTextEdit()) {
        getTextEdit()->slotTextUnderline();
    }
}

/**
 * Format Strikethrough
 */
void CRM_TemplateWidget::formatStrikethrough() {
    if (getTextEdit()) {
        getTextEdit()->slotTextStrikeOut();
    }
}

/**
 * Format Align Left
 */
void CRM_TemplateWidget::formatAlignLeft() {
    if (getTextEdit()) {
        getTextEdit()->slotTextAlign(RB_TextEdit::AlignLeft);
    }
}

/**
 * Format Align Center
 */
void CRM_TemplateWidget::formatAlignCenter() {
    if (getTextEdit()) {
        getTextEdit()->slotTextAlign(RB_TextEdit::AlignCenter);
    }
}

/**
 * Format Align Right
 */
void CRM_TemplateWidget::formatAlignRight() {
    if (getTextEdit()) {
        getTextEdit()->slotTextAlign(RB_TextEdit::AlignRight);
    }
}

/**
 * Format Align Justify
 */
void CRM_TemplateWidget::formatAlignJustify() {
    if (getTextEdit()) {
        getTextEdit()->slotTextAlign(RB_TextEdit::AlignJustify);
    }
}

/**
 * Format Increase Indent
 */
void CRM_TemplateWidget::formatIncreaseIndent() {
    if (getTextEdit()) {
        getTextEdit()->slotTextIndent(1);
    }
}

/**
 * Format Decrease Indent
 */
void CRM_TemplateWidget::formatDecreaseIndent() {
    if (getTextEdit()) {
        getTextEdit()->slotTextIndent(-1);
    }
}

/**
 * Format numbered list
 */
void CRM_TemplateWidget::formatNumberedList() {
    if (getTextEdit()) {
        getTextEdit()->slotTextStyle(RB_TextEdit::ListDecimal);
    }
}

/**
 * Format bulleted list
 */
void CRM_TemplateWidget::formatBulletedList() {
    if (getTextEdit()) {
        getTextEdit()->slotTextStyle(RB_TextEdit::ListDisc);
    }
}

/**
 * Format font family
 */
void CRM_TemplateWidget::formatFontName() {
    if (getTextEdit()) {
        getTextEdit()->slotTextFamily();
        // Used for painting
        getTextEdit()->document()->setDefaultFont(
                    getTextEdit()->textCursor().charFormat().font());
    }
}

/**
 * Format font size
 */
void CRM_TemplateWidget::formatFontSize() {
    if (getTextEdit()) {
        getTextEdit()->slotTextSize();
        // Used for painting
        getTextEdit()->document()->setDefaultFont(
                    getTextEdit()->textCursor().charFormat().font());
    }
}

/**
 * Format text color
 */
void CRM_TemplateWidget::formatTextColor() {
    if (getTextEdit()) {
        getTextEdit()->slotTextColor();
    }
}

/**
 * Format background color
 */
void CRM_TemplateWidget::formatBackgroundColor() {
    if (getTextEdit()) {
        getTextEdit()->slotBackgroundColor();
    }
}

/**
 * Remove formatting and style
 */
void CRM_TemplateWidget::formatRemove() {
//    webView->triggerPageAction(QWebPage::RemoveFormat);
}

/**
 * Button add clicked
 */
void CRM_TemplateWidget::on_pbAdd_clicked() {
    if (!mModel) return;
    mModel->undoFilterSort();

    // always insert at the begin
    int row = 0; // mModel->rowCount();
    mModel->insertRows(row, 1, QModelIndex());

    // NOTE: do not forget to set the default column values, specially for the
    //       relational table otherwise new row will not show!
    QModelIndex idx;
    idx = mModel->index(row, mModel->fieldIndex("type_id"));
    mModel->setData(idx, 0, Qt::EditRole);
    idx = mModel->index(row, mModel->fieldIndex("lastused"));
    mModel->setData(idx, QDateTime::currentDateTime(), Qt::EditRole);
    idx = mModel->index(row, mModel->fieldIndex("papersize"));
    mModel->setData(idx, 4, Qt::EditRole); // A4

    idx = mModel->index(row, mModel->fieldIndex("marginleft"));
    mModel->setData(idx, 15.0, Qt::EditRole);
    idx = mModel->index(row, mModel->fieldIndex("marginright"));
    mModel->setData(idx, 10.0, Qt::EditRole);
    idx = mModel->index(row, mModel->fieldIndex("margintop"));
    mModel->setData(idx, 10.0, Qt::EditRole);
    idx = mModel->index(row, mModel->fieldIndex("marginbottom"));
    mModel->setData(idx, 10.0, Qt::EditRole);

    idx = mModel->index(row, mModel->fieldIndex("pageheaderheight"));
    mModel->setData(idx, 20.0, Qt::EditRole);
    idx = mModel->index(row, mModel->fieldIndex("pagefooterheight"));
    mModel->setData(idx, 20.0, Qt::EditRole);
    // end NOTE

    tvTemplate->setCurrentIndex(mModel->index(row, RB2::HIDDENCOLUMNS, QModelIndex()));
    tvTemplate->scrollTo(tvTemplate->currentIndex());
    tabWidget->setCurrentIndex(0);
    cbType->setFocus();
}

/**
 * Button delete clicked
 */
void CRM_TemplateWidget::on_pbDelete_clicked() {
    int ret = CRM_DIALOGFACTORY->requestYesNoDialog(
            tr("Delete document ..."),
            tr("This action cannot be undone.\n"
               "Do you want to delete this document?"));
    if (ret == QMessageBox::No) {
        CRM_DIALOGFACTORY->statusBarMessage(tr("Deletion canceled"), 2000);
        return;
    }

    QModelIndex index = tvTemplate->currentIndex();
    if (!index.isValid()) return;

    mModel->removeRows(index.row(), 1, QModelIndex());
    fileSave(true);
}

/**
 * Push button save clicked
 */
void CRM_TemplateWidget::on_pbSave_clicked() {
    fileSave(true);
}

void CRM_TemplateWidget::on_pbFilterOn_clicked() {
    RB_String str = leFilter->text();
    if (str.isEmpty()) {
        return;
    }
    if (!str.endsWith("%")) {
        str += "%";
    }
    RB_String filter = "`crm_template`.`description` LIKE '";
    filter += str + "'";
    mModel->setWhere(filter);
}

void CRM_TemplateWidget::on_pbFilterOff_clicked() {
    mModel->setWhere("");
    leFilter->setText("");
}

/**
 * User clicked on tabWidget, tab preview will refresh preview
 * @param index current selected tab
 */
void CRM_TemplateWidget::slotTabChanged(int index) {
    if (index == 2) {
        // 2 is preview
        QApplication::setOverrideCursor(Qt::WaitCursor);

        // Set margins, paper size and update()
        mTextPreview->slotSetPaperSize(
                    mTextPreview->getPaperSizeEnum(cbPaperSize->currentText()));
        mTextPreview->setPaperMargin(leLeft->text().toDouble(),
                                     leRight->text().toDouble(),
                                     leTop->text().toDouble(),
                                     leBottom->text().toDouble());
        mTextPreview->setPageHeader(tePageHeader->document(),
                                    lePageHeaderHeight->text().toDouble());
        mTextPreview->setDocContent(teDocContent->document());
        mTextPreview->setPageFooter(tePageFooter->document(),
                                    lePageFooterHeight->text().toDouble());
        mTextPreview->update();

        QApplication::restoreOverrideCursor();
    }

//    setPageSize(mTextPreview->getPaperSizeEnum(cbPaperSize->currentText()));
//    setPageMargins(leLeft->text().toDouble(),
//                   leRight->text().toDouble(),
//                   leTop->text().toDouble(),
//                   leBottom->text().toDouble());
}



/**
 * Slot current item in document table changed, to update the reference
 * lineEdits.
 * @param curr current index
 * @param prev previous index
 */
void CRM_TemplateWidget::slotCurrentItemChanged(const QModelIndex& curr,
                                                const QModelIndex& /*prev*/) {
    if (!curr.isValid()) {
        leReferenceText->setText("");
        leReferencePreview->setText("");
    } else {
        leReferenceText->setText(mModel->getCurrentValue("reference").toString());
        leReferencePreview->setText(mModel->getCurrentValue("reference").toString());
    }
}

/**
 * @returns textEdit that has focus or NULL
 */
RB_TextEdit *CRM_TemplateWidget::getTextEdit() {
    if (tePageHeader->hasFocus()) {
        return tePageHeader;
    } else if (teDocContent->hasFocus()) {
        return teDocContent;
    } else if (tePageFooter->hasFocus()) {
        return tePageFooter;
    }

    DB_DIALOGFACTORY->commandMessage("No text edit selected.");
    return NULL;
}

/**
 * Get help subject
 * @return help subject (name of widget)
 */
RB_String CRM_TemplateWidget::getHelpSubject() const {
    return objectName();
}

/**
 * Set printer properties such as page size and margins
 */
void CRM_TemplateWidget::setPrinterProperties(QPrinter* pr) {
    const bool oldFullPage = pr->fullPage();
    pr->setFullPage(true);
    pr->setPaperSize(mTextPreview->getPaperSizeEnum(cbPaperSize->currentText()));
    pr->setOrientation(QPrinter::Portrait);
    pr->setPageMargins(leLeft->text().toDouble(),
                       leRight->text().toDouble(),
                       leTop->text().toDouble(),
                       leBottom->text().toDouble(), QPrinter::Millimeter);

    pr->setFullPage(oldFullPage);
}

/**
 * Close widget
 */
bool CRM_TemplateWidget::closeWidget() {
    RB_Widget::closeWidget();
    if (maybeSave()) {
        return true;
    }
    return false;
}

/**
 * Change event, for example translation
 */
void CRM_TemplateWidget::changeEvent(QEvent *e) {
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}

