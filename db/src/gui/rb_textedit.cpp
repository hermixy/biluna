/*****************************************************************
 * $Id: rb_textedit.cpp 2198 2014-11-17 21:34:16Z rutger $
 * Created: 2011-05-02 rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "rb_textedit.h"

#include <QAbstractTextDocumentLayout>
#include <QBuffer>
#include <QColorDialog>
#include <QFontDatabase>
#include <QImageReader>
#include <QInputDialog>
#include <QMimeData>
#include <QPainter>
#include <QPrinter>
#include <QTextCursor>
#include <QTextDocument>
#include <QTextList>
#include <QTextListFormat>
#include <QTextTable>
#include <QUrl>
#include "db_actionfactory.h"
#include "rb_debug.h"
#include "rb_printtextheaderfooter.h" // HACK only
#include "rb_variant.h"

/**
 * Constructor
 */
RB_TextEdit::RB_TextEdit(QWidget *parent) : QTextEdit(parent) {
    mIsDialog = false;
    mIsContentInserted = false;
    mIsShowVirtualTableBorders = false;

    setDefaultFormat();
    setPaletteColors();

    connect(this, SIGNAL(cursorPositionChanged()),
            this, SLOT(slotUpdateCursorPosition()));
    connect(this, SIGNAL(cursorPositionChanged()),
            this, SLOT(slotAdjustActions()));
}

/**
 * Set isDialog to true if this widget is on a dialog. This will prevent
 * that the actions on the mainmenu are updated when the cursor position
 * changes
 */
void RB_TextEdit::setIsDialog() {
    mIsDialog = true;
}

/**
 * Print this textEdit widget
 * @param printer
 */
void RB_TextEdit::print(QPrinter* printer) {
    // allows printing of selection
    // QTextEdit::print(printer);

    // otherwise:
    // document()->print(printer);

    // HACK: because direct printing does not work Qt5.2
    // TODO: replace by above code, QTextEdit::print(printer) when bug solved
    RB_PrintTextHeaderFooter util;
    util.setDocContent(this->document());
    util.setPrintPageNumbers();
    util.print(printer);
}

/**
 * Insert image from file
 * @param fn file path and name
 */
void RB_TextEdit::insertImage(const QString &fn) {
    mIsContentInserted = true;
    dropImage(QUrl::fromLocalFile(fn), QImage());
    mIsContentInserted = false;
}

/**
 * TODO: <IMG SRC="data:image/gif;base64,RAAA...more data....."> or red dot
<img src="data:image/png;base64,
iVBORw0KGgoAAAANSUhEUgAAAAoAAAAKCAYAAACNMs+9AAAABGdBTUEAALGPC/xhBQAAAAlwSFlz
AAALEwAACxMBAJqcGAAAAAd0SU1FB9YGARc5KB0XV+IAAAAddEVYdENvbW1lbnQAQ3JlYXRlZCB3
aXRoIFRoZSBHSU1Q72QlbgAAAF1JREFUGNO9zL0NglAAxPEfdLTs4BZM4DIO4C7OwQg2JoQ9LE1e
xdlYvBBeZ7jqch9//q1uH4TLzw4d6+ErXMMcXuHWxId3KOETnnXXV6MJpcq2MLaI97CER3N0vr4M
khoXe0rZigAAAABJRU5ErkJggg==" alt="" />
 */
