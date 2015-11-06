/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Qt Software Information (qt-info@nokia.com)
**
** This file is part of the Graphics Dojo project on Qt Labs.
**
** This file may be used under the terms of the GNU General Public
** License version 2.0 or 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of
** this file.  Please review the following information to ensure GNU
** General Public Licensing requirements will be met:
** http://www.fsf.org/licensing/licenses/info/GPLv2.html and
** http://www.gnu.org/copyleft/gpl.html.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/


#include "db_htmleditorwidget.h"

// #include "ui_htmleditor.h"
// #include "ui_inserthtmldialog.h"

#include <QtWidgets>
#include <QtWebEngineWidgets>
#include "db_actionfactory.h"
#include "db_actionfilesaveas.h"
#include "db_dialogfactory.h"
#include "db_htmlhighlighter.h"
#include "rb_mdiwindow.h"

/*
#define FORWARD_ACTION(action1, action2) \
    connect(action1, SIGNAL(triggered()), \
            ui->webView->pageAction(action2), SLOT(trigger())); \
    connect(ui->webView->pageAction(action2), \
            SIGNAL(changed()), SLOT(adjustActions()));
*/

DB_HtmlEditorWidget::DB_HtmlEditorWidget(QWidget *parent) : RB_Widget(parent)
        , mSourceDirty(false)
        , mCodeHighlighter(0)
