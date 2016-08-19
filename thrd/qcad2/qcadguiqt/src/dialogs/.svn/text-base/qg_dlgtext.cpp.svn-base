/****************************************************************************
**
** Copyright (C) 2001-2003 RibbonSoft. All rights reserved.
**
** This file is part of the qcadlib Library project.
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.
**
** Licensees holding valid qcadlib Professional Edition licenses may use 
** this file in accordance with the qcadlib Commercial License
** Agreement provided with the Software.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** See http://www.ribbonsoft.com for further details.
**
** Contact info@ribbonsoft.com if any conditions of this licensing are
** not clear to you.
**
**********************************************************************/

#include "qg_dlgtext.h"

#include <QLocale>
#include <QTextCodec>
#include <QTextStream>
#include <QFileDialog>

#include "rs_debug.h"
#include "rs_settings.h"
#include "rs_system.h"
#include "rs_text.h"
#include "rs_filterdxf.h"

/**
 * Constructor
 */
QG_DlgText::QG_DlgText(QWidget* parent)
        : QDialog(parent) {
    RS_DEBUG->print("QG_DlgText::QG_DlgText");
    setupUi(this);
            
    cbSymbol->addItem(QString("%1 (%2)").arg(QChar(248)).arg(tr("Diameter")));
    cbSymbol->addItem(QString("%1 (%2)").arg(QChar(176)).arg(tr("Degree")));
    cbSymbol->addItem(QString("%1 (%2)").arg(QChar(177)).arg(tr("Plus / Minus")));
    cbSymbol->addItem(QString("%1 (%2)").arg(QChar(960)).arg(tr("Pi")));
    cbSymbol->addItem(QString("%1 (%2)").arg(QChar(215)).arg(tr("Times")));
    cbSymbol->addItem(QString("%1 (%2)").arg(QChar(247)).arg(tr("Division")));
    cbSymbol->addItem(QString("%1 (%2)").arg(QChar('@')).arg(tr("At")));
    cbSymbol->addItem(QString("%1 (%2)").arg(QChar('#')).arg(tr("Hash")));
    cbSymbol->addItem(QString("%1 (%2)").arg(QChar('$')).arg(tr("Dollar")));
    cbSymbol->addItem(QString("%1 (%2)").arg(QChar(169)).arg(tr("Copyright")));
    cbSymbol->addItem(QString("%1 (%2)").arg(QChar(174)).arg(tr("Registered")));
    cbSymbol->addItem(QString("%1 (%2)").arg(QChar(167)).arg(tr("Paragraph")));
    cbSymbol->addItem(QString("%1 (%2)").arg(QChar(163)).arg(tr("Pound")));
    cbSymbol->addItem(QString("%1 (%2)").arg(QChar(165)).arg(tr("Yen")));
    cbSymbol->addItem(QString("%1 (%2)").arg(QChar(0x03A9)).arg(tr("Ohm")));
    cbSymbol->addItem(QString("%1 (%2)").arg(QChar(0x03BC)).arg(tr("Micro")));
            
    cbUniPage->addItem(QString("[0000-007F] %1").arg(tr("Basic Latin")));
    cbUniPage->addItem(QString("[0080-00FF] %1").arg(tr("Latin-1 Supplementary")));
    cbUniPage->addItem(QString("[0100-017F] %1").arg(tr("Latin Extended-A")));
    cbUniPage->addItem(QString("[0180-024F] %1").arg(tr("Latin Extended-B")));
    cbUniPage->addItem(QString("[0250-02AF] %1").arg(tr("IPA Extensions")));
    cbUniPage->addItem(QString("[02B0-02FF] %1").arg(tr("Spacing Modifier Letters")));
    cbUniPage->addItem(QString("[0300-036F] %1").arg(tr("Combining Diacritical Marks")));
    cbUniPage->addItem(QString("[0370-03FF] %1").arg(tr("Greek and Coptic")));
    cbUniPage->addItem(QString("[0400-04FF] %1").arg(tr("Cyrillic")));
    cbUniPage->addItem(QString("[0500-052F] %1").arg(tr("Cyrillic Supplementary")));
    cbUniPage->addItem(QString("[0530-058F] %1").arg(tr("Armenian")));
    cbUniPage->addItem(QString("[0590-05FF] %1").arg(tr("Hebrew")));
    cbUniPage->addItem(QString("[0600-06FF] %1").arg(tr("Arabic")));
    cbUniPage->addItem(QString("[0700-074F] %1").arg(tr("Syriac")));
    cbUniPage->addItem(QString("[0780-07BF] %1").arg(tr("Thaana")));
    cbUniPage->addItem(QString("[0900-097F] %1").arg(tr("Devanagari")));
    cbUniPage->addItem(QString("[0980-09FF] %1").arg(tr("Bengali")));
    cbUniPage->addItem(QString("[0A00-0A7F] %1").arg(tr("Gurmukhi")));
    cbUniPage->addItem(QString("[0A80-0AFF] %1").arg(tr("Gujarati")));
    cbUniPage->addItem(QString("[0B00-0B7F] %1").arg(tr("Oriya")));
    cbUniPage->addItem(QString("[0B80-0BFF] %1").arg(tr("Tamil")));
    cbUniPage->addItem(QString("[0C00-0C7F] %1").arg(tr("Telugu")));
    cbUniPage->addItem(QString("[0C80-0CFF] %1").arg(tr("Kannada")));
    cbUniPage->addItem(QString("[0D00-0D7F] %1").arg(tr("Malayalam")));
    cbUniPage->addItem(QString("[0D80-0DFF] %1").arg(tr("Sinhala")));
    cbUniPage->addItem(QString("[0E00-0E7F] %1").arg(tr("Thai")));
    cbUniPage->addItem(QString("[0E80-0EFF] %1").arg(tr("Lao")));
    cbUniPage->addItem(QString("[0F00-0FFF] %1").arg(tr("Tibetan")));
    cbUniPage->addItem(QString("[1000-109F] %1").arg(tr("Myanmar")));
    cbUniPage->addItem(QString("[10A0-10FF] %1").arg(tr("Georgian")));
    cbUniPage->addItem(QString("[1100-11FF] %1").arg(tr("Hangul Jamo")));
    cbUniPage->addItem(QString("[1200-137F] %1").arg(tr("Ethiopic")));
    cbUniPage->addItem(QString("[13A0-13FF] %1").arg(tr("Cherokee")));
    cbUniPage->addItem(QString("[1400-167F] %1").arg(tr("Unified Canadian Aboriginal Syllabic")));
    cbUniPage->addItem(QString("[1680-169F] %1").arg(tr("Ogham")));
    cbUniPage->addItem(QString("[16A0-16FF] %1").arg(tr("Runic")));
    cbUniPage->addItem(QString("[1700-171F] %1").arg(tr("Tagalog")));
    cbUniPage->addItem(QString("[1720-173F] %1").arg(tr("Hanunoo")));
    cbUniPage->addItem(QString("[1740-175F] %1").arg(tr("Buhid")));
    cbUniPage->addItem(QString("[1760-177F] %1").arg(tr("Tagbanwa")));
    cbUniPage->addItem(QString("[1780-17FF] %1").arg(tr("Khmer")));
    cbUniPage->addItem(QString("[1800-18AF] %1").arg(tr("Mongolian")));
    cbUniPage->addItem(QString("[1E00-1EFF] %1").arg(tr("Latin Extended Additional")));
    cbUniPage->addItem(QString("[1F00-1FFF] %1").arg(tr("Greek Extended")));
    cbUniPage->addItem(QString("[2000-206F] %1").arg(tr("General Punctuation")));
    cbUniPage->addItem(QString("[2070-209F] %1").arg(tr("Superscripts and Subscripts")));
    cbUniPage->addItem(QString("[20A0-20CF] %1").arg(tr("Currency Symbols")));
    cbUniPage->addItem(QString("[20D0-20FF] %1").arg(tr("Combining Marks for Symbols")));
    cbUniPage->addItem(QString("[2100-214F] %1").arg(tr("Letterlike Symbols")));
    cbUniPage->addItem(QString("[2150-218F] %1").arg(tr("Number Forms")));
    cbUniPage->addItem(QString("[2190-21FF] %1").arg(tr("Arrows")));
    cbUniPage->addItem(QString("[2200-22FF] %1").arg(tr("Mathematical Operators")));
    cbUniPage->addItem(QString("[2300-23FF] %1").arg(tr("Miscellaneous Technical")));
    cbUniPage->addItem(QString("[2400-243F] %1").arg(tr("Control Pictures")));
    cbUniPage->addItem(QString("[2440-245F] %1").arg(tr("Optical Character Recognition")));
    cbUniPage->addItem(QString("[2460-24FF] %1").arg(tr("Enclosed Alphanumerics")));
    cbUniPage->addItem(QString("[2500-257F] %1").arg(tr("Box Drawing")));
    cbUniPage->addItem(QString("[2580-259F] %1").arg(tr("Block Elements")));
    cbUniPage->addItem(QString("[25A0-25FF] %1").arg(tr("Geometric Shapes")));
    cbUniPage->addItem(QString("[2600-26FF] %1").arg(tr("Miscellaneous Symbols")));
    cbUniPage->addItem(QString("[2700-27BF] %1").arg(tr("Dingbats")));
    cbUniPage->addItem(QString("[27C0-27EF] %1").arg(tr("Miscellaneous Mathematical Symbols-A")));
    cbUniPage->addItem(QString("[27F0-27FF] %1").arg(tr("Supplemental Arrows-A")));
    cbUniPage->addItem(QString("[2800-28FF] %1").arg(tr("Braille Patterns")));
    cbUniPage->addItem(QString("[2900-297F] %1").arg(tr("Supplemental Arrows-B")));
    cbUniPage->addItem(QString("[2980-29FF] %1").arg(tr("Miscellaneous Mathematical Symbols-B")));
    cbUniPage->addItem(QString("[2A00-2AFF] %1").arg(tr("Supplemental Mathematical Operators")));
    cbUniPage->addItem(QString("[2E80-2EFF] %1").arg(tr("CJK Radicals Supplement")));
    cbUniPage->addItem(QString("[2F00-2FDF] %1").arg(tr("Kangxi Radicals")));
    cbUniPage->addItem(QString("[2FF0-2FFF] %1").arg(tr("Ideographic Description Characters")));
    cbUniPage->addItem(QString("[3000-303F] %1").arg(tr("CJK Symbols and Punctuation")));
    cbUniPage->addItem(QString("[3040-309F] %1").arg(tr("Hiragana")));
    cbUniPage->addItem(QString("[30A0-30FF] %1").arg(tr("Katakana")));
    cbUniPage->addItem(QString("[3100-312F] %1").arg(tr("Bopomofo")));
    cbUniPage->addItem(QString("[3130-318F] %1").arg(tr("Hangul Compatibility Jamo")));
    cbUniPage->addItem(QString("[3190-319F] %1").arg(tr("Kanbun")));
    cbUniPage->addItem(QString("[31A0-31BF] %1").arg(tr("Bopomofo Extended")));
    cbUniPage->addItem(QString("[3200-32FF] %1").arg(tr("Enclosed CJK Letters and Months")));
    cbUniPage->addItem(QString("[3300-33FF] %1").arg(tr("CJK Compatibility")));
    cbUniPage->addItem(QString("[3400-4DBF] %1").arg(tr("CJK Unified Ideographs Extension A")));
    cbUniPage->addItem(QString("[4E00-9FAF] %1").arg(tr("CJK Unified Ideographs")));
    cbUniPage->addItem(QString("[A000-A48F] %1").arg(tr("Yi Syllables")));
    cbUniPage->addItem(QString("[A490-A4CF] %1").arg(tr("Yi Radicals")));
    cbUniPage->addItem(QString("[AC00-D7AF] %1").arg(tr("Hangul Syllables")));
    cbUniPage->addItem(QString("[D800-DBFF] %1").arg(tr("High Surrogates")));
    cbUniPage->addItem(QString("[DC00-DFFF] %1").arg(tr("Low Surrogate Area")));
    cbUniPage->addItem(QString("[E000-F8FF] %1").arg(tr("Private Use Area")));
    cbUniPage->addItem(QString("[F900-FAFF] %1").arg(tr("CJK Compatibility Ideographs")));
    cbUniPage->addItem(QString("[FB00-FB4F] %1").arg(tr("Alphabetic Presentation Forms")));
    cbUniPage->addItem(QString("[FB50-FDFF] %1").arg(tr("Arabic Presentation Forms-A")));
    cbUniPage->addItem(QString("[FE00-FE0F] %1").arg(tr("Variation Selectors")));
    cbUniPage->addItem(QString("[FE20-FE2F] %1").arg(tr("Combining Half Marks")));
    cbUniPage->addItem(QString("[FE30-FE4F] %1").arg(tr("CJK Compatibility Forms")));
    cbUniPage->addItem(QString("[FE50-FE6F] %1").arg(tr("Small Form Variants")));
    cbUniPage->addItem(QString("[FE70-FEFF] %1").arg(tr("Arabic Presentation Forms-B")));
    cbUniPage->addItem(QString("[FF00-FFEF] %1").arg(tr("Halfwidth and Fullwidth Forms")));
    cbUniPage->addItem(QString("[FFF0-FFFF] %1").arg(tr("Specials")));
    cbUniPage->addItem(QString("[10300-1032F] %1").arg(tr("Old Italic")));
    cbUniPage->addItem(QString("[10330-1034F] %1").arg(tr("Gothic")));
    cbUniPage->addItem(QString("[10400-1044F] %1").arg(tr("Deseret")));
    cbUniPage->addItem(QString("[1D000-1D0FF] %1").arg(tr("Byzantine Musical Symbols")));
    cbUniPage->addItem(QString("[1D100-1D1FF] %1").arg(tr("Musical Symbols")));
    cbUniPage->addItem(QString("[1D400-1D7FF] %1").arg(tr("Mathematical Alphanumeric Symbols")));
    cbUniPage->addItem(QString("[20000-2A6DF] %1").arg(tr("CJK Unified Ideographs Extension B")));
    cbUniPage->addItem(QString("[2F800-2FA1F] %1").arg(tr("CJK Compatibility Ideographs Supplement")));
    cbUniPage->addItem(QString("[E0000-E007F] %1").arg(tr("Tags")));
    cbUniPage->addItem(QString("[F0000-FFFFD] %1").arg(tr("Supplementary Private Use Area-A")));
    cbUniPage->addItem(QString("[100000-10FFFD] %1").arg(tr("Supplementary Private Use Area-B")));

    connect(bTL, SIGNAL(clicked()),
        this, SLOT(setAlignmentTL()));
    connect(bTC, SIGNAL(clicked()),
        this, SLOT(setAlignmentTC()));
    connect(bTR, SIGNAL(clicked()),
        this, SLOT(setAlignmentTR()));
    connect(bML, SIGNAL(clicked()),
        this, SLOT(setAlignmentML()));
    connect(bMC, SIGNAL(clicked()),
        this, SLOT(setAlignmentMC()));
    connect(bMR, SIGNAL(clicked()),
        this, SLOT(setAlignmentMR()));
    connect(bBL, SIGNAL(clicked()),
        this, SLOT(setAlignmentBL()));
    connect(bBC, SIGNAL(clicked()),
        this, SLOT(setAlignmentBC()));
    connect(bBR, SIGNAL(clicked()),
        this, SLOT(setAlignmentBR()));
    connect(cbDefault, SIGNAL(toggled(bool)),
        leLineSpacingFactor, SLOT(setDisabled(bool)));
    connect(cbDefault, SIGNAL(toggled(bool)),
        this, SLOT(defaultChanged(bool)));
    connect(bClear, SIGNAL(clicked()),
        teText, SLOT(clear()));
    connect(bCut, SIGNAL(clicked()),
        teText, SLOT(cut()));
    connect(bCopy, SIGNAL(clicked()),
        teText, SLOT(copy()));
    connect(bPaste, SIGNAL(clicked()),
        teText, SLOT(paste()));
    connect(bLoad, SIGNAL(clicked()),
        this, SLOT(loadText()));
    connect(bSave, SIGNAL(clicked()),
        this, SLOT(saveText()));
    connect(cbUniPage, SIGNAL(activated(int)),
        this, SLOT(updateUniCharComboBox(int)));
    connect(bUnicode, SIGNAL(clicked()),
        this, SLOT(insertChar()));
    connect(cbUniPage, SIGNAL(activated(int)),
        this, SLOT(updateUniCharButton(int)));
    connect(cbUniChar, SIGNAL(activated(int)),
        this, SLOT(updateUniCharButton(int)));
    connect(cbSymbol, SIGNAL(activated(const QString&)),
            this, SLOT(insertSymbol(const QString&)));
    
    RS_DEBUG->print("QG_DlgText::QG_DlgText: connections OK");
    
    cbFont->init();
    font=NULL;
    text = NULL;
    isNew = false;
    
    updateUniCharComboBox(0);
    updateUniCharButton(0);
    
    RS_DEBUG->print("QG_DlgText::QG_DlgText: OK");
}