void RB_TextEdit::saveHtmlEmbeddedImage(RB_String& html) {
    /*
    // Icon is not required, relevant resource image to be converted
    QIcon icon = mostVisited.at(i).icon; // use your method to retrieve the QIcon object
    QImage image(icon.pixmap(20,20).toImage()); // 20,20 are the size of the icon
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    image.save(&buffer, "PNG"); // writes the image in PNG format inside the buffer
    QString iconBase64 = QString::fromLatin1(byteArray.toBase64().data());
    */

    RB_Variant var;
    RB_String imgName;
    QImage img;

    // From image editing software
    QRegExp rx("<img src=\"(inserted_image_.+)\""); // cap(1) = inserted_image_%1
    rx.setMinimal(true);
    rx.setCaseSensitivity(Qt::CaseInsensitive);
    int pos = 0;

    while ((pos = rx.indexIn(html, pos)) != -1) {
        imgName = rx.cap(1);
        var = document()->resource(QTextDocument::ImageResource, imgName);
        img = qvariant_cast<QImage>(var);
        QByteArray ba;
        QBuffer buffer(&ba);
        img.save(&buffer, "PNG"); // writes the image in PNG format inside the buffer

        html.replace(imgName, "data:image/png;base64,"
                     + RB_String::fromLatin1(ba.toBase64().data()), Qt::CaseInsensitive);
        pos += rx.matchedLength();
    }

    // From file
    QUrl imgUrl;
    QString urlPath;
    QFile file;

    rx.setPattern("<img src=\"(file:///.+)\"");
    rx.setMinimal(true);
    rx.setCaseSensitivity(Qt::CaseInsensitive);
    pos = 0;

    while ((pos = rx.indexIn(html, pos)) != -1) {
        imgUrl.setPath(rx.cap(1));

        if (imgUrl.isValid()) {
            urlPath = imgUrl.toLocalFile();
            urlPath.remove("file:///", Qt::CaseInsensitive);
            file.setFileName(urlPath);

            if (img.load(file.fileName())) {
                QByteArray ba;
                QBuffer buffer(&ba);
                img.save(&buffer, "PNG"); // writes the image in PNG format inside the buffer

                html.replace(imgUrl.toLocalFile(), "data:image/png;base64,"
                             + RB_String::fromLatin1(ba.toBase64().data()), Qt::CaseInsensitive);
            }
        }

        pos += rx.matchedLength();
    }
}

/**
 * Show temporarily/virtual table borders when table is clicked
 * @param isShowVirtualTableBorders
 */
void RB_TextEdit::showVirtualTableBorders(bool isShowVirtualTableBorders) {
    mIsShowVirtualTableBorders = isShowVirtualTableBorders;
}

/**
 * Increase point size of each font
 * @param pointSize
 */
void RB_TextEdit::fontZoomIn(int pointSize) {
    QTextDocument* doc = document();
    QTextCursor textCursor(doc); // at beginning first block
    bool lastBlockEdited = false;
    textCursor.beginEditBlock();

    while (!lastBlockEdited) {
        textCursor.movePosition(QTextCursor::StartOfBlock);
        textCursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);

        QTextCharFormat fmt = textCursor.charFormat();
        QFont f = fmt.font();
        int fontPointSize = f.pointSize();
        fmt.setFontPointSize(fontPointSize + pointSize);
        textCursor.mergeCharFormat(fmt);

        if (textCursor.block() == doc->lastBlock()) {
            lastBlockEdited = true;
        } else {
            textCursor.movePosition(QTextCursor::NextBlock);
        }
    }

    textCursor.endEditBlock();
}

/**
 * Decrease the point size of each font
 * @param pointSize
 */
void RB_TextEdit::fontZoomOut(int pointSize) {
    fontZoomIn(-pointSize);
}

/**
 * Overriden function: set flag to prevent calling slotUpdateCursorPosition
 * with parsing of content in progress
 * @param text
 */
void RB_TextEdit::append(const QString& text) {
    mIsContentInserted = true;
    QTextEdit::append(text);
    mIsContentInserted = false;
}

/**
 * Overriden function: set flag to prevent calling slotUpdateCursorPosition
 * with parsing of content in progress
 */
void RB_TextEdit::cut() {
    mIsContentInserted = true;
    QTextEdit::cut();
    mIsContentInserted = false;
}

/**
 * Overriden function: set flag to prevent calling slotUpdateCursorPosition
 * with parsing of content in progress
 * @param text
 */
void RB_TextEdit::insertHtml(const QString& text) {
    mIsContentInserted = true;
    QTextEdit::insertHtml(text);
    mIsContentInserted = false;
}

