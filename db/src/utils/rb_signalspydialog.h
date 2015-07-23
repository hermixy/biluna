/*****************************************************************
 * $Id: rb_signalspydialog.h 2248 2015-06-21 09:13:00Z rutger $
 *
 * Copyright (C) 2005 Red-Bag. All rights reserved.
 * This file is part of the Biluna LDT project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

/**
 * To be used with Q4puGenericSignalSpy,h and .cpp. This dialog
 * can be opened with show() to create a textEdit which can receive
 * the information from the Q4puGenericSignalSpy
 *
 *
 * Use as follows 2012-12-21:

    ==> in .cpp file of widget or dialog

    #include "rb_signalspydialog.h"

    ==> in .cpp init()

    RB_SignalSpyDialog* ssd = new RB_SignalSpyDialog(this, mModel);
    ssd->show();




 * Below is now included in RB_SignalSpyDialog
 * old: add the following code to the window or dialog with the widget to be spy-ed,
 	
	==> in the dialog or window .h header file
	
    #include "../utils/Q4puGenericSignalSpy.h"
    #include "../utils/rb_signalspydialog.h"
	
	public:
		Q4puGenericSignalSpy* spy;
		RB_SignalSpyDialog* ssd;
	
	
 	==> in the dialog or window source .cpp file constructor
  
  	// Setup the spy, cw is the widget to be checked for the signal and slots 
  	//   with copies (signals slots) send to SignalSpyDialog (ssd)
	if (!spy) {
		spy = new Q4puGenericSignalSpy(this);
		spy->spyOn(model);
	} else {
		delete spy;
		spy = new Q4puGenericSignalSpy(this);
		spy->spyOn(model);
	}
	
	// dialog to receive the signal and slots from genericsignalspy
	ssd = new RB_SignalSpyDialog(this);

	// connect the signal and slots
	QObject::connect(spy, SIGNAL(caughtSignal(const QString&)), 
						ssd, SLOT(append(const QString&)));
	QObject::connect(spy, SIGNAL(caughtSlot(const QString&)), 
						ssd, SLOT(append(const QString&)));
	
	ssd->show();
	
 * 
 */
 
#ifndef RB_SIGNALSPYDIALOG_H
#define RB_SIGNALSPYDIALOG_H

// #include <QtTest>
#include <QDialog>
#include <QTextEdit>
#include "db_global.h"


/*******************************************************
**
** Declaration of Q4puGenericSignalSpy
**
** Copyright (C) 2005 Prashanth N Udupa
**
** Q4puGenericSignalSpy can be used to spy on QObjects
** for signals that they emit, and the slots that are
** invoked in them.
**
********************************************************/

/***************************************************************************
                           Q4puGenericSignalSpy.h
                             -------------------
    begin                : Mon 8 August 2005
    copyright            : (C) 2005 by Prashanth Udupa
    email                : prashanth.udupa@gmail.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

// #ifndef Q4PU_GENERIC_SIGNAL_SPY
// #define Q4PU_GENERIC_SIGNAL_SPY

#include <QObject>

class Q4puGenericSignalSpyPrivate;

class Q4puGenericSignalSpy : public QObject
{
    Q_OBJECT

public:
    Q4puGenericSignalSpy(QObject* parent);
    ~Q4puGenericSignalSpy();

    void spyOn(QObject* object) { _Object = object; }
    QObject* spyingOn() const   { return _Object; }

    void emitCaughtSignal(const QString &sig);
    void emitCaughtSlot(const QString &slot);

signals:
    void caughtSignal(const QString &name);
    void caughtSlot(const QString &name);

private:
    QObject* _Object;
};

inline void Q4puGenericSignalSpy::emitCaughtSignal(const QString &sig)
{
    emit caughtSignal(sig);
}

inline void Q4puGenericSignalSpy::emitCaughtSlot(const QString &slot)
{
    emit caughtSlot(slot);
}

// #endif



/**
 * Class to spy or watch the signals emitted and slots used by a widget,
 * to be used for debugging only.
 */
class DB_EXPORT RB_SignalSpyDialog : public QDialog {

	Q_OBJECT

public:
	RB_SignalSpyDialog(QWidget* parent, QObject* target);
	virtual ~RB_SignalSpyDialog();
	
public slots:
	virtual void append(const QString& str);
	virtual void clearText();

private:
	Q4puGenericSignalSpy* spy;
	QTextEdit* textEdit;

};

#endif //_RB_SIGNALSPYDIALOG_H_