/**
 * Destructor
 */
QG_DlgText::~QG_DlgText() {
    if (isNew) {
        RS_SETTINGS->beginGroup("/Draw");
        RS_SETTINGS->writeEntry("/TextHeight", leHeight->text());
        RS_SETTINGS->writeEntry("/TextFont", cbFont->currentText());
        RS_SETTINGS->writeEntry("/TextDefault", (int)cbDefault->isChecked());
        RS_SETTINGS->writeEntry("/TextAlignment", getAlignment());
        //RS_SETTINGS->writeEntry("/TextLetterSpacing", leLetterSpacing->text());
        //RS_SETTINGS->writeEntry("/TextWordSpacing", leWordSpacing->text());
        RS_SETTINGS->writeEntry("/TextLineSpacingFactor",
                                leLineSpacingFactor->text());
        RS_SETTINGS->writeEntry("/TextString", teText->toPlainText());
        //RS_SETTINGS->writeEntry("/TextShape", getShape());
        RS_SETTINGS->writeEntry("/TextAngle", leAngle->text());
        //RS_SETTINGS->writeEntry("/TextRadius", leRadius->text());
        RS_SETTINGS->endGroup();
    }
}


void QG_DlgText::updateUniCharComboBox(int) {
    RS_DEBUG->print("QG_DlgText::updateUniCharComboBox");
    QString t = cbUniPage->currentText();
    int i1 = t.indexOf('-');
    int i2 = t.indexOf(']');
    int min = t.mid(1, i1-1).toInt(NULL, 16);
    int max = t.mid(i1+1, i2-i1-1).toInt(NULL, 16);

    cbUniChar->clear();
    for (int c=min; c<=max; c++) {
        char buf[5];
        sprintf(buf, "%04X", c);
        cbUniChar->addItem(QString("[%1] %2").arg(buf).arg(QChar(c)));
    }
    RS_DEBUG->print("QG_DlgText::updateUniCharComboBox: OK");
}


