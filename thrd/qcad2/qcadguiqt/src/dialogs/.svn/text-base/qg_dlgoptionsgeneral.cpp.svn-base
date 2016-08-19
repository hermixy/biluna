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

#include "qg_dlgoptionsgeneral.h"

#include <QMessageBox>
#include <QColorDialog>

#include "rs_settings.h"
#include "rs_system.h"
#include "rs.h"
#include "rs_units.h"

/**
 * Constructor
 */
QG_DlgOptionsGeneral::QG_DlgOptionsGeneral(QWidget* parent)
        : QDialog(parent) {
    setupUi(this);

    cbMinGridSpacing->addItem("0");
    cbMinGridSpacing->addItem("4");
    cbMinGridSpacing->addItem("5");
    cbMinGridSpacing->addItem("6");
    cbMinGridSpacing->addItem("8");
    cbMinGridSpacing->addItem("10");
    cbMinGridSpacing->addItem("15");
    cbMinGridSpacing->addItem("20");

    cbMaxPreview->addItem("0");
    cbMaxPreview->addItem("50");
    cbMaxPreview->addItem("100");
    cbMaxPreview->addItem("200");

    cbSizeStatus->addItem("5");
    cbSizeStatus->addItem("6");
    cbSizeStatus->addItem("7");
    cbSizeStatus->addItem("8");
    cbSizeStatus->addItem("9");
    cbSizeStatus->addItem("10");
    cbSizeStatus->addItem("11");
    cbSizeStatus->addItem("12");
    cbSizeStatus->addItem("13");
    cbSizeStatus->addItem("14");

    /*
    cbBackgroundColor->addItem("#000000");
    cbGridColor->addItem("#c0c0c0");
    cbMetaGridColor->addItem("#404040");
    cbSelectedColor->addItem("#a54747");
    cbHighlightedColor->addItem("#739373");
    cbPreviewColor->addItem("#7f0000");
    */
    
    connect(okButton, SIGNAL(clicked()),
            this, SLOT(ok()));
    connect(cbSizeStatus, SIGNAL(activated(int)),
            this, SLOT(setRestartNeeded()));
    /*
    connect(cbLanguageCmd, SIGNAL(activated(int)),
            this, SLOT(setRestartNeeded()));
    */
    connect(cbLanguage, SIGNAL(activated(int)),
            this, SLOT(setRestartNeeded()));
    connect(cbShowKeycodes, SIGNAL(stateChanged(int)),
            this, SLOT(setRestartNeeded()));
    
    connect(tbBackgroundColor, SIGNAL(clicked(bool)),
        this, SLOT(chooseBackgroundColor()));
    connect(tbGridColor, SIGNAL(clicked(bool)),
        this, SLOT(chooseGridColor()));
    connect(tbMetaGridColor, SIGNAL(clicked(bool)),
        this, SLOT(chooseMetaGridColor()));
    connect(tbSelectedColor, SIGNAL(clicked(bool)),
        this, SLOT(chooseSelectedColor()));
    connect(tbHighlightedColor, SIGNAL(clicked(bool)),
        this, SLOT(chooseHighlightedColor()));
    connect(tbPreviewColor, SIGNAL(clicked(bool)),
        this, SLOT(choosePreviewColor()));

#ifdef QC_PREDEFINED_LOCALE
    cbLanguage->hide();
//    Widget9Layout->addMultiCellWidget( bgGraphicView, 0, 2, 0, 0 ); //use empty space as well
#endif

    // Fill combobox with languages:
    QStringList languageList = RS_SYSTEM->getLanguageList();
    languageList.prepend("en");
    QStringList languageNames;
    for (RS_StringList::Iterator it = languageList.begin();
            it!=languageList.end();
            it++) {

        RS_DEBUG->print("QG_DlgOptionsGeneral::init: adding %s to combobox",
                        (const char*)(*it).toLatin1());

        QString l = RS_SYSTEM->symbolToLanguage(*it);
        if (l.isEmpty()==false) {
            RS_DEBUG->print("QG_DlgOptionsGeneral::init: %s", (const char*)l.toLatin1());
            if (!languageNames.contains(l)) {
                languageNames << l;
            }
        }
    }
    // sort:
    languageNames.sort();

    for (RS_StringList::Iterator it2 = languageNames.begin();
            it2!=languageNames.end();
            it2++) {
        
        cbLanguage->addItem(*it2);
        //cbLanguageCmd->addItem(*it2);
    }
        

    RS_SETTINGS->beginGroup("/Appearance");

    // set current language:
    QString def_lang = "en";

#ifdef QC_PREDEFINED_LOCALE
    def_lang = QC_PREDEFINED_LOCALE;
#endif

    QString lang = RS_SETTINGS->readEntry("/Language", def_lang);
    cbLanguage->setCurrentIndex(
        cbLanguage->findText(RS_SYSTEM->symbolToLanguage(lang)));

    /*
    QString langCmd = RS_SETTINGS->readEntry("/LanguageCmd", def_lang);
    cbLanguageCmd->setCurrentIndex(cbLanguageCmd->findText(RS_SYSTEM->symbolToLanguage(langCmd)));
    */

    // graphic view:
    // rulers
    QString showRulers = RS_SETTINGS->readEntry("/ShowRulers", "1");
    cbShowRulers->setChecked(showRulers=="1");
    // crosshairs:
    QString showCrosshairs = RS_SETTINGS->readEntry("/ShowCrosshairs", "1");
    cbShowCrosshairs->setChecked(showCrosshairs=="1");

    // grid:
    QString scaleGrid = RS_SETTINGS->readEntry("/ScaleGrid", "1");
    cbScaleGrid->setChecked(scaleGrid=="1");
    QString minGridSpacing = RS_SETTINGS->readEntry("/MinGridSpacing", "10");
    cbMinGridSpacing->setEditText(minGridSpacing);
    QString solidGridLines = RS_SETTINGS->readEntry("/SolidGridLines", 
#ifdef Q_OS_MACX
    "1"
#else
    "0"
#endif
    );
    cbSolidGridLines->setChecked(solidGridLines=="1");

    // preview:
    QString maxPreview = RS_SETTINGS->readEntry("/MaxPreview", "100");
    cbMaxPreview->setEditText(maxPreview);


    // colors:
    QString color = RS_SETTINGS->readEntry("/BackgroundColor", "Black");
    setColorPreview(*lBackgroundColorPreview, color);
    //cbBackgroundColor->setEditText(color);
    color = RS_SETTINGS->readEntry("/GridColor", "Gray");
    setColorPreview(*lGridColorPreview, color);
    //cbGridColor->setEditText(gridColor);
    color = RS_SETTINGS->readEntry("/MetaGridColor", "#404040");
    setColorPreview(*lMetaGridColorPreview, color);
    //cbMetaGridColor->setEditText(metaGridColor);
    color = RS_SETTINGS->readEntry("/SelectedColor", "#a54747");
    setColorPreview(*lSelectedColorPreview, color);
    //cbSelectedColor->setEditText(selectedColor);
    color = RS_SETTINGS->readEntry("/HighlightedColor", "#739373");
    setColorPreview(*lHighlightedColorPreview, color);
    //cbHighlightedColor->setEditText(highlightedColor);
    color = RS_SETTINGS->readEntry("/PreviewColor", "#7F0000");
    setColorPreview(*lPreviewColorPreview, color);
    //cbPreviewColor->setEditText(previewColor);

    // font size:
    QString sizeStatus = RS_SETTINGS->readEntry("/StatusBarFontSize", "9");
    cbSizeStatus->setEditText(sizeStatus);
    QString showKeycodes = RS_SETTINGS->readEntry("/ShowKeycodes", 
#ifdef Q_OS_MACX
    "0"
#else
    "1"
#endif
    );
    cbShowKeycodes->setChecked(showKeycodes=="1");

    RS_SETTINGS->endGroup();

    RS_SETTINGS->beginGroup("/Paths");

    lePathTranslations->setText(RS_SETTINGS->readEntry("/Translations", ""));
    lePathHatch->setText(RS_SETTINGS->readEntry("/Patterns", ""));
    lePathFonts->setText(RS_SETTINGS->readEntry("/Fonts", ""));
    lePathScripts->setText(RS_SETTINGS->readEntry("/Scripts", ""));
    lePathLibrary->setText(RS_SETTINGS->readEntry("/Library", ""));

    RS_SETTINGS->endGroup();

    // units:
    for (int i=RS2::None; i<RS2::LastUnit; i++) {
        if (i!=(int)RS2::None)
            cbUnit->addItem(RS_Units::unitToString((RS2::Unit)i));
    }
    // ### Qt4 ###
    cbUnit->model()->sort(0);
    cbUnit->addItem( RS_Units::unitToString(RS2::None), 0 );

    QString def_unit = "Millimeter";
#ifdef QC_PREDEFINED_UNIT

    def_unit = QC_PREDEFINED_UNIT;
#endif

    RS_SETTINGS->beginGroup("/Defaults");
    QString localizedUnit = QObject::tr(RS_SETTINGS->readEntry("/Unit", def_unit).toUtf8());
    cbUnit->setCurrentIndex(cbUnit->findText(localizedUnit));
    RS_SETTINGS->endGroup();

    RS_SETTINGS->beginGroup("/Behavior");
    QString persistentSelections = RS_SETTINGS->readEntry("/PersistentSelections", "0");
    cbPersistentSelections->setChecked(persistentSelections=="1");
    QString highlightOnMouseOver = RS_SETTINGS->readEntry("/HighlightOnMouseOver", "1");
    cbHighlightOnMouseOver->setChecked(highlightOnMouseOver=="1");
    RS_SETTINGS->endGroup();

    restartNeeded = false;
}