/*        , ui_dialog(0)  */
        , mInsertHtmlDialog(0)
{

    setupUi(this);

    webView->setGridLayout(gridLayoutView);
    tabWidget->setTabText(0, "View");
    tabWidget->setTabText(1, "Source");
    plainTextEdit->setTabStopWidth(20); // in pixels, default is 80

    connect(webView, SIGNAL(keyWasReleasedEvent()),
            this, SLOT(slotContentChanged()));
    connect(plainTextEdit, SIGNAL(textChanged()),
            this, SLOT(slotContentChanged()));
    connect(tabWidget, SIGNAL(currentChanged(int)),
            this, SLOT(changeTab(int)));
//    connect(webView, SIGNAL(linkClicked(QUrl)),
//            this, SLOT(openLink(QUrl)));
    connect(webView->page(), SIGNAL(selectionChanged()),
            this, SLOT(adjustActions()));
//    connect(webView, SIGNAL(linkClicked(QUrl)),
//            this, SLOT(slotLoadUrl(QUrl)));


    mCodeHighlighter = new DB_HtmlHighlighter(plainTextEdit->document());
    mVerticalScrollbarPerunage = 0.0;

    QtWebEngine::initialize();
    mResult = new QVariant();

/*    QWidget *spacer = new QWidget(this);
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    ui->standardToolBar->insertWidget(ui->actionZoomOut, spacer);

    zoomLabel = new QLabel;
    ui->standardToolBar->insertWidget(ui->actionZoomOut, zoomLabel);

    zoomSlider = new QSlider(this);
    zoomSlider->setOrientation(Qt::Horizontal);
    zoomSlider->setMaximumWidth(150);
    zoomSlider->setRange(25, 400);
    zoomSlider->setSingleStep(25);
    zoomSlider->setPageStep(100);
    connect(zoomSlider, SIGNAL(valueChanged(int)), SLOT(changeZoom(int)));
    ui->standardToolBar->insertWidget(ui->actionZoomIn, zoomSlider);

    connect(ui->actionFileNew, SIGNAL(triggered()), SLOT(fileNew()));
    connect(ui->actionFileOpen, SIGNAL(triggered()), SLOT(fileOpen()));
    connect(ui->actionFileSave, SIGNAL(triggered()), SLOT(fileSave()));
    connect(ui->actionFileSaveAs, SIGNAL(triggered()), SLOT(fileSaveAs()));
    connect(ui->actionExit, SIGNAL(triggered()), SLOT(close()));
    connect(ui->actionInsertImage, SIGNAL(triggered()), SLOT(insertImage()));
    connect(ui->actionCreateLink, SIGNAL(triggered()), SLOT(createLink()));
    connect(ui->actionInsertHtml, SIGNAL(triggered()), SLOT(insertHtml()));
    connect(ui->actionZoomOut, SIGNAL(triggered()), SLOT(zoomOut()));
    connect(ui->actionZoomIn, SIGNAL(triggered()), SLOT(zoomIn()));

    // these are forward to internal QWebEngineView
    FORWARD_ACTION(ui->actionEditUndo, QWebPage::Undo);
    FORWARD_ACTION(ui->actionEditRedo, QWebPage::Redo);
    FORWARD_ACTION(ui->actionEditCut, QWebPage::Cut);
    FORWARD_ACTION(ui->actionEditCopy, QWebPage::Copy);
    FORWARD_ACTION(ui->actionEditPaste, QWebPage::Paste);
    FORWARD_ACTION(ui->actionFormatBold, QWebPage::ToggleBold);
    FORWARD_ACTION(ui->actionFormatItalic, QWebPage::ToggleItalic);
    FORWARD_ACTION(ui->actionFormatUnderline, QWebPage::ToggleUnderline);

    // Qt 4.5.0 has a bug: always returns 0 for QWebPage::SelectAll
    connect(ui->actionEditSelectAll, SIGNAL(triggered()), SLOT(editSelectAll()));

    connect(ui->actionStyleParagraph, SIGNAL(triggered()), SLOT(styleParagraph()));
    connect(ui->actionStyleHeading1, SIGNAL(triggered()), SLOT(styleHeading1()));
    connect(ui->actionStyleHeading2, SIGNAL(triggered()), SLOT(styleHeading2()));
    connect(ui->actionStyleHeading3, SIGNAL(triggered()), SLOT(styleHeading3()));
    connect(ui->actionStyleHeading4, SIGNAL(triggered()), SLOT(styleHeading4()));
    connect(ui->actionStyleHeading5, SIGNAL(triggered()), SLOT(styleHeading5()));
    connect(ui->actionStyleHeading6, SIGNAL(triggered()), SLOT(styleHeading6()));
    connect(ui->actionStylePreformatted, SIGNAL(triggered()), SLOT(stylePreformatted()));
    connect(ui->actionStyleAddress, SIGNAL(triggered()), SLOT(styleAddress()));
    connect(ui->actionFormatFontName, SIGNAL(triggered()), SLOT(formatFontName()));
    connect(ui->actionFormatFontSize, SIGNAL(triggered()), SLOT(formatFontSize()));
     connect(ui->actionFormatTextColor, SIGNAL(triggered()), SLOT(formatTextColor()));
    connect(ui->actionFormatBackgroundColor, SIGNAL(triggered()), SLOT(formatBackgroundColor()));

    // no page action exists yet for these, so use execCommand trick
    connect(ui->actionFormatStrikethrough, SIGNAL(triggered()), SLOT(formatStrikeThrough()));
    connect(ui->actionFormatAlignLeft, SIGNAL(triggered()), SLOT(formatAlignLeft()));
    connect(ui->actionFormatAlignCenter, SIGNAL(triggered()), SLOT(formatAlignCenter()));
    connect(ui->actionFormatAlignRight, SIGNAL(triggered()), SLOT(formatAlignRight()));
    connect(ui->actionFormatAlignJustify, SIGNAL(triggered()), SLOT(formatAlignJustify()));
    connect(ui->actionFormatDecreaseIndent, SIGNAL(triggered()), SLOT(formatDecreaseIndent()));
    connect(ui->actionFormatIncreaseIndent, SIGNAL(triggered()), SLOT(formatIncreaseIndent()));
    connect(ui->actionFormatNumberedList, SIGNAL(triggered()), SLOT(formatNumberedList()));
    connect(ui->actionFormatBulletedList, SIGNAL(triggered()), SLOT(formatBulletedList()));


    // necessary to sync our actions
    connect(ui->webView->page(), SIGNAL(selectionChanged()), SLOT(adjustActions()));

    connect(ui->webView->page(), SIGNAL(contentsChanged()), SLOT(adjustSource()));
    ui->webView->setFocus();

    setCurrentFileName(QString());

    QString initialFile = ":/example.html";
    const QStringList args = QCoreApplication::arguments();
    if (args.count() == 2)
        initialFile = args.at(1);

    if (!load(initialFile))
        fileNew();

    adjustActions();
    adjustSource();
    setWindowModified(false);
    changeZoom(100);
*/
}

DB_HtmlEditorWidget::~DB_HtmlEditorWidget() {
//    delete ui;
//    delete ui_dialog;
    delete mCodeHighlighter;
    delete mResult;
}

/**
 * Get name for MDI window title
 * @return file name or type of window (in case of unique window)
 */
RB_String DB_HtmlEditorWidget::getName() const {
    if (mFullFileName.isEmpty()) {
        return getId();
    }
    return QFileInfo(mFullFileName).fileName();
}

/**
 * @returns plain text edit or the webview depending the tab view/source
 */
QWidget* DB_HtmlEditorWidget::getMainWidget() {
    if (tabWidget->currentIndex() == 0) {
        return webView;
    }

    return plainTextEdit;
}

void DB_HtmlEditorWidget::init() {
    webView->page()->runJavaScript("document.documentElement.contentEditable = true");
//    webView->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);

    DB_ACTIONFACTORY->enableEditMenu(getWidgetType());
    DB_ACTIONFACTORY->enableFormatMenu(getWidgetType());
    readSettings();
    webView->setFocus();
}