/**
 * Sets the text entity represented by this dialog.
 */
void QG_DlgText::setText(RS_Text& t, bool isNew) {
    RS_DEBUG->print("QG_DlgText::setText");
    text = &t;
    this->isNew = isNew;

    QString fon;
    QString height;
    QString def;
    QString alignment;
    //QString letterSpacing;
    //QString wordSpacing;
    QString lineSpacingFactor;
    QString str;
    //QString shape;
    QString angle;

    if (isNew) {
        RS_SETTINGS->beginGroup("/Draw");
        //default font depending on locale
        QString iso = RS_System::localeToISO( QLocale::system().name() );
        if (iso=="ISO8859-1") {
             fon = RS_SETTINGS->readEntry("/TextFont", "standard");
        } else if (iso=="ISO8859-2") {
             fon = RS_SETTINGS->readEntry("/TextFont", "normallatin2");
        } else if (iso=="ISO8859-7") {
             fon = RS_SETTINGS->readEntry("/TextFont", "greekc");
        } else if (iso=="KOI8-U" || iso=="KOI8-R") {
             fon = RS_SETTINGS->readEntry("/TextFont", "cyrillic_ii");
        } else {
             fon = RS_SETTINGS->readEntry("/TextFont", "standard");
        }
        height = RS_SETTINGS->readEntry("/TextHeight", "1.0");
        def = RS_SETTINGS->readEntry("/TextDefault", "1");
        alignment = RS_SETTINGS->readEntry("/TextAlignment", "1");
        //letterSpacing = RS_SETTINGS->readEntry("/TextLetterSpacing", "0");
        //wordSpacing = RS_SETTINGS->readEntry("/TextWordSpacing", "0");
        lineSpacingFactor = RS_SETTINGS->readEntry("/TextLineSpacingFactor", "1");
        str = RS_SETTINGS->readEntry("/TextString", "");
        //shape = RS_SETTINGS->readEntry("/TextShape", "0");
        angle = RS_SETTINGS->readEntry("/TextAngle", "0");
        //radius = RS_SETTINGS->readEntry("/TextRadius", "10");
        RS_SETTINGS->endGroup();
    } else {
        fon = text->getStyle();
        setFont(fon);
        height = QString("%1").arg(text->getHeight());
        if (font!=NULL) {
            if (font->getLineSpacingFactor()==text->getLineSpacingFactor()) {
                def = "1";
            } else {
                def = "0";
            }
        }
        alignment = QString("%1").arg(text->getAlignment());
        //QString letterSpacing = RS_SETTINGS->readEntry("/TextLetterSpacing", "0");
        //QString wordSpacing = RS_SETTINGS->readEntry("/TextWordSpacing", "0");
        lineSpacingFactor = QString("%1").arg(text->getLineSpacingFactor());

/* // Doesn't make sense. We don't want to show native DXF strings in the Dialog.
#if defined(OOPL_VERSION) && defined(Q_WS_WIN) 
        QCString iso = RS_System::localeToISO( QTextCodec::locale() );
        QTextCodec *codec = QTextCodec::codecForName(iso);
        if (codec!=NULL) {
            str = codec->toUnicode(RS_FilterDxf::toNativeString(text->getText().local8Bit()));
        } else {
            str = RS_FilterDxf::toNativeString(text->getText().local8Bit());
        }
#else*/
       str = text->getText();
//#endif
        //QString shape = RS_SETTINGS->readEntry("/TextShape", "0");
        angle = QString("%1").arg(RS_Math::rad2deg(text->getAngle()));
    }

    cbDefault->setChecked(def=="1");
    setFont(fon);
    leHeight->setText(height);
    setAlignment(alignment.toInt());
    if (def!="1" || font==NULL) {
        //leLetterSpacing->setText(letterSpacing);
        //leWordSpacing->setText(wordSpacing);
        leLineSpacingFactor->setText(lineSpacingFactor);
    } else {
        //leLetterSpacing->setText(font->getLetterSpacing());
        //leWordSpacing->setText(font->getWordSpacing());
        leLineSpacingFactor->setText(
            QString("%1").arg(font->getLineSpacingFactor()));
    }
    teText->setPlainText(str);
    //setShape(shape.toInt());
    leAngle->setText(angle);
    //leRadius->setText(radius);
    teText->setFocus();
    teText->selectAll();
    
    RS_DEBUG->print("QG_DlgText::setText: OK");
}