/**
 * Overriden function: set flag to prevent calling slotUpdateCursorPosition
 * with parsing of content in progress
 * @param text
 */
void RB_TextEdit::insertPlainText(const QString& text) {
    mIsContentInserted = true;
    QTextEdit::insertPlainText(text);
    mIsContentInserted = false;
}

/**
 * Overriden function: set flag to prevent calling slotUpdateCursorPosition
 * with parsing of content in progress
 */
void RB_TextEdit::paste() {
    mIsContentInserted = true;
    QTextEdit::paste();
    mIsContentInserted = false;
}

/**
 * Overriden function: set flag to prevent calling slotUpdateCursorPosition
 * with parsing of content in progress
 * @param text
 */
void RB_TextEdit::setHtml(const QString& text) {
    mIsContentInserted = true;
    QTextEdit::setHtml(text);
    mIsContentInserted = false;
}

/**
 * Overriden function: set flag to prevent calling slotUpdateCursorPosition
 * with parsing of content in progress
 * @param text
 */
void RB_TextEdit::setPlainText(const QString& text) {
    mIsContentInserted = true;
    QTextEdit::setPlainText(text);
    mIsContentInserted = false;
}

/**
 * Overriden function: set flag to prevent calling slotUpdateCursorPosition
 * with parsing of content in progress
 * @param text
 */
void RB_TextEdit::setText(const QString& text) {
    mIsContentInserted = true;
    QTextEdit::setText(text);
    mIsContentInserted = false;
}

/**
 * Set or unset bold
 */
void RB_TextEdit::slotTextBold() {
    QTextCursor cursor = getTextCursor(); // textCursor(); does not work for widget losing focus
    QTextCharFormat fmt;
    fmt.setFontWeight(cursor.charFormat().fontWeight() != QFont::Bold ? QFont::Bold : QFont::Normal);
    mergeFormatOnWordOrSelection(fmt);
}

/**
 * Set or unset underline
 */
void RB_TextEdit::slotTextUnderline() {
    QTextCursor cursor = getTextCursor();
    QTextCharFormat fmt;
    fmt.setFontUnderline(!cursor.charFormat().fontUnderline());
    mergeFormatOnWordOrSelection(fmt);
}

/**
 * Set or unset italic
 */
void RB_TextEdit::slotTextItalic() {
    QTextCursor cursor = getTextCursor();
    QTextCharFormat fmt;
    fmt.setFontItalic(!cursor.charFormat().fontItalic());
    mergeFormatOnWordOrSelection(fmt);
}

/**
 * Set or unset strikeout or strikethrough
 */
void RB_TextEdit::slotTextStrikeOut() {
    QTextCursor cursor = getTextCursor();
    QTextCharFormat fmt;
    fmt.setFontStrikeOut(!cursor.charFormat().fontStrikeOut());
    mergeFormatOnWordOrSelection(fmt);
}

/**
 * Set text family
 */
void RB_TextEdit::slotTextFamily() {
    QTextCursor cursor = getTextCursor();
    RB_String family = cursor.charFormat().fontFamily();

    RB_StringList families = QFontDatabase().families();
    int number = families.indexOf(family);
    if (number < 0) number = 0;
    bool ok = false;

    RB_String f = QInputDialog::getItem(this, tr("Font"), tr("Select font:"),
                                        families, number, false, &ok);
    if (ok) {
        QTextCharFormat fmt;
        fmt.setFontFamily(f);
        mergeFormatOnWordOrSelection(fmt);
    }

}

/**
 * Set text size
 */
void RB_TextEdit::slotTextSize() {
    QTextCursor cursor = getTextCursor();
    RB_String size = RB_String::number(cursor.charFormat().fontPointSize());
    RB_String family = cursor.charFormat().fontFamily();

    QList<int> sizeList = QFontDatabase().pointSizes(family);
    RB_StringList sizes;

    for  (int i = 0; i < sizeList.size(); ++i) {
        sizes.append(RB_String::number(sizeList.at(i)));
    }

    bool ok = false;
    RB_String s = QInputDialog::getItem(this, tr("Font Size"), tr("Select font size:"),
                                        sizes, sizes.indexOf(size), false, &ok);

    if (ok) {
        QTextCharFormat fmt;
        fmt.setFontPointSize(s.toInt());
        mergeFormatOnWordOrSelection(fmt);
    }
}

