/****************************************************************************
** $Id: qg_filedialog.cpp 9395 2008-03-12 19:29:51Z andrew $
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

#include "qg_filedialog.h"

#include "qg_graphicview.h"

#include <QFileDialog>
#include <qmessagebox.h>

#include "rs_string.h"
#include "rs_settings.h"
#include "rs_system.h"
#include "rs_graphic.h"
#include "rs_fileio.h"


/**
 * Shows a dialog for choosing a file name. Saving the file is up to
 * the caller.
 *
 * @param type Will contain the file type that was chosen by the user.
 *             Can be NULL to be ignored.
 * @param suggestion Suggestion for path and file name.
 *
 * @return File name with path and extension to determine the file type
 *         or an empty string if the dialog was cancelled.
 */
RS_String QG_FileDialog::getSaveFileName(QWidget* parent, 
    RS2::FormatType* type, const QString& caption, const QString& suggestion) {

    // read default settings:
    RS_SETTINGS->beginGroup("/Paths");
    RS_String defDir = RS_SETTINGS->readEntry("/Save",
                       RS_SYSTEM->getHomeDir());
    RS_String defFilter = RS_SETTINGS->readEntry("/SaveFilter",
                          RS_FILEIO->getFilterString(RS2::FormatDxf));
    RS_SETTINGS->endGroup();
    
    // prepare file save as dialog:
    QFileDialog* fileDlg = new QFileDialog(parent, Qt::Dialog);
    bool cancel = false;
    RS_String fn = "";

    QStringList filters = RS_FILEIO->getAllExportFilterStrings();

    fileDlg->setAcceptMode(QFileDialog::AcceptSave);
    fileDlg->setNameFilters(filters);
    fileDlg->setFileMode(QFileDialog::AnyFile);
    if (!suggestion.isEmpty()) {
        fileDlg->selectFile(QFileInfo(suggestion).fileName());
    }
    if (caption.isEmpty()) {
        fileDlg->setWindowTitle(QObject::tr("Save Drawing As"));
    }
    else {
        fileDlg->setWindowTitle(caption);
    }
    if (!suggestion.isEmpty() && 
        !QFileInfo(suggestion).absolutePath().isEmpty()) {
        fileDlg->setDirectory(QFileInfo(suggestion).absolutePath());
    }
    else {
        fileDlg->setDirectory(defDir);
    }
    fileDlg->selectNameFilter(defFilter);
    fileDlg->setConfirmOverwrite(true);
#ifdef RS_ARCH
    fileDlg->setDefaultSuffix("rdxf");
#else
    fileDlg->setDefaultSuffix("dxf");
#endif

    /*
    // doesn't cut it since Qt ignores the filter argument on OS X.
    fn = QFileDialog::getSaveFileName(
                    parent,
                    QObject::tr("Save Drawing As"),
                    defDir,
                    filters.join(";;"),
                    &defFilter);
    */

    // accepted 
    // Qt 4.3.3 ends up here even after a negative answer to an overwrite warning. Qt bug.
    if (fileDlg->exec()==QDialog::Accepted) {
        RS_DEBUG->print("QG_FileDialog::getSaveFileName: accepted");
        RS_StringList fns = fileDlg->selectedFiles();
        if (fns.count()==1) {
            fn = fns.at(0);
            fn = QDir::toNativeSeparators(QFileInfo(fn).absoluteFilePath());
            cancel = false;
    
            // find out extension:
            QString filter = fileDlg->selectedNameFilter();
            QString format = "";
            int i = filter.indexOf("(*.");
            if (i!=-1) {
                int i2 = filter.indexOf(QRegExp("[) ]"), i);
                format = filter.mid(i+3, i2-(i+3));
                format = format.toLower();
            }

            // automatically append default extension to file:
            /*
            if (!fn.endsWith("."+format, Qt::CaseInsensitive)) {
                fn.append("." + format);
            }
            */

            // set format:
            if (type!=NULL) {
                *type = RS_FILEIO->getType(fileDlg->selectedNameFilter());
            }
        }
    } else {
        RS_DEBUG->print("QG_FileDialog::getSaveFileName: cancel");
        cancel = true;
        fn = "";
    }

    // store new default settings:
    if (!cancel) {
        RS_SETTINGS->beginGroup("/Paths");
        RS_SETTINGS->writeEntry("/Save", QFileInfo(fn).absolutePath());
        //RS_SETTINGS->writeEntry("/SaveFilter", fileDlg->selectedFilter());
        RS_SETTINGS->endGroup();
    }

    delete fileDlg;

#ifdef Q_OS_WIN
    if (fn.contains(":\\\\")) {
        fn.replace(":\\\\", ":\\");
    }
#endif

    return fn;
}