/**
 * Updates the text entity represented by the dialog to fit the choices of the user.
 */
void QG_DlgText::updateText() {
    RS_DEBUG->print("QG_DlgText::updateText");
    if (text!=NULL) {
        text->setStyle(cbFont->currentText());
        text->setHeight(leHeight->text().toDouble());

// 'fix' for windows (causes troubles if locale returns en_us):
/*#if defined(OOPL_VERSION) && defined(Q_WS_WIN)
        QCString iso = RS_System::localeToISO( QTextCodec::locale() );
        text->setPlainText(
            RS_FilterDxf::toNativeString( 
             QString::fromLocal8Bit( QTextCodec::codecForName( iso )->fromUnicode( teText->toPlainText() ) )
            )
        );
#else*/
        text->setText(RS_FilterDxf::toNativeString(teText->toPlainText()));
//#endif
        //text->setLetterSpacing(leLetterSpacing.toDouble());
        text->setLineSpacingFactor(leLineSpacingFactor->text().toDouble());
        text->setAlignment(getAlignment());
        text->setAngle(RS_Math::deg2rad(leAngle->text().toDouble()));
    }
    RS_DEBUG->print("QG_DlgText::updateText: OK");
}


void QG_DlgText::setAlignmentTL() {
    setAlignment(1);
}