/**
 * Set text style
 */
void RB_TextEdit::slotTextStyle(int listIndex) {
    QTextCursor cursor = getTextCursor();

    if (listIndex != 0) {
        QTextListFormat::Style style = QTextListFormat::ListDisc;

        if (listIndex == RB_TextEdit::ListDisc) {
            // User selected a bulleted list, now select which bullet
            RB_StringList bullets;
            bullets << tr("Disc") << tr("Circle") << tr("Square");
            bool ok = false;

            RB_String b = QInputDialog::getItem(this, tr("Bulleted List"),
                                                tr("Select bullet type:"),
                                                bullets, 0, false, &ok);
            if (ok) {
                if (b == tr("Disc")) {
                    style = QTextListFormat::ListDisc;
                } else if (b == tr("Circle")) {
                    style = QTextListFormat::ListCircle;
                } else if (b == tr("Square")) {
                    style = QTextListFormat::ListSquare;
                }
            }
        } else if (listIndex == RB_TextEdit::ListDecimal) {
            // User selected a numbered list, now select which number
            RB_StringList bullets;
            bullets << tr("Decimal") << tr("Lower Alpha") << tr("Upper Alpha")
                    << tr("Lower Roman") << tr("Upper Roman");
            bool ok = false;

            RB_String b = QInputDialog::getItem(this, tr("Numbered List"),
                                                tr("Select number type:"),
                                                bullets, 0, false, &ok);
            if (ok) {
                if (b == tr("Decimal")) {
                    style = QTextListFormat::ListDecimal;
                } else if (b == tr("Lower Alpha")) {
                    style = QTextListFormat::ListLowerAlpha;
                } else if (b == tr("Upper Alpha")) {
                    style = QTextListFormat::ListUpperAlpha;
                } else if (b == tr("Lower Roman")) {
                    style = QTextListFormat::ListLowerRoman;
                } else if (b == tr("Upper Roman")) {
                    style = QTextListFormat::ListUpperRoman;
                }
            }
        }

        cursor.beginEditBlock();

        QTextBlockFormat blockFmt = cursor.blockFormat();
        QTextListFormat listFmt;
        listFmt.setIndent(blockFmt.indent() + 1);
        blockFmt.setIndent(0);
        cursor.setBlockFormat(blockFmt);
        listFmt.setStyle(style);
        cursor.createList(listFmt);

        cursor.endEditBlock();
    } else {
        // #### unformats block?
        QTextBlockFormat bfmt;
        bfmt.setObjectIndex(-1);
        cursor.mergeBlockFormat(bfmt);
    }
}

/**
 * Update text indent
 * @param increment +1 is indent -1 is outdent
 */