/*
bool DB_HtmlEditorWidget::maybeSave()
{
    if (!isWindowModified())
        return true;

    QMessageBox::StandardButton ret;
    ret = QMessageBox::warning(this, tr("HTML Editor"),
                               tr("The document has been modified.\n"
                                  "Do you want to save your changes?"),
                               QMessageBox::Save | QMessageBox::Discard
                               | QMessageBox::Cancel);
    if (ret == QMessageBox::Save)
        return fileSave();
    else if (ret == QMessageBox::Cancel)
        return false;
    return true;
}

void DB_HtmlEditorWidget::fileNew()
{
    if (maybeSave()) {
        ui->webView->setHtml("<p></p>");
        ui->webView->setFocus();
        ui->webView->page()->setContentEditable(true);
        setCurrentFileName(QString());
        setWindowModified(false);

        // quirk in QWebEngineView: need an initial mouse click to show the cursor
        int mx = ui->webView->width() / 2;
        int my = ui->webView->height() / 2;
        QPoint center = QPoint(mx, my);
        QMouseEvent *e1 = new QMouseEvent(QEvent::MouseButtonPress, center,
                                          Qt::LeftButton, Qt::LeftButton,
                                          Qt::NoModifier);
        QMouseEvent *e2 = new QMouseEvent(QEvent::MouseButtonRelease, center,
                                          Qt::LeftButton, Qt::LeftButton,
                                          Qt::NoModifier);
        QApplication::postEvent(ui->webView, e1);
        QApplication::postEvent(ui->webView, e2);
    }
}

void DB_HtmlEditorWidget::fileOpen()
{
    QString fn = QFileDialog::getOpenFileName(this, tr("Open File..."),
                 QString(), tr("HTML-Files (*.htm *.html);;All Files (*)"));
    if (!fn.isEmpty())
        load(fn);
}
*/

/**
 * File New, does nothing
 */
void DB_HtmlEditorWidget::fileNew() {
    setIsNewWidget(true);
}

/**
 * File Open, does nothing
 */
void DB_HtmlEditorWidget::fileOpen() {
    // does nothing
}

/**
 * File Save, can be called by closeWidget()
 */
bool DB_HtmlEditorWidget::fileSave(bool /*withSelect*/) {
    if (isNewWidget()) {
        return fileSaveAs();
    }
    return saveFile(mFullFileName);
}

/**
 * File Save As, the DB_ActionFileSaveAs calls saveFile directly
 */
bool DB_HtmlEditorWidget::fileSaveAs() {
    DB_ActionFileSaveAs a;
    a.setWidget(this);
    a.trigger();
    return true;
}

/**
 * File export PDF.
 * @param printer
 */
void DB_HtmlEditorWidget::filePdf(QPrinter* printer) {
#if defined(QWEBENGINEPAGE_PRINT)
    if (tabWidget->currentIndex() == 0) {
        webView->print(printer);
    } else {
        plainTextEdit->print(printer);
    }
#endif
}

/**
 * File Print.
 * @param printer
 */
void DB_HtmlEditorWidget::filePrint(QPrinter* printer) {
#if defined(QWEBENGINEPAGE_PRINT)
    if (tabWidget->currentIndex() == 0) {
        webView->print(printer);
    } else {
        plainTextEdit->print(printer);
    }
#endif
}

/**
 * File Print Preview.
 * @param printer
 */
void DB_HtmlEditorWidget::filePrintPreview(QPrinter* printer) {
#if defined(QWEBENGINEPAGE_PRINT)
    if (tabWidget->currentIndex() == 0) {
        webView->print(printer);
    } else {
        plainTextEdit->print(printer);
    }
#endif
}

/**
 * Edit Cut.
 */
void DB_HtmlEditorWidget::editCut() {
    if (tabWidget->currentIndex() == 0) {
        webView->pageAction(QWebEnginePage::Cut)->trigger();
    } else {
        plainTextEdit->cut();
    }
}

/**
 * Edit Copy.
 */
void DB_HtmlEditorWidget::editCopy() {
    // webView->triggerPageAction(QWebPage::Copy); alternative:
    if (tabWidget->currentIndex() == 0) {
        webView->pageAction(QWebEnginePage::Copy)->trigger();
    } else {
        plainTextEdit->copy();
    }
}

/**
 * Edit Paste.
 */
void DB_HtmlEditorWidget::editPaste() {
    if (tabWidget->currentIndex() == 0) {
        webView->pageAction(QWebEnginePage::Paste)->trigger();
    } else {
        plainTextEdit->paste();
    }
}

/**
 * Edit Undo.
 */
void DB_HtmlEditorWidget::editUndo() {
    if (tabWidget->currentIndex() == 0) {
        webView->pageAction(QWebEnginePage::Undo)->trigger();
    } else {
        plainTextEdit->undo();
    }

}

/**
 * Edit Redo.
 */
void DB_HtmlEditorWidget::editRedo() {
    webView->triggerPageAction(QWebEnginePage::Redo);
    if (tabWidget->currentIndex() == 0) {
        webView->pageAction(QWebEnginePage::Redo)->trigger();
    } else {
        plainTextEdit->redo();
    }
}