void QG_DlgText::setAlignmentTC() {
    setAlignment(2);
}

void QG_DlgText::setAlignmentTR() {
    setAlignment(3);
}

void QG_DlgText::setAlignmentML() {
    setAlignment(4);
}

void QG_DlgText::setAlignmentMC() {
    setAlignment(5);
}

void QG_DlgText::setAlignmentMR() {
    setAlignment(6);
}

void QG_DlgText::setAlignmentBL() {
    setAlignment(7);
}

void QG_DlgText::setAlignmentBC() {
    setAlignment(8);
}

void QG_DlgText::setAlignmentBR() {
    setAlignment(9);
}

void QG_DlgText::setAlignment(int a) {
    bTL->setChecked(false);
    bTC->setChecked(false);
    bTR->setChecked(false);
    bML->setChecked(false);
    bMC->setChecked(false);
    bMR->setChecked(false);
    bBL->setChecked(false);
    bBC->setChecked(false);
    bBR->setChecked(false);

    switch (a) {
    case 1:
        bTL->setChecked(true);
        break;
    case 2:
        bTC->setChecked(true);
        break;
    case 3:
        bTR->setChecked(true);
        break;
    case 4:
        bML->setChecked(true);
        break;
    case 5:
        bMC->setChecked(true);
        break;
    case 6:
        bMR->setChecked(true);
        break;
    case 7:
        bBL->setChecked(true);
        break;
    case 8:
        bBC->setChecked(true);
        break;
    case 9:
        bBR->setChecked(true);
        break;
    default:
        break;
    }
}