void RB_TextEdit::slotTextIndent(int incr) {
    QTextCursor cursor = getTextCursor();
    QTextBlockFormat blockFmt = cursor.blockFormat();

    cursor.beginEditBlock();

    if (cursor.currentList()) {
        // Is a list
        QTextListFormat listFmt = cursor.currentList()->format();
        QTextListFormat::Style style = listFmt.style();

        if (incr > 0) {
            switch (style) {
            case QTextListFormat::ListDisc:
                style = QTextListFormat::ListCircle;
                break;
            case QTextListFormat::ListCircle:
                style = QTextListFormat::ListSquare;
                break;
            case QTextListFormat::ListSquare:
                // remains square
                break;
            case QTextListFormat::ListDecimal:
                style = QTextListFormat::ListUpperAlpha;
                break;
            case QTextListFormat::ListUpperAlpha:
                style = QTextListFormat::ListUpperRoman;
                break;
            case QTextListFormat::ListUpperRoman:
                style = QTextListFormat::ListLowerAlpha;
                break;
            case QTextListFormat::ListLowerAlpha:
                style = QTextListFormat::ListLowerRoman;
                break;
            case QTextListFormat::ListLowerRoman:
                // remains lower roman
                break;
            default:
                break;
            }
        } else if (incr < 0) {
            switch (style) {
            case QTextListFormat::ListDisc:
                // remains disc
                break;
            case QTextListFormat::ListCircle:
                style = QTextListFormat::ListDisc;
                break;
            case QTextListFormat::ListSquare:
                style = QTextListFormat::ListCircle;
                break;
            case QTextListFormat::ListDecimal:
                break;
            case QTextListFormat::ListUpperAlpha:
                style = QTextListFormat::ListDecimal;
                break;
            case QTextListFormat::ListUpperRoman:
                style = QTextListFormat::ListUpperAlpha;
                break;
            case QTextListFormat::ListLowerAlpha:
                style = QTextListFormat::ListUpperRoman;
                break;
            case QTextListFormat::ListLowerRoman:
                style = QTextListFormat::ListLowerAlpha;
                break;
            default:
                break;
            }
        }

        listFmt.setIndent(listFmt.indent() + incr);
//        blockFmt.setIndent(0);
        cursor.setBlockFormat(blockFmt);
        listFmt.setStyle(style);
        cursor.createList(listFmt);
    } else {
        // Is not a list
        blockFmt.setIndent(blockFmt.indent() + incr);
        cursor.setBlockFormat(blockFmt);
    }


    cursor.endEditBlock();


//    switch (listIndex) {
//    default:
//    case ListDisc:
//        break;
//    case ListCircle:
//        style = QTextListFormat::ListCircle;
//        break;
//    case ListSquare:
//        break;
//    case ListDecimal:
//        style = QTextListFormat::ListDecimal;
//        break;
//    case ListLowerAlpha:
//        style = QTextListFormat::ListLowerAlpha;
//        break;
//    case ListUpperAlpha:
//        style = QTextListFormat::ListUpperAlpha;
//        break;
//    case ListLowerRoman:
//        style = QTextListFormat::ListLowerRoman;
//        break;
//    case ListUpperRoman:
//        style = QTextListFormat::ListUpperRoman;
//        break;
//    }


}

/**
 * Set text color
 */
void RB_TextEdit::slotTextColor() {
    QColor col = QColorDialog::getColor(this->textColor(), this);
    if (!col.isValid())
        return;
    QTextCharFormat fmt;
    fmt.setForeground(col);
    mergeFormatOnWordOrSelection(fmt);
}

/**
 * Set text background color
 */
void RB_TextEdit::slotBackgroundColor() {
    QTextCursor cursor = getTextCursor();
    QColor col = QColorDialog::getColor(cursor.charFormat().background().color(), this);
    if (!col.isValid())
        return;
    QTextCharFormat fmt;
    fmt.setBackground(col);
    mergeFormatOnWordOrSelection(fmt);
}

/**
 * Set text alignment
 */
void RB_TextEdit::slotTextAlign(int alignIndex) {
    switch (alignIndex) {
    default:
    case AlignLeft:
        this->setAlignment(Qt::AlignLeft | Qt::AlignAbsolute);
        break;
    case AlignCenter:
        this->setAlignment(Qt::AlignHCenter);
        break;
    case AlignRight:
        this->setAlignment(Qt::AlignRight | Qt::AlignAbsolute);
        break;
    case AlignJustify:
        this->setAlignment(Qt::AlignJustify);
        break;
    }
}

/**
 * Insert HTML at cursor position
 * @param html HTML to be inserted
 */
void RB_TextEdit::slotInsertHtml(const RB_String& html) {
    insertHtml(html);
}

/**
 * Check whether this resource can be inserted
 * @param source mime source, image or text
 * @returns true on success
 */
bool RB_TextEdit::canInsertFromMimeData(const QMimeData* source) const {
    return source->hasImage() || source->hasUrls()
            || QTextEdit::canInsertFromMimeData(source);
}

/**
 * Insert resource
 * @param source mime source, image or text
 */