/**
 * Edit Select All
 */
void DB_HtmlEditorWidget::editSelectAll() {
    if (tabWidget->currentIndex() == 0) {
        webView->pageAction(QWebEnginePage::SelectAll)->trigger();
    } else {
        plainTextEdit->selectAll();
    }
}

/**
 * Insert image
 * @param fn local filename
 */
void DB_HtmlEditorWidget::editInsertImage(const RB_String& fn) {
    if (fn.isEmpty() )
        return;
    if (!QFile::exists(fn))
        return;

    QUrl url = QUrl::fromLocalFile(fn);
    execCommand("insertImage", url.toString());

    // HACK: to see the image
    // remember the vertical scrollbar position
/*    int min = webView->page()->mainFrame()->scrollBarMinimum(Qt::Vertical);
    int max = webView->page()->mainFrame()->scrollBarMaximum(Qt::Vertical);
    int value = webView->page()->mainFrame()->scrollBarValue(Qt::Vertical);
    mVerticalScrollbarPerunage = (double)(value - min) / (max - min);

    RB_String content = webView->page()->mainFrame()->toHtml();
    webView->page()->mainFrame()->setHtml(content, url);
*/
//    min = webView->page()->mainFrame()->scrollBarMinimum(Qt::Vertical);
//    max = webView->page()->mainFrame()->scrollBarMaximum(Qt::Vertical);
//    value = (int)((perunage) * (max - min) + min);
//    webView->page()->mainFrame()->setScrollBarValue(Qt::Vertical, value);
    QTimer::singleShot(200, this, SLOT(setWebViewScrollbar()));
}

// shamelessly copied from Qt Demo Browser
static QUrl guessUrlFromString(const QString &string)
{
    QString urlStr = string.trimmed();
    QRegExp test(QLatin1String("^[a-zA-Z]+\\:.*"));

    // Check if it looks like a qualified URL. Try parsing it and see.
    bool hasSchema = test.exactMatch(urlStr);
    if (hasSchema) {
        QUrl url(urlStr, QUrl::TolerantMode);
        if (url.isValid())
            return url;
    }

    // Might be a file.
    if (QFile::exists(urlStr))
        return QUrl::fromLocalFile(urlStr);

    // Might be a shorturl - try to detect the schema.
    if (!hasSchema) {
        int dotIndex = urlStr.indexOf(QLatin1Char('.'));
        if (dotIndex != -1) {
            QString prefix = urlStr.left(dotIndex).toLower();
            QString schema = (prefix == QLatin1String("ftp")) ? prefix : QLatin1String("http");
            QUrl url(schema + QLatin1String("://") + urlStr, QUrl::TolerantMode);
            if (url.isValid())
                return url;
        }
    }

    // Fall back to QUrl's own tolerant parser.
    return QUrl(string, QUrl::TolerantMode);
}

/**
 * Create hyperlink
 */
void DB_HtmlEditorWidget::editCreateLink() {
    QString link = QInputDialog::getText(this, tr("Create link"),
                                         "Enter URL");
    if (!link.isEmpty()) {
        QUrl url = guessUrlFromString(link);
        if (url.isValid())
            execCommand("createLink", url.toString());
    }
}

/**
 * Change event, for example translation
 */
void DB_HtmlEditorWidget::changeEvent(QEvent *e) {
    RB_Widget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}

/**
 * Insert plain text edit HTML tag
 * @param tag without < and >
 */
void DB_HtmlEditorWidget::insertPlainTextEditHtmlTag(const RB_String& tag) {
    QTextCursor cursor = plainTextEdit->textCursor();

    int pos = cursor.position();
    int anchor = cursor.anchor();
    // including internal whitespace, trimmed() does only beginning and ending
    RB_String str = cursor.selectedText().simplified();

    if (anchor != pos) {
        cursor.insertText("<" + tag + ">" + str + "</" + tag + ">");
    } else {
        cursor.insertText("<" + tag + "></" + tag + ">");
    }

    plainTextEdit->setFocus();
}

/**
 * Format Bold
 */
void DB_HtmlEditorWidget::formatBold() {
    if (tabWidget->currentIndex() == 0) {
        execCommand("bold");
    } else {
        insertPlainTextEditHtmlTag("b");
    }
}

/**
 * Format Italic
 */
void DB_HtmlEditorWidget::formatItalic() {
    if (tabWidget->currentIndex() == 0) {
        execCommand("italic");
    } else {
        insertPlainTextEditHtmlTag("i");
    }
}

/**
 * Format Underline
 */
void DB_HtmlEditorWidget::formatUnderline() {
    if (tabWidget->currentIndex() == 0) {
        execCommand("underline");
    } else {
        insertPlainTextEditHtmlTag("u");
    }
}