/**
 * Destructor
 */
QG_DlgOptionsGeneral::~QG_DlgOptionsGeneral() {}


void QG_DlgOptionsGeneral::setRestartNeeded() {
    restartNeeded = true;
}

void QG_DlgOptionsGeneral::chooseColor(QLabel& l) {
    QColor col(getColorPreview(l));
    //QColor col = QColor(cb.currentText());
    QColor newCol = QColorDialog::getColor(col, this);
    if (newCol.isValid()) {
        //cb.setEditText(newCol.name());
        setColorPreview(l, newCol.name());
    }
}

void QG_DlgOptionsGeneral::chooseBackgroundColor() {
    chooseColor(*lBackgroundColorPreview);
}

void QG_DlgOptionsGeneral::chooseGridColor() {
    chooseColor(*lGridColorPreview);
}

void QG_DlgOptionsGeneral::chooseMetaGridColor() {
    chooseColor(*lMetaGridColorPreview);
}

void QG_DlgOptionsGeneral::chooseSelectedColor() {
    chooseColor(*lSelectedColorPreview);
}

void QG_DlgOptionsGeneral::chooseHighlightedColor() {
    chooseColor(*lHighlightedColorPreview);
}


void QG_DlgOptionsGeneral::choosePreviewColor() {
    chooseColor(*lPreviewColorPreview);
}