void RB_TextEdit::insertFromMimeData(const QMimeData* source) {
    mIsContentInserted = true;

    if (source->hasImage()) {
        static int i = 1;
        QUrl url(QString("inserted_image_%1").arg(i++)); // identifier only
        dropImage(url, qvariant_cast<QImage>(source->imageData()));
    } else if (source->hasUrls()) {
        foreach (QUrl url, source->urls()) {
            QFileInfo info(url.toLocalFile());

            if (QImageReader::supportedImageFormats()
                    .contains(info.suffix().toLower().toLatin1())) {
                dropImage(url, QImage(info.filePath()));
            } else {
                dropTextFile(url);
            }
        }
    } else {
        QTextEdit::insertFromMimeData(source);
    }

    mIsContentInserted = false;
}

void RB_TextEdit::keyPressEvent(QKeyEvent* ke) {
    if (ke->key() == Qt::Key_BraceLeft) {
        mLeftLimitChar = "{";
        mRightLimitChar = "}";
        handleLeftLimitCharPressed();
        ke->accept();
        return;
    } else if (ke->key() == Qt::Key_BracketLeft) {
        mLeftLimitChar = "[";
        mRightLimitChar = "]";
        handleLeftLimitCharPressed();
        ke->accept();
        return;
    } else if (ke->key() == Qt::Key_ParenLeft) {
        mLeftLimitChar = "(";
        mRightLimitChar = ")";
        handleLeftLimitCharPressed();
        ke->accept();
        return;
    } else if (ke->key() == Qt::Key_Less) {
        mLeftLimitChar = "<";
        mRightLimitChar = ">";
        handleLeftLimitCharPressed();
        ke->accept();
        return;
    } else if (ke->key() == Qt::Key_QuoteDbl) {
        mLeftLimitChar = "\"";
        mRightLimitChar = "\"";
        handleLeftLimitCharPressed();
        ke->accept();
        return;
    } else if (ke->key() == Qt::Key_Apostrophe) {
        mLeftLimitChar = "\'";
        mRightLimitChar = "\'";
        handleLeftLimitCharPressed();
        ke->accept();
        return;
    }

    QTextEdit::keyPressEvent(ke);
}

void RB_TextEdit::mergeFormatOnWordOrSelection(const QTextCharFormat& format) {
    QTextCursor cursor = getTextCursor();
    if (!cursor.hasSelection())
        cursor.select(QTextCursor::WordUnderCursor);
    cursor.mergeCharFormat(format);
    this->mergeCurrentCharFormat(format);
}

/**
 * Get the text cursor this is different for a MDI window or Dialog
 */
QTextCursor RB_TextEdit::getTextCursor() {
    if (mIsDialog) {
        QTextCursor cursor = textCursor();
        cursor.setPosition(mCursorAnchor, QTextCursor::MoveAnchor);
        cursor.setPosition(mCursorPosition, QTextCursor::KeepAnchor);
        return cursor;
    }

    return textCursor();
}

/**
 * Slot to store cursor anchor and position in case of dialog,
 * highlight table if cursor is in table,
 * \todo change context menu.
 */
void RB_TextEdit::slotUpdateCursorPosition() {
    // HACK: store cursor position for widgets that lose focus on for example dialog
    if (mIsDialog && (textCursor().anchor() != 0 || textCursor().position() != 0)) {
        mCursorAnchor = textCursor().anchor();
        mCursorPosition = textCursor().position();
    }

    if (mIsContentInserted) {
        // do not continue with updating table border
        // because content insertion is in progress
        // and the table might be not yet valid
        return;
    }

    QTextTable* table = textCursor().currentTable();
    if (table && mIsShowVirtualTableBorders) {
       if (table->format().border() == 0.0) {
           QTextTableFormat format = table->format();
           format.setBorder(0.2);
           table->setFormat(format);
       }
    } else {
        // set all tables in list to border 0.0
        // http://qt-project.org/doc/qt-5.0/qtgui/richtext-structure.html
        QTextFrame* frame = document()->rootFrame();
        unsetBorder(frame);
    }

//    QTextImageFormat* imgFormat = textCursor().charFormat().toImageFormat();
//    if (imgFormat) {
    //    }
}

