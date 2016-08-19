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

#include "qg_textoptions.h"

#include "rs_actiondrawtext.h"
#include "rs_settings.h"

/**
 * Constructor
 */
QG_TextOptions::QG_TextOptions(QWidget* parent)
        : QG_OptionsBase(parent) {
    setupUi(this);

    connect(leText, SIGNAL(textChanged(const QString&)),
        this, SLOT(updateText(const QString&)));
    connect(leAngle, SIGNAL(textChanged(const QString&)),
        this, SLOT(updateAngle(const QString&)));
}


/**
 * Destructor
 */
QG_TextOptions::~QG_TextOptions() {
}


void QG_TextOptions::setAction(RS_ActionInterface* a, bool update) {
    action = dynamic_cast<RS_ActionDrawText*>(a);
    if (action!=NULL) {
        QString st;
        QString sa;
        if (update) {
            st = action->getText();
            sa = QString("%1").arg(RS_Math::rad2deg(action->getAngle()));
        } else {
            st = "";
            sa = "0.0";
        }

/*#if defined(OOPL_VERSION) && defined(Q_WS_WIN)
        QCString iso = RS_System::localeToISO( QTextCodec::locale() );
        QTextCodec *codec = QTextCodec::codecForName(iso);
        if (codec!=NULL) {
            st = codec->toUnicode(RS_FilterDxf::toNativeString(action->getText().local8Bit()));
        } else {
            st = RS_FilterDxf::toNativeString(action->getText().local8Bit());
        }
//#else*/
        if (st.indexOf(QChar::LineSeparator)<0) {
            leText->setText(st);
        }
        else {
            leText->setText("");
            leText->setDisabled(true);
        }
//#endif
        leAngle->setText(sa);
    } else {
        RS_DEBUG->print(RS_Debug::D_ERROR, 
            "QG_TextOptions::setAction: wrong action type");
        action = NULL;
    }
}


void QG_TextOptions::updateText(const QString&) {
    RS_DEBUG->print("QG_TextOptions::updateText");
    if (action!=NULL) {
/*#if defined(OOPL_VERSION) && defined(Q_WS_WIN)
        QCString iso = RS_System::localeToISO( QTextCodec::locale() );
        action->setText(
            RS_FilterDxf::toNativeString( 
             QString::fromLocal8Bit( QTextCodec::codecForName( iso )->fromUnicode( leText->text() ) )
            )
        );
//#else*/
       action->setText(leText->text());
//#endif
    }
}

void QG_TextOptions::updateAngle(const QString&) {
    if (action!=NULL) {
        action->setAngle(RS_Math::deg2rad(RS_Math::eval(leAngle->text())));
    }
}
