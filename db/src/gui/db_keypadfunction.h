// This file is part of the SpeedCrunch project
// Copyright (C) 2005-2006 Johan Thelin <e8johan@gmail.com>
// Copyright (C) 2007-2009 Helder Correia <helder.pereira.correia@gmail.com>
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; see the file COPYING.  If not, write to
// the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
// Boston, MA 02110-1301, USA.

#ifndef DB_KEYPADFUNCTION_H
#define DB_KEYPADFUNCTION_H

#include <QWidget>
#include <memory>
#include "db_global.h"

class DB_EXPORT DB_KeypadFunction : public QWidget
{
    Q_OBJECT

public:
    enum Button { /* Key0, Key1, Key2, Key3, Key4, Key5, Key6, Key7, Key8, Key9,
                  KeyEquals, KeyPlus, KeyMinus, KeyTimes, KeyDivide,
                  KeyRadixChar, */ KeyClear, KeyEE, KeyLeftPar, KeyRightPar,
                  KeyRaise, KeySqrt, KeyPercent, KeyFactorial, KeyPi, KeyAns,
                  KeyX, KeyXEquals, KeyExp, KeyLn, KeySin, KeyAsin, KeyCos,
                  KeyAcos, KeyTan, KeyAtan };

    explicit DB_KeypadFunction( QWidget * parent  = 0 );
    ~DB_KeypadFunction();

signals:
    void buttonPressed( DB_KeypadFunction::Button ) const;

public slots:
//    void handleRadixCharacterChange();
    void retranslateText();

protected slots:
    void emitButtonPressed( int button ) const;

protected:
    virtual void changeEvent( QEvent * );

private:
    struct Private;
    const std::auto_ptr<Private> d;

    DB_KeypadFunction( const DB_KeypadFunction & );
    DB_KeypadFunction & operator=( const DB_KeypadFunction & );
};

#endif