void RB_TextEdit::changeEvent(QEvent* e) {
    QTextEdit::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
//        retranslateUi(this);
        break;
    case QEvent::PaletteChange:
        setPaletteColors();
        // update(); does not work, need to open and close widget
        break;
    default:
        break;
    }
}

void RB_TextEdit::unsetBorder(QTextFrame* frame) {
    if (!frame) {
        return;
    }

    QTextFrame::iterator it;

    for (it = frame->begin(); !(it.atEnd()); ++it) {
        QTextFrame* childFrame = it.currentFrame();

        if (childFrame) {
            unsetBorder(childFrame);
            QTextTable* childTable = qobject_cast<QTextTable*>(childFrame);

            if (childTable) {
                QTextTableFormat format = childTable->format();

                if (format.border() == 0.2) {
                    format.setBorder(0.0);
                    childTable->setFormat(format);
                }
            }
        }
    }
}

/**
 * Adjust toggle state of actions.
 * Currently only of bold, italic, underline, strikethrough,
 * numberedlist and bulletedlist
 */
void RB_TextEdit::slotAdjustActions() {
    if (mIsDialog) return;

    QTextCursor cursor = getTextCursor();
    QTextCharFormat fmt = cursor.charFormat();

    DB_ACTIONFACTORY->setFormatBoldChecked(
            fmt.fontWeight() == QFont::Bold ? true : false);
    DB_ACTIONFACTORY->setFormatUnderlineChecked(fmt.fontUnderline());
    DB_ACTIONFACTORY->setFormatItalicChecked(fmt.fontItalic());
    DB_ACTIONFACTORY->setFormatStrikethroughChecked(fmt.fontStrikeOut());

    QTextListFormat listFmt;
    if (cursor.currentList()) {
        listFmt = cursor.currentList()->format();

        bool numList = false;
        bool bulList = false;
        if (listFmt.style() == QTextListFormat::ListDisc
                    || listFmt.style() == QTextListFormat::ListCircle
                    || listFmt.style() == QTextListFormat::ListSquare) {
            bulList = true;
        } else if (listFmt.style() == QTextListFormat::ListDecimal
                    || listFmt.style() == QTextListFormat::ListLowerAlpha
                    || listFmt.style() == QTextListFormat::ListUpperAlpha
                    || listFmt.style() == QTextListFormat::ListLowerRoman
                    || listFmt.style() == QTextListFormat::ListUpperRoman) {
            numList = true;
        }

        DB_ACTIONFACTORY->setFormatNumberedListChecked(numList);
        DB_ACTIONFACTORY->setFormatBulletedListChecked(bulList);
    } else {
        DB_ACTIONFACTORY->setFormatNumberedListChecked(false);
        DB_ACTIONFACTORY->setFormatBulletedListChecked(false);
    }
}

/**
 * Drop an image file based on an url or file location
 * @param url file location or identifier, here used to identify the
 * the image in the resource system
 * @param image prepared image from file path to be inserted as resource
 */
void RB_TextEdit::dropImage(const QUrl& url, const QImage& image){
    if (!image.isNull()) {
        QTextEdit::document()->addResource(QTextDocument::ImageResource, url, image);
        // QTextEdit::textCursor().insertImage(url.toString());

        // embed image
        QByteArray ba;
        QBuffer buffer(&ba);
        image.save(&buffer, "PNG");

        QString html;
        html = "<img src=\"data:image/png;base64,";
        html += RB_String::fromLatin1(ba.toBase64().data());
        html += "\" />";
        QTextEdit::textCursor().insertHtml(html);

    } else if (url.isValid()) {
        QString urlPath;
        QFile file;

        if (url.isValid() && url.isLocalFile()) {
            urlPath = url.toLocalFile();
            urlPath.remove("file:///", Qt::CaseInsensitive);
            file.setFileName(urlPath);
            QImage img;

            if (img.load(file.fileName())) {
                QByteArray ba;
                QBuffer buffer(&ba);
                img.save(&buffer, "PNG"); // writes the image in PNG format inside the buffer

                QString html;
                html = "<img src=\"data:image/png;base64,"; //   (file:///.+)\"
                html += RB_String::fromLatin1(ba.toBase64().data());
                html += "\" />";
                QTextEdit::textCursor().insertHtml(html);
            }
        }
    }
}