void QG_DlgOptionsGeneral::ok() {
    //RS_SYSTEM->loadTranslation(cbLanguage->currentText());
    RS_SETTINGS->beginGroup("/Appearance");
    RS_SETTINGS->writeEntry("/Language",
                            RS_SYSTEM->languageToSymbol(cbLanguage->currentText()));
    /*
    RS_SETTINGS->writeEntry("/LanguageCmd",
                            RS_SYSTEM->languageToSymbol(cbLanguageCmd->currentText()));
    */
    RS_SETTINGS->writeEntry("/ShowRulers",
                            QString("%1").arg((int)cbShowRulers->isChecked()));
    RS_SETTINGS->writeEntry("/ShowCrosshairs",
                            QString("%1").arg((int)cbShowCrosshairs->isChecked()));
    RS_SETTINGS->writeEntry("/ScaleGrid",
                            QString("%1").arg((int)cbScaleGrid->isChecked()));
    RS_SETTINGS->writeEntry("/SolidGridLines",
                            QString("%1").arg((int)cbSolidGridLines->isChecked()));
    RS_SETTINGS->writeEntry("/MinGridSpacing",
                            cbMinGridSpacing->currentText());
    RS_SETTINGS->writeEntry("/MaxPreview",
                            cbMaxPreview->currentText());

    RS_SETTINGS->writeEntry("/BackgroundColor",
                            getColorPreview(*lBackgroundColorPreview));
    RS_SETTINGS->writeEntry("/GridColor",
                            getColorPreview(*lGridColorPreview));
    RS_SETTINGS->writeEntry("/MetaGridColor",
                            getColorPreview(*lMetaGridColorPreview));
    RS_SETTINGS->writeEntry("/SelectedColor",
                            getColorPreview(*lSelectedColorPreview));
    RS_SETTINGS->writeEntry("/HighlightedColor",
                            getColorPreview(*lHighlightedColorPreview));
    RS_SETTINGS->writeEntry("/PreviewColor",
                            getColorPreview(*lPreviewColorPreview));

    RS_SETTINGS->writeEntry("/StatusBarFontSize",
                            cbSizeStatus->currentText());
    RS_SETTINGS->writeEntry("/ShowKeycodes",
                            QString("%1").arg((int)cbShowKeycodes->isChecked()));
    RS_SETTINGS->endGroup();

    RS_SETTINGS->beginGroup("/Paths");
    RS_SETTINGS->writeEntry("/Translations", lePathTranslations->text());
    RS_SETTINGS->writeEntry("/Patterns", lePathHatch->text());
    RS_SETTINGS->writeEntry("/Fonts", lePathFonts->text());
    RS_SETTINGS->writeEntry("/Scripts", lePathScripts->text());
    RS_SETTINGS->writeEntry("/Library", lePathLibrary->text());
    RS_SETTINGS->endGroup();

    RS_SETTINGS->beginGroup("/Defaults");
    RS_SETTINGS->writeEntry("/Unit",
                            RS_Units::unitToString( RS_Units::stringToUnit( cbUnit->currentText() ), false/*untr.*/) );
    RS_SETTINGS->endGroup();
    
    RS_SETTINGS->beginGroup("/Behavior");
    RS_SETTINGS->writeEntry("/PersistentSelections", 
                            QString("%1").arg((int)cbPersistentSelections->isChecked()));
    RS_SETTINGS->writeEntry("/HighlightOnMouseOver", 
                            QString("%1").arg((int)cbHighlightOnMouseOver->isChecked()));
    RS_SETTINGS->endGroup();

    if (restartNeeded==true) {
        QMessageBox::warning( this, tr("Preferences"),
                              tr("Please restart the application to apply all changes."),
                              QMessageBox::Ok,
                              Qt::NoButton);
    }
    accept();
    //return true;
}
    
    
    
void QG_DlgOptionsGeneral::setColorPreview(QLabel& l, const QString& col) {
    QPalette p = l.palette();
    p.setBrush(QPalette::Background, QColor(col));
    l.setPalette(p);
}

QString QG_DlgOptionsGeneral::getColorPreview(const QLabel& l) {
    QPalette p = l.palette();
    return p.brush(QPalette::Background).color().name();
}