/**
 * Format Strikethrough
 */
void DB_HtmlEditorWidget::formatStrikethrough() {
    if (tabWidget->currentIndex() == 0) {
        execCommand("strikeThrough");
    } else {
        insertPlainTextEditHtmlTag("del");
    }
}

void DB_HtmlEditorWidget::formatAlignLeft() {
    execCommand("justifyLeft");
}

void DB_HtmlEditorWidget::formatAlignCenter() {
    execCommand("justifyCenter");
}

void DB_HtmlEditorWidget::formatAlignRight() {
    execCommand("justifyRight");
}

void DB_HtmlEditorWidget::formatAlignJustify() {
    execCommand("justifyFull");
}

void DB_HtmlEditorWidget::formatIncreaseIndent() {
    execCommand("indent");
}

void DB_HtmlEditorWidget::formatDecreaseIndent() {
    execCommand("outdent");
}

void DB_HtmlEditorWidget::formatNumberedList() {
    execCommand("insertOrderedList");
}

void DB_HtmlEditorWidget::formatBulletedList() {
    execCommand("insertUnorderedList");
}

void DB_HtmlEditorWidget::formatFontName() {
    QStringList families = QFontDatabase().families();
    bool ok = false;
    QString family = QInputDialog::getItem(this, tr("Font"), tr("Select font:"),
                                           families, 0, false, &ok);

    if (ok)
        execCommand("fontName", family);
}

void DB_HtmlEditorWidget::formatFontSize() {
    QStringList sizes;
    sizes << "h1";
    sizes << "h2";
    sizes << "h3";
    sizes << "h4";
    sizes << "xx-small";
    sizes << "x-small";
    sizes << "small";
    sizes << "medium";
    sizes << "large";
    sizes << "x-large";
    sizes << "xx-large";

    bool ok = false;
    QString size = QInputDialog::getItem(this, tr("Font Size"), tr("Select font size:"),
                                        sizes, sizes.indexOf("medium"), false, &ok);

    if (ok) {
        if (sizes.indexOf(size) < 4) {
            execCommand("formatBlock", size);
        } else {
            execCommand("fontSize", QString::number(sizes.indexOf(size) - 4));
        }
    }
}

void DB_HtmlEditorWidget::formatTextColor() {
    QColor color = QColorDialog::getColor(Qt::black, this);
    if (color.isValid())
        execCommand("foreColor", color.name());
}

void DB_HtmlEditorWidget::formatBackgroundColor() {
    QColor color = QColorDialog::getColor(Qt::white, this);
    if (color.isValid())
        execCommand("hiliteColor", color.name());
}

/**
 * Remove formatting and style
 */
void DB_HtmlEditorWidget::formatRemove() {
    execCommand("removeFormat");
}

/*
void DB_HtmlEditorWidget::insertHtml() {
    if (!insertHtmlDialog) {
        insertHtmlDialog = new QDialog(this);
        if (!ui_dialog)
            ui_dialog = new Ui_Dialog;
        ui_dialog->setupUi(insertHtmlDialog);
        connect(ui_dialog->buttonBox, SIGNAL(accepted()),
                insertHtmlDialog, SLOT(accept()));
        connect(ui_dialog->buttonBox, SIGNAL(rejected()),
                insertHtmlDialog, SLOT(reject()));
    }

    ui_dialog->plainTextEdit->clear();
    ui_dialog->plainTextEdit->setFocus();
    Highlighter *hilite = new Highlighter(ui_dialog->plainTextEdit->document());

    if (insertHtmlDialog->exec() == QDialog::Accepted)
        execCommand("insertHTML", ui_dialog->plainTextEdit->toPlainText());

    delete hilite;
}

void DB_HtmlEditorWidget::zoomOut()
{
    int percent = static_cast<int>(ui->webView->zoomFactor() * 100);
    if (percent > 25) {
        percent -= 25;
        percent = 25 * (int((percent + 25 - 1) / 25));
        qreal factor = static_cast<qreal>(percent) / 100;
        ui->webView->setZoomFactor(factor);
        ui->actionZoomOut->setEnabled(percent > 25);
        ui->actionZoomIn->setEnabled(true);
        zoomSlider->setValue(percent);
    }
}

void DB_HtmlEditorWidget::zoomIn()
{
    int percent = static_cast<int>(ui->webView->zoomFactor() * 100);
    if (percent < 400) {
        percent += 25;
        percent = 25 * (int(percent / 25));
        qreal factor = static_cast<qreal>(percent) / 100;
        ui->webView->setZoomFactor(factor);
        ui->actionZoomIn->setEnabled(percent < 400);
        ui->actionZoomOut->setEnabled(true);
        zoomSlider->setValue(percent);
    }
}
*/

