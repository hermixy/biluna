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

#include "db_keypadnumeric.h"

// #include "core/settings.hxx"

#include <cstring>
#include <QtCore/QLocale>
#include <QtCore/QHash>
#include <QtCore/QSignalMapper>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#if QT_VERSION >= 0x040400 && defined(Q_WS_MAC) && !defined(QT_NO_STYLE_MAC)
#include <QtWidgets/QMacStyle>
#endif
#include <QtWidgets/QStyle>
#include <QtWidgets/QStyleOptionButton>
#include <QtWidgets/QToolButton>
#include "rb_debug.h"


struct DB_KeypadNumeric::Private
{
    static const struct KeyDescription {
        QString label;
        Button button;
        bool boldFont;
        int gridRow;
        int gridColumn;
    } keyDescriptions[];

    DB_KeypadNumeric * p;
    QHash<Button, QPair<QToolButton*, const KeyDescription*> > keys;
    QSignalMapper mapper;

    QToolButton* key(Button button) const;
    void createButtons();
    void disableButtonFocus();
    void layoutButtons();
    void setButtonTooltips();
    void sizeButtons();
};

const DB_KeypadNumeric::Private::KeyDescription DB_KeypadNumeric::Private::keyDescriptions[] = {
    {QString::fromLatin1("0"),      Key0,           true,   3, 0},
    {QString::fromLatin1("1"),      Key1,           true,   2, 0},
    {QString::fromLatin1("2"),      Key2,           true,   2, 1},
    {QString::fromLatin1("3"),      Key3,           true,   2, 2},
    {QString::fromLatin1("4"),      Key4,           true,   1, 0},
    {QString::fromLatin1("5"),      Key5,           true,   1, 1},
    {QString::fromLatin1("6"),      Key6,           true,   1, 2},
    {QString::fromLatin1("7"),      Key7,           true,   0, 0},
    {QString::fromLatin1("8"),      Key8,           true,   0, 1},
    {QString::fromLatin1("9"),      Key9,           true,   0, 2},
    {QString::fromLatin1("."),      KeyRadixChar,   true,   3, 1},
    {QString::fromUtf8("="),        KeyEquals,      true,   3, 2},
    {QString::fromUtf8("÷"),        KeyDivide,      true,   0, 3},
    {QString::fromUtf8("*"),        KeyTimes,       true,   1, 3},
    {QString::fromUtf8("−"),        KeyMinus,       true,   2, 3},
    {QString::fromUtf8("+"),        KeyPlus,        true,   3, 3}
/*    {QString::fromLatin1("acos"),   KeyAcos,        false,  2, 8},
    {QString::fromLatin1("ans"),    KeyAns,         false,  1, 6},
    {QString::fromLatin1("asin"),   KeyAsin,        false,  1, 8},
    {QString::fromLatin1("atan"),   KeyAtan,        false,  3, 8},
    {QString::fromLatin1("C"),      KeyClear,       false,  0, 4},
    {QString::fromLatin1("cos"),    KeyCos,         false,  2, 7},
    {QString::fromLatin1("E"),      KeyEE,          false,  1, 4},
    {QString::fromLatin1("exp"),    KeyExp,         false,  0, 7},
    {QString::fromLatin1("!"),      KeyFactorial,   false,  3, 5},
    {QString::fromLatin1("ln"),     KeyLn,          false,  0, 8},
    {QString::fromLatin1("("),      KeyLeftPar,     false,  2, 4},
    {QString::fromLatin1("%"),      KeyPercent,     false,  3, 4},
    {QString::fromLatin1("^"),      KeyRaise,       false,  1, 5},
    {QString::fromLatin1(")"),      KeyRightPar,    false,  2, 5},
    {QString::fromLatin1("sin"),    KeySin,         false,  1, 7},
    {QString::fromLatin1("tan"),    KeyTan,         false,  3, 7},
    {QString::fromLatin1("x="),     KeyXEquals,     false,  3, 6},
    {QString::fromLatin1("x"),      KeyX,           false,  2, 6},
    {QString::fromUtf8("π"),        KeyPi,          false,  0, 6},
    {QString::fromUtf8("√"),        KeySqrt,        false,  0, 5} */
};

DB_KeypadNumeric::DB_KeypadNumeric( QWidget * parent )
    : QWidget( parent ), d( new DB_KeypadNumeric::Private )
{
    d->p = this;

    d->createButtons();
    d->sizeButtons();
    d->layoutButtons();
    d->setButtonTooltips();
    d->disableButtonFocus();
    setLayoutDirection( Qt::LeftToRight );
    connect(&d->mapper, SIGNAL(mapped(int)), SLOT(emitButtonPressed(int)));
}

DB_KeypadNumeric::~DB_KeypadNumeric()
{
}

//void DB_KeypadNumeric::handleRadixCharacterChange()
//{
//    d->key(KeyRadixChar)->setText( QString(QChar(Settings::instance()->radixCharacter())) );
//}

void DB_KeypadNumeric::retranslateText()
{
    d->setButtonTooltips();
//    handleRadixCharacterChange();
}

void DB_KeypadNumeric::emitButtonPressed( int button ) const
{
    emit buttonPressed(Button(button));
}

QToolButton* DB_KeypadNumeric::Private::key(Button button) const
{
    Q_ASSERT(keys.contains(button));
    return keys.value(button).first;
}