int QG_DlgText::getAlignment() {
    if (bTL->isChecked()) {
        return 1;
    } else if (bTC->isChecked()) {
        return 2;
    } else if (bTR->isChecked()) {
        return 3;
    } else if (bML->isChecked()) {
        return 4;
    } else if (bMC->isChecked()) {
        return 5;
    } else if (bMR->isChecked()) {
        return 6;
    } else if (bBL->isChecked()) {
        return 7;
    } else if (bBC->isChecked()) {
        return 8;
    } else if (bBR->isChecked()) {
        return 9;
    }

    return 1;
}

void QG_DlgText::setFont(const QString& f) {
    RS_DEBUG->print("QG_DlgText::setFont");
    
    cbFont->setCurrentIndex(cbFont->findText(f));
    font = cbFont->getFont();
    defaultChanged(false);
    
    RS_DEBUG->print("QG_DlgText::setFont: OK");
}

/*
void QG_DlgText::setShape(int s) {
    switch (s) {
    case 0:
        rbStraight->setChecked(true);
        break;
    case 1:
        rbRound1->setChecked(true);
        break;
    case 2:
        rbRound2->setChecked(true);
        break;
    default:
        break;
    }
}
 
int QG_DlgText::getShape() {
    if (rbStraight->isChecked()) {
        return 0;
    } else if (rbRound1->isChecked()) {
        return 1;
    } else if (rbRound2->isChecked()) {
        return 2;
    }
    return 1;
}
*/

