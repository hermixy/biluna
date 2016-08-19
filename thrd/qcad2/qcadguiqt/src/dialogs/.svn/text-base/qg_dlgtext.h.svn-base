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

#ifndef QG_DLGTEXT_H
#define QG_DLGTEXT_H

#include <QDialog>

#include "rs_text.h"

#include "ui_qg_dlgtext.h"

/**
 *
 */
class QG_DlgText: public QDialog, private Ui::QG_DlgText {
    Q_OBJECT

public:
    QG_DlgText(QWidget* parent=0);
    virtual ~QG_DlgText();
    
private:
    int getAlignment();

public slots:
    void updateUniCharComboBox(int);
    void setText(RS_Text & t, bool isNew);
    void updateText();
    void setAlignmentTL();
    void setAlignmentTC();
    void setAlignmentTR();
    void setAlignmentML();
    void setAlignmentMC();
    void setAlignmentMR();
    void setAlignmentBL();
    void setAlignmentBC();
    void setAlignmentBR();
    void setAlignment(int a);
    void setFont(const QString& f);
    void defaultChanged(bool);
    void loadText();
    void load(const QString& fn);
    void saveText();
    void save(const QString& fn);
    void insertSymbol(const QString& s);
    void updateUniCharButton(int);
    void insertChar();

private:
    bool isNew;
    RS_Text* text;
    RS_Font* font;
};

#endif