void DB_HtmlEditorWidget::styleParagraph() {
    if (tabWidget->currentIndex() == 0) {
        execCommand("formatBlock", "p");
    } else {
        insertPlainTextEditHtmlTag("p");
    }
}

void DB_HtmlEditorWidget::styleHeading1() {
    if (tabWidget->currentIndex() == 0) {
        execCommand("formatBlock", "h1");
    } else {
        insertPlainTextEditHtmlTag("h1");
    }
}

void DB_HtmlEditorWidget::styleHeading2() {
    if (tabWidget->currentIndex() == 0) {
        execCommand("formatBlock", "h2");
    } else {
        insertPlainTextEditHtmlTag("h2");
    }
}

void DB_HtmlEditorWidget::styleHeading3() {
    if (tabWidget->currentIndex() == 0) {
        execCommand("formatBlock", "h3");
    } else {
        insertPlainTextEditHtmlTag("h3");
    }
}

void DB_HtmlEditorWidget::styleHeading4() {
    if (tabWidget->currentIndex() == 0) {
        execCommand("formatBlock", "h4");
    } else {
        insertPlainTextEditHtmlTag("h4");
    }
}

void DB_HtmlEditorWidget::styleHeading5() {
    if (tabWidget->currentIndex() == 0) {
        execCommand("formatBlock", "h5");
    } else {
        insertPlainTextEditHtmlTag("h5");
    }
}

void DB_HtmlEditorWidget::styleHeading6() {
    if (tabWidget->currentIndex() == 0) {
        execCommand("formatBlock", "h6");
    } else {
        insertPlainTextEditHtmlTag("h6");
    }
}

void DB_HtmlEditorWidget::stylePreformatted() {
    if (tabWidget->currentIndex() == 0) {
        execCommand("formatBlock", "pre");
    } else {
        insertPlainTextEditHtmlTag("pre");
    }
}

void DB_HtmlEditorWidget::styleAddress() {
    if (tabWidget->currentIndex() == 0) {
        execCommand("formatBlock", "address");
    } else {
        insertPlainTextEditHtmlTag("address");
    }
}

/**
 * Execute command using the Javascript engine, HACK for functions not yet
 * directly available in API
 * @param cmd command string
 */
void DB_HtmlEditorWidget::execCommand(const QString &cmd) {
    QString js = QString("document.execCommand(\"%1\", false, null)").arg(cmd);
    webView->page()->runJavaScript(js);
}

/**
 * Execute command using the Javascript engine, HACK for functions not yet
 * directly available in API
 * @param cmd command string
 * @param arg additional arguments
 */
void DB_HtmlEditorWidget::execCommand(const QString &cmd, const QString &arg) {
    QString js = QString("document.execCommand(\"%1\", false, \"%2\")").arg(cmd).arg(arg);
    webView->page()->runJavaScript(js);
}

/**
 * Determines the current state of the named command using the Javascript engine,
 * HACK for functions not yet directly available in API, strikeThrough,
 * orderedList and unorderdList
 * @param cmd command
 * @return true
 */
bool DB_HtmlEditorWidget::queryCommandState(const QString &cmd) {
    QString js = QString("document.queryCommandState(\"%1\", false, null)").arg(cmd);
    webView->page()->runJavaScript(js, [this] (const QVariant& value) { mResult->setValue(value); } );

    QString str = mResult->toString();
    return str.simplified().toLower() == "true";
}

/**
 * Determines the current checked state of the enumerated action
 * @param wa enumerator of QWebPage::WebAction such as QWebPage::ToggleBold
 * @return true if valid
 */
//bool DB_HtmlEditorWidget::pageActionChecked(QWebPage::WebAction wa) {
//    return webView->pageAction(wa)->isChecked();
//}

/**
 * Determines the current enabled state of the enumerated action
 * @param wa enumerator of QWebPage::WebAction such as QWebPage::Undo
 * @return true if valid
 */
//bool DB_HtmlEditorWidget::pageActionEnabled(QWebPage::WebAction wa) {
//    return webView->pageAction(wa)->isEnabled();
//}

