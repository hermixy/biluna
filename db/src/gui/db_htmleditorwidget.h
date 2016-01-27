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

#ifndef DB_HTMLEDITORWIDGET_H
#define DB_HTMLEDITORWIDGET_H

//#include <QMainWindow>
#include "db_htmlhighlighter.h"
#include "rb_string.h"
#include "rb_widget.h"
#include "ui_db_htmleditorwidget.h"

#if QT_VERSION < 0x040500
#error You must use Qt >= 4.5.0!
#endif

class QLabel;
class QSlider;
class QUrl;

/**
 * Class for viewing and editing HTML content
 */
class DB_EXPORT DB_HtmlEditorWidget : public RB_Widget, private Ui::DB_HtmlEditorWidget {

    Q_OBJECT

public:
    DB_HtmlEditorWidget(QWidget *parent = 0);
    virtual ~DB_HtmlEditorWidget();

    virtual RB_String getName() const;
    virtual RB2::PerspectiveType getPerspectiveType() const { return RB2::PerspectiveDB; }
    virtual QWidget* getMainWidget();

    virtual void init();

    virtual void fileNew();
    virtual void fileOpen();
    virtual bool fileSave(bool withSelect);
    virtual bool fileSaveAs();
    virtual void filePdf(QPrinter* printer);
    virtual void filePrint(QPrinter* printer);
    virtual void filePrintPreview(QPrinter* printer);

    virtual void editCut();
    virtual void editCopy();
    virtual void editPaste();
    virtual void editUndo();
    virtual void editRedo();
    virtual void editSelectAll();
    virtual void editInsertImage(const RB_String& fn);
    virtual void editCreateLink();

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

    virtual void styleParagraph();
    virtual void styleHeading1();
    virtual void styleHeading2();
    virtual void styleHeading3();
    virtual void styleHeading4();
    virtual void styleHeading5();
    virtual void styleHeading6();
    virtual void stylePreformatted();
    virtual void styleAddress();

    virtual RB_String getHelpSubject() const;

    virtual bool loadFile(const RB_String& fn);
    virtual bool saveFile(const RB_String& fn);
    virtual bool closeWidget();

    virtual void insertHtml(const RB_String& html);

//    bool queryCommandState(const QString&);
//    bool pageActionChecked(QWebPage::WebAction wa);
//    bool pageActionEnabled(QWebPage::WebAction wa);

protected:
    void changeEvent(QEvent *e);
//    virtual void closeEvent(QCloseEvent *e);
//    virtual void keyPressEvent(QKeyEvent* e); does not work
    void insertPlainTextEditHtmlTag(const RB_String& tag);


protected slots:
    void slotContentChanged();
    void slotLoadUrl(const QUrl& url);

private:

//    void setupActions();
//    void setupToolBar();
//    void setupMenu();

    void execCommand(const QString&);
    void execCommand(const QString &cmd, const QString &arg);

private slots:
//    void insertHtml();
//    void zoomOut();
//    void zoomIn();
    void adjustActions();
//    void adjustSource();
    void changeTab(int);
    void openLink(const QUrl&);
//    void changeZoom(int);

    void setWebViewScrollbar();

private:
    void setWebPageContentToPlainTextWidget();
    void setContent(const QString& content);
    bool mSourceDirty;
//    QLabel *zoomLabel;
//    QSlider *zoomSlider;
    DB_HtmlHighlighter* mCodeHighlighter;
//    Ui_Dialog *ui_dialog;
    QDialog* mInsertHtmlDialog;
    // To support lambda function
    QString mContent;

    double mVerticalScrollbarPerunage;

};

#endif // HTML_EDITOR_H