/**
 * Drop a text file based on an url or file location
 * @param url file location
 */
void RB_TextEdit::dropTextFile(const QUrl& url) {
    QFile file(url.toLocalFile());
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextEdit::textCursor().insertText(file.readAll());
    }
}

/**
 * @brief Set default format for this text document
 */
void RB_TextEdit::setDefaultFormat() {
    // Default font size
//    QFont font = this->document()->defaultFont();
//    font.setPointSize(11);
//    this->document()->setDefaultFont(font);
    // Alternative
    QFont f = font();
    int newSize = 8;
    f.setPointSize(newSize);
    setFont(f);

    // Default block margins, always one text block available at construction
    QTextCursor tc = this->textCursor();
    QTextBlock tb = tc.block();
    QTextBlockFormat tbf = tb.blockFormat();
    tbf.setBottomMargin(6);
    tbf.setTopMargin(6);
    tc.setBlockFormat(tbf);
}

void RB_TextEdit::setPaletteColors() {
    mAlternateBaseColor = QApplication::palette().color(QPalette::Active,
                                                        QPalette::AlternateBase);
    mBaseColor = QApplication::palette().color(QPalette::Active,
                                               QPalette::Base);
    mTextColor = QApplication::palette().color(QPalette::Active,
                                               QPalette::Text);
}

/**
 * Completion for keyPressedEvent of 'limiting' characters
 */
void RB_TextEdit::handleLeftLimitCharPressed() {
    // Handles keyPressEvent in Editor class if key == Qt::Key_ParenLeft
    // and signal emitted for this widget to catch, and key pad

    // cursor positions
    QTextCursor cursor = textCursor();

    int openCharPos = 0;
    int closeCharPos = 0;

    if (cursor.anchor() < cursor.position()) {
        openCharPos = cursor.anchor();
        closeCharPos = cursor.position();
    } else {
        openCharPos = cursor.position();
        closeCharPos = cursor.anchor();
    }

    // check for number of right parenthese at the right
    cursor.setPosition(openCharPos, QTextCursor::MoveAnchor);
    cursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor, 1);
    setTextCursor(cursor);

    QString str = cursor.selectedText();
    int left = 0;
    int right = 0;

    QChar leftChar = mLeftLimitChar.at(0);
    QChar rightChar = mRightLimitChar.at(0);

    for (int i = 0; i < str.size(); ++i) {
        if (str.at(i) == leftChar) {
            left += 1;
        } else if (str.at(i) == rightChar) {
            right += 1;
        }
    }

    cursor.setPosition(openCharPos, QTextCursor::MoveAnchor);
    cursor.setPosition(closeCharPos, QTextCursor::KeepAnchor);
    setTextCursor(cursor);

    if (cursor.hasSelection()) {
        // selection of text prevails above left/right mismatch
        cursor.setPosition(closeCharPos, QTextCursor::MoveAnchor);
        setTextCursor(cursor);
        cursor.insertText(mRightLimitChar);
        cursor.setPosition(openCharPos, QTextCursor::MoveAnchor);
        setTextCursor(cursor);
        cursor.insertText(mLeftLimitChar);
        cursor.setPosition(closeCharPos + 2, QTextCursor::MoveAnchor);
        setTextCursor(cursor);
    } else if (left < right) {
        // complete the left parenthese only
        cursor.insertText(mLeftLimitChar);
    } else {
        cursor.insertText(mLeftLimitChar + mRightLimitChar);
        cursor.setPosition(cursor.position() - 1);
        setTextCursor(cursor);
    }
}