/*
#define FOLLOW_ENABLE(a1, a2) a1->setEnabled(ui->webView->pageAction(a2)->isEnabled())
#define FOLLOW_CHECK(a1, a2) a1->setChecked(ui->webView->pageAction(a2)->isChecked())
*/
void DB_HtmlEditorWidget::adjustActions() {
    DB_ACTIONFACTORY->setFormatBoldChecked(queryCommandState("bold"));
    DB_ACTIONFACTORY->setFormatItalicChecked(queryCommandState("italic"));
    DB_ACTIONFACTORY->setFormatUnderlineChecked(queryCommandState("underline"));
    DB_ACTIONFACTORY->setFormatStrikethroughChecked(queryCommandState("strikeThrough"));
    DB_ACTIONFACTORY->setFormatNumberedListChecked(queryCommandState("insertOrderedList"));
    DB_ACTIONFACTORY->setFormatBulletedListChecked(queryCommandState("insertUnorderedList"));

//    FOLLOW_ENABLE(ui->actionEditUndo, QWebPage::Undo);
//    FOLLOW_ENABLE(ui->actionEditRedo, QWebPage::Redo);
//    FOLLOW_ENABLE(ui->actionEditCut, QWebPage::Cut);
//    FOLLOW_ENABLE(ui->actionEditCopy, QWebPage::Copy);
//    FOLLOW_ENABLE(ui->actionEditPaste, QWebPage::Paste);

//    FOLLOW_CHECK(ui->actionFormatBold, QWebPage::ToggleBold);
//    FOLLOW_CHECK(ui->actionFormatItalic, QWebPage::ToggleItalic);
//    FOLLOW_CHECK(ui->actionFormatUnderline, QWebPage::ToggleUnderline);
//    ui->actionFormatStrikethrough->setChecked(queryCommandState("strikeThrough"));
//    ui->actionFormatNumberedList->setChecked(queryCommandState("insertOrderedList"));
//    ui->actionFormatBulletedList->setChecked(queryCommandState("insertUnorderedList"));
}
/*
void DB_HtmlEditorWidget::adjustSource()
{
    setWindowModified(true);
    sourceDirty = true;

    if (ui->tabWidget->currentIndex() == 1)
        changeTab(1);
}
*/

/**
 * Change tab
 */
void DB_HtmlEditorWidget::changeTab(int index) {
    if (mSourceDirty && (index == 1)) {
        QWebEnginePage* page = webView->page();

        // Does not work because htmlContent does not exist at call back
        // QString* htmlContent = new QString();
        // page->toHtml([htmlContent] (const QString& cbValue) { *htmlContent = cbValue; });
        // plainTextEdit->setPlainText(*htmlContent);
        // delete htmlContent;

        page->toHtml([this] (const QString& result) { plainTextEdit->setPlainText(result); });
        mSourceDirty = false;

//        int min = webView->page()->mainFrame()->scrollBarMinimum(Qt::Vertical);
//        int max = webView->page()->mainFrame()->scrollBarMaximum(Qt::Vertical);
//        int value = webView->page()->mainFrame()->scrollBarValue(Qt::Vertical);
//        double perunage = (double)(value - min) / (max - min);

//        QScrollBar* vsb = plainTextEdit->verticalScrollBar();
//        min = vsb->minimum();
//        max = vsb->maximum();
//        value = (int)((perunage) * (max - min) + min);
//        vsb->setValue(value);

    } else if (mSourceDirty && (index == 0)) {
        RB_String content = plainTextEdit->toPlainText();
        QUrl url = webView->url();
        webView->page()->setHtml(content, url);
        mSourceDirty = false;

        QScrollBar* vsb = plainTextEdit->verticalScrollBar();
        int min = vsb->minimum();
        int max = vsb->maximum();
        int value = vsb->value();
        mVerticalScrollbarPerunage = (double)(value - min) / (max - min);

        // ERROR: max is now zero
        // min = webView->page()->mainFrame()->scrollBarMinimum(Qt::Vertical);
        // max = webView->page()->mainFrame()->scrollBarMaximum(Qt::Vertical);
        // value = (int)((perunage) * (max - min) + min);
        // webView->page()->mainFrame()->setScrollBarValue(Qt::Vertical, value);

        // HACK:
        QTimer::singleShot(200, this, SLOT(setWebViewScrollbar()));
    }


}

void DB_HtmlEditorWidget::setWebViewScrollbar() {
//    QScrollBar* vsb = plainTextEdit->verticalScrollBar();
//    int min = vsb->minimum();
//    int max = vsb->maximum();
//    int value = vsb->value();
//    double perunage = (double)(value - min) / (max - min);

/*    int min = webView->page()->mainFrame()->scrollBarMinimum(Qt::Vertical);
    int max = webView->page()->mainFrame()->scrollBarMaximum(Qt::Vertical);
    int value = (int)((mVerticalScrollbarPerunage) * (max - min) + min);
    webView->page()->mainFrame()->setScrollBarValue(Qt::Vertical, value);
*/
}


void DB_HtmlEditorWidget::openLink(const QUrl &url) {
    QString msg = QString(tr("Open %1 ?")).arg(url.toString());
    if (QMessageBox::question(this, tr("Open link"), msg,
                              QMessageBox::Open | QMessageBox::Cancel) ==
            QMessageBox::Open)
        QDesktopServices::openUrl(url);
}

/*
void DB_HtmlEditorWidget::changeZoom(int percent) {
    ui->actionZoomOut->setEnabled(percent > 25);
    ui->actionZoomIn->setEnabled(percent < 400);
    qreal factor = static_cast<qreal>(percent) / 100;
    webView->setZoomFactor(factor);
    zoomLabel->setText(tr(" Zoom: %1% ").arg(percent));
    zoomSlider->setValue(percent);
}


void DB_HtmlEditorWidget::closeEvent(QCloseEvent *e)
{
    if (maybeSave())
        e->accept();
    else
        e->ignore();
}
*/