void QG_DlgText::defaultChanged(bool) {
    if (cbDefault->isChecked() && font!=NULL) {
        leLineSpacingFactor->setText(
            QString("%1").arg(font->getLineSpacingFactor()));
    }
}

void QG_DlgText::loadText() {
    QString fn = QFileDialog::getOpenFileName(NULL, tr("Load Text"));
    if (!fn.isEmpty()) {
        load(fn);
    }
}

void QG_DlgText::load(const QString& fn) {
    QFile f(fn);
    if (!f.open(QIODevice::ReadOnly)) {
        return;
    }

    QTextStream ts(&f);
    ts.setCodec("UTF-8");
    teText->setPlainText(ts.readAll());
    f.close();
}

void QG_DlgText::saveText() {
    QString fn = QFileDialog::getSaveFileName(this, tr("Save Text"));
    if (!fn.isEmpty()) {
        save(fn);
    }
}

void QG_DlgText::save(const QString& fn) {
    QString text = teText->toPlainText();
    QFile f(fn);
    if (f.open(QIODevice::WriteOnly)) {
        QTextStream t(&f);
        t << text;
        f.close();
    }
}

void QG_DlgText::insertSymbol(const QString& s) {
    teText->insertPlainText(s.left(1));
}

void QG_DlgText::updateUniCharButton(int) {
    QString t = cbUniChar->currentText();
    int i1 = t.indexOf(']');
    int c = t.mid(1, i1-1).toInt(NULL, 16);
    bUnicode->setText(QString("%1").arg(QChar(c)));
}

void QG_DlgText::insertChar() {
    QString t = cbUniChar->currentText();
    int i1 = t.indexOf(']');
    int c = t.mid(1, i1-1).toInt(NULL, 16);
    teText->insertPlainText(QString("%1").arg(QChar(c)));
}