/**
 * Shows a dialog for choosing one or mode files. Opening the files is up to
 * the caller.
 *
 * @return File name(s) with path and extension to determine the file type
 *         or an empty string list if the dialog was cancelled.
 */
QStringList QG_FileDialog::getOpenFileNames(QWidget* parent, 
    RS2::FormatType* type,
    const QString& caption) {

    RS_DEBUG->print("QG_FileDialog::getOpenFileNames");

    // read default settings:
    RS_SETTINGS->beginGroup("/Paths");
    RS_String defDir = RS_SETTINGS->readEntry("/Open",
                       RS_SYSTEM->getHomeDir());
    //RS_String defFilter = RS_SETTINGS->readEntry("/OpenFilter",
    //                      "Drawing Exchange (*.dxf *.DXF)");
    RS_String defFilter = RS_FILEIO->getFilterString(RS2::FormatDxf);
    RS_SETTINGS->endGroup();

    //RS_String fn = "";
    RS_StringList fns;
    bool cancel = false;
    
    /*
    // system dialog:
    QStringList filters = RS_FILEIO->getAllImportFilterStrings();
    fn = QFileDialog::getOpenFileName(parent, 
        QObject::tr("Open Drawing"),
        defDir,
        filters.join(";;"),
        &defFilter);
    cancel = fn.isNull();
    */

    QFileDialog* fileDlg = new QFileDialog(parent, Qt::Dialog);

    QStringList filters = RS_FILEIO->getAllImportFilterStrings();

    fileDlg->setAcceptMode(QFileDialog::AcceptOpen);
    fileDlg->setNameFilters(filters);
    fileDlg->setFileMode(QFileDialog::ExistingFiles);
    if (caption.isEmpty()) {
        fileDlg->setWindowTitle(QObject::tr("Open Drawing"));
    }
    else {
        fileDlg->setWindowTitle(caption);
    }
    fileDlg->setDirectory(defDir);
    fileDlg->selectNameFilter(defFilter);
    
    if (fileDlg->exec()==QDialog::Accepted) {
        fns = fileDlg->selectedFiles();
        if (fns.count()>=1) {
            /*
            fn = fns.at(0);
            fn = QDir::convertSeparators(QFileInfo(fn).absoluteFilePath());
            */
            if (type!=NULL) {
                *type = RS_FILEIO->getType(fileDlg->selectedNameFilter());
            }
            cancel = false;
        }
    } else {
        cancel = true;
    }
    
    // store new default settings:
    if (!cancel) {
        RS_SETTINGS->beginGroup("/Paths");
        RS_SETTINGS->writeEntry("/Open", fileDlg->directory().path());
        RS_SETTINGS->writeEntry("/OpenFilter", fileDlg->selectedNameFilter());
        RS_SETTINGS->endGroup();
    }

    //RS_DEBUG->print("QG_FileDialog::getOpenFileName: fileName: %s", (const char*)fn.toLatin1());
    RS_DEBUG->print("QG_FileDialog::getOpenFileNames: OK");

    delete fileDlg;

    return fns;
}