//void DB_HtmlEditorWidget::keyPressEvent(QKeyEvent* e) {
//    if (e->key() == (Qt::CTRL + Qt::Key_Tab)) {
//        if (this->tabWidget->currentIndex() == 0) {
//            changeTab(1);
//            return;
//        } else if (this->tabWidget->currentIndex() == 1) {
//            changeTab(0);
//            return;
//        }
//    }
//    RB_Widget::keyPressEvent(e);
//}

bool DB_HtmlEditorWidget::closeWidget() {
    if (maybeSave()) {
        return true;
    } else {
        return false;
    }
}

/**
 * Insert HTML at the cursor position
 * @param html HTML to be added
 */
void DB_HtmlEditorWidget::insertHtml(const RB_String& html) {
    if (tabWidget->currentIndex() == 0) {
        execCommand("insertHtml", html);
    } else {
        plainTextEdit->insertPlainText(html);
    }

}

/**
 * @returns the help subject, in this case the widget objectname
 */
RB_String DB_HtmlEditorWidget::getHelpSubject() const {
    return objectName();
}

/**
 * Load file
 * @param fn filename
 * @return true at success
 */
bool DB_HtmlEditorWidget::loadFile(const QString& fn) {
    // Check if file can be read
    QFile file(fn);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        DB_DIALOGFACTORY->requestWarningDialog(
            tr("Cannot read file %1:\n%2.").arg(fn).arg(file.errorString()));
        return false;
    }

    // Read file, does not work anymore in Qt5.1.1
//    QApplication::setOverrideCursor(Qt::WaitCursor);
//    webView->setUrl(QUrl(file.fileName()));
//    plainTextEdit->setPlainText(webView->page()->mainFrame()->toHtml());
//    QApplication::restoreOverrideCursor();

    // Read file
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QTextStream in(&file);
    plainTextEdit->setPlainText(in.readAll());
    plainTextEdit->document()->setModified(false);
    RB_String content = plainTextEdit->toPlainText();
    webView->page()->setHtml(content, QUrl(file.fileName()));
    QApplication::restoreOverrideCursor();


    setCurrentFileName(fn);
    emit modificationChanged(false);
    return true;
}

/**
 * Save file
 * @param fn filename
 * @return true if success
 */
bool DB_HtmlEditorWidget::saveFile(const RB_String &fn) {
    QFile file(fn);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        DB_DIALOGFACTORY->requestWarningDialog(
            tr("Cannot write file %1:\n%2.").arg(fn).arg(file.errorString()));
        DB_DIALOGFACTORY->statusBarMessage(tr("File save ERROR"), 2000);
        return false;
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);
    QTextStream out(&file);
    if (tabWidget->currentIndex() == 0) {
        QString* html = new QString();
        webView->page()->toHtml([html] (const QString& cbValue) { *html = cbValue; });
        out << html;
        delete html;
    } else {
        out << plainTextEdit->toPlainText();
    }
    QApplication::restoreOverrideCursor();

    // original
//    // FIXME: here we always use UTF-8 encoding
//    QString content = webView->page()->mainFrame()->toHtml();
//    QByteArray data = content.toUtf8();
//    qint64 c = file.write(data);
//    bool success = (c >= data.length());

    setCurrentFileName(fn);
    setIsNewWidget(false);
    emit modificationChanged(false);
    DB_DIALOGFACTORY->statusBarMessage(tr("File saved"), 2000);
    return true;
}

/*
void DB_HtmlEditorWidget::setCurrentFileName(const QString &fileName)
{
    this->fileName = fileName;

    QString shownName;
    if (fileName.isEmpty())
        shownName = "untitled";
    else
        shownName = QFileInfo(fileName).fileName();

    setWindowTitle(tr("%1[*] - %2").arg(shownName).arg(tr("HTML Editor")));
    setWindowModified(false);

    bool allowSave = true;
    if (fileName.isEmpty() || fileName.startsWith(QLatin1String(":/")))
        allowSave = false;
    ui->actionFileSave->setEnabled(allowSave);
}
*/

void DB_HtmlEditorWidget::slotContentChanged() {
    mSourceDirty = true;
    emit modificationChanged(true);
}

/**
 * @brief DB_HtmlEditorWidget::slotLoadUrl
 * @param url
 */
void DB_HtmlEditorWidget::slotLoadUrl(const QUrl &url) {
    if (!webView->isWidgetMaximized()) {
        maybeSave();
    }

    if (url.isLocalFile()) {
        setCurrentFileName(url.toLocalFile());
    }

    webView->load(url);
    emit modificationChanged(false);
}