void DB_KeypadNumeric::Private::createButtons()
{
    keys.clear();

    QFont boldFont;
    boldFont.setBold( true );
    boldFont.setPointSize( boldFont.pointSize() + 3 );

    static const int keyDescriptionsCount =
        int(sizeof keyDescriptions / sizeof keyDescriptions[0]);
    for (int i = 0; i < keyDescriptionsCount; ++i) {
        const KeyDescription *description = keyDescriptions + i;
        QToolButton* key = new QToolButton();
        key->setToolButtonStyle(Qt::ToolButtonTextOnly);
        key->setText(description->label);
        if (description->boldFont)
            key->setFont(boldFont);
        const QPair<QToolButton*, const KeyDescription*> hashValue(key, description);
        keys.insert(description->button, hashValue);

        QObject::connect(key, SIGNAL(clicked()), &mapper, SLOT(map()));
        mapper.setMapping(key, description->button);
    }
}

void DB_KeypadNumeric::Private::disableButtonFocus()
{
    QHashIterator<Button, QPair<QToolButton*, const KeyDescription*> > i(keys);
    while (i.hasNext()) {
        i.next();
        i.value().first->setFocusPolicy( Qt::NoFocus );
    }
}

void DB_KeypadNumeric::Private::layoutButtons()
{
    int layoutSpacing = 3;

#if QT_VERSION >= 0x040400 && defined(Q_WS_MAC) && !defined(QT_NO_STYLE_MAC)
    // Workaround for a layouting bug in QMacStyle, Qt 4.4.0. Buttons would overlap
    if ( qobject_cast<QMacStyle *>(p->style()) )
        layoutSpacing = -1;
#endif

    QGridLayout * layout = new QGridLayout( p );
    layout->setMargin( 3 );
    layout->setSpacing( layoutSpacing );

    QHashIterator<Button, QPair<QToolButton*, const KeyDescription*> > i(keys);
    while (i.hasNext()) {
        i.next();
        QWidget *widget = i.value().first;
        const int row = i.value().second->gridRow;
        const int column = i.value().second->gridColumn;
        layout->addWidget(widget, row, column);
    }
}

void DB_KeypadNumeric::Private::setButtonTooltips()
{
//    key(KeyAcos)   ->setToolTip( DB_KeypadNumeric::tr("Inverse cosine") );
//    key(KeyAns)    ->setToolTip( DB_KeypadNumeric::tr("The last result") );
//    key(KeyAsin)   ->setToolTip( DB_KeypadNumeric::tr("Inverse sine") );
//    key(KeyAtan)   ->setToolTip( DB_KeypadNumeric::tr("Inverse tangent") );
//    key(KeyClear)  ->setToolTip( DB_KeypadNumeric::tr("Clear expression") );
//    key(KeyCos)    ->setToolTip( DB_KeypadNumeric::tr("Cosine") );
//    key(KeyEE)     ->setToolTip( DB_KeypadNumeric::tr("Scientific notation") );
//    key(KeyExp)    ->setToolTip( DB_KeypadNumeric::tr("Exponential") );
//    key(KeyLn)     ->setToolTip( DB_KeypadNumeric::tr("Natural logarithm") );
//    key(KeySin)    ->setToolTip( DB_KeypadNumeric::tr("Sine") );
//    key(KeySqrt)   ->setToolTip( DB_KeypadNumeric::tr("Square root") );
//    key(KeyTan)    ->setToolTip( DB_KeypadNumeric::tr("Tangent") );
//    key(KeyXEquals)->setToolTip( DB_KeypadNumeric::tr("Assign variable x") );
//    key(KeyX)      ->setToolTip( DB_KeypadNumeric::tr("The variable x") );
}

void DB_KeypadNumeric::Private::sizeButtons()
{
    // The same font in all buttons, so just pick one
/*    QFontMetrics fm = key(Key0)->fontMetrics();

    int maxWidth = fm.width( key(KeyAcos)->text() );
    int textHeight = qMax( fm.lineSpacing(), 14 );
    //int ls = fm.lineSpacing();

    QStyle::ContentsType type = QStyle::CT_ToolButton;
    QStyleOptionButton opt;
    const QWidget *exampleWidget = key(Key9);
    opt.initFrom(exampleWidget);
    QSize size = exampleWidget->style()->sizeFromContents( type, &opt,
        QSize(int(maxWidth),int(textHeight)).expandedTo(QApplication::globalStrut()), exampleWidget );

#ifdef Q_WS_X11
    // we would like to use the button size as indicated by the widget style,
    // but in some cases, e.g. KDE's Plastik or Oxygen, another few pixels
    // (typically 5) are added as the content margin, thereby making the button
    // incredibly wider than necessary
    // workaround: take only the hinted height, adjust the width ourselves (with our own margin)
    maxWidth += 6;
    int hh = size.height();
    int ww = hh * 162 / 100; // use golden ratio
    size = QSize( qMax(ww, maxWidth), hh );
#endif
*/
    QSize size = QSize(43, 24);

    // limit the size of the buttons
    QHashIterator<Button, QPair<QToolButton*, const KeyDescription*> > i(keys);
    while (i.hasNext()) {
        i.next();
        i.value().first->setFixedSize( size );
    }
}

void DB_KeypadNumeric::changeEvent( QEvent * e )
{
    if ( e->type() == QEvent::LanguageChange )
        retranslateText();
    else
        QWidget::changeEvent( e );
}
