/*****************************************************************
 * $Id: db_calculatorwidget.cpp 2182 2014-09-30 16:03:34Z rutger $
 * Created: Nov 24, 2012 1:08:25 PM - Rutger Botermans
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_calculatorwidget.h"

#include "db_keypadfunction.h"
#include "db_keypadnumeric.h"
#include "rb_flowlayout.h"

#include "constants.hxx"
#include "editor.hxx"
#include "evaluator.hxx"
#include "floatconfig.h"
#include "functions.hxx"
#include "resultdisplay.hxx"

/**
 * Constructor
 */
DB_CalculatorWidget::DB_CalculatorWidget(QWidget *parent) : RB_Widget(parent) {
    mConstants = new Constants(); // Constants::instance();
    mFunctions = new Functions(); // Functions::instance();
    mEvaluator = new Evaluator(mFunctions); // Evaluator::instance();

    setupUi();
}

/**
 * Destructor
 */
DB_CalculatorWidget::~DB_CalculatorWidget() {
    // nothing
    delete mConstants;
    delete mEvaluator;
    delete mFunctions;
}

void DB_CalculatorWidget::init() {
    createFixedConnections();
}

/**
 * Set focus to Editor and select all
 */
void DB_CalculatorWidget::initFocus() {
    mEditor->setFocus();
    mEditor->selectAll();
}

void DB_CalculatorWidget::setupUi() {
    QVBoxLayout* vbLayout = new QVBoxLayout(this);
    vbLayout->setMargin(0);
    vbLayout->setSpacing(1);

    mResultDisplay = new ResultDisplay();
    vbLayout->addWidget(mResultDisplay);

    QAction* copyAction = new QAction(tr("Copy"), mResultDisplay);
    connect(copyAction, SIGNAL(triggered()),
            mResultDisplay, SLOT(copy()));
    mResultDisplay->addAction(copyAction);
    mResultDisplay->setContextMenuPolicy(Qt::ActionsContextMenu);

    mEditor = new Editor(mEvaluator, mFunctions, mConstants);
    vbLayout->addWidget(mEditor);

    QHBoxLayout* hbLayout = new QHBoxLayout();
    hbLayout->setMargin(0);
    hbLayout->setSpacing(3);

    QComboBox* cbAngleUnit = new QComboBox(this);
    cbAngleUnit->addItem("Radian");
    // Degree
    cbAngleUnit->setEnabled(false);
    hbLayout->addWidget(cbAngleUnit);

    QComboBox* cbResultFormat = new QComboBox(this);
    cbResultFormat->addItem("General decimal");
    // Binary, Octal, Hexadecimal, Fixed decimal, Engineering decimal, Scientific decimal
    cbResultFormat->setEnabled(false);
    hbLayout->addWidget(cbResultFormat);

    vbLayout->addLayout(hbLayout);

    RB_FlowLayout* flowLayout = new RB_FlowLayout(NULL, 0, 0, 0);

    QWidget* kf = new DB_KeypadFunction(this);
    kf->setFocusPolicy(Qt::NoFocus);
    flowLayout->addWidget(kf);
    connect(kf, SIGNAL(buttonPressed(DB_KeypadFunction::Button)),
            this, SLOT(slotHandleKeypadFunctionPress(DB_KeypadFunction::Button)));

    QWidget* kn = new DB_KeypadNumeric(this);
    kn->setFocusPolicy(Qt::NoFocus);
    flowLayout->addWidget(kn);
    connect(kn, SIGNAL(buttonPressed(DB_KeypadNumeric::Button)),
            this, SLOT(slotHandleKeypadNumericPress(DB_KeypadNumeric::Button)));

    vbLayout->addLayout(flowLayout);

}

/**
 * Slot handling the function key pressed/clicked event
 */
void DB_CalculatorWidget::slotHandleKeypadFunctionPress(DB_KeypadFunction::Button pb) {
    QTextCursor cursor = mEditor->textCursor();

    switch (pb) {
    case DB_KeypadFunction::KeyEE       : mEditor->insert( "e" ); break;
    case DB_KeypadFunction::KeyLeftPar  : {
        slotHandleLeftParenPressed();
        break;
    }
    case DB_KeypadFunction::KeyRightPar : mEditor->insert( ")" ); break;
    case DB_KeypadFunction::KeyRaise    : mEditor->insert( "^" ); break;
    case DB_KeypadFunction::KeyPercent  : mEditor->insert( "%" ); break;
    case DB_KeypadFunction::KeyFactorial: mEditor->insert( "!" ); break;

    case DB_KeypadFunction::KeyX      : mEditor->insert( "x"   ); break;
    case DB_KeypadFunction::KeyXEquals: mEditor->insert( "x="  ); break;
    case DB_KeypadFunction::KeyPi     : mEditor->insert( "pi"  ); break;
    case DB_KeypadFunction::KeyAns    : mEditor->insert( "ans" ); break;

    case DB_KeypadFunction::KeySqrt :
        mEditor->insert( "sqrt()" );
        cursor.setPosition(cursor.position() - 1);
        mEditor->setTextCursor(cursor);
        break;
    case DB_KeypadFunction::KeyLn:
        mEditor->insert( "ln()"   );
        cursor.setPosition(cursor.position() - 1);
        mEditor->setTextCursor(cursor);
        break;
    case DB_KeypadFunction::KeyExp :
        mEditor->insert( "exp()"  );
        cursor.setPosition(cursor.position() - 1);
        mEditor->setTextCursor(cursor);
        break;
    case DB_KeypadFunction::KeySin :
        mEditor->insert( "sin()"  );
        cursor.setPosition(cursor.position() - 1);
        mEditor->setTextCursor(cursor);
        break;
    case DB_KeypadFunction::KeyCos :
        mEditor->insert( "cos()"  );
        cursor.setPosition(cursor.position() - 1);
        mEditor->setTextCursor(cursor);
        break;
    case DB_KeypadFunction::KeyTan :
        mEditor->insert( "tan()"  );
        cursor.setPosition(cursor.position() - 1);
        mEditor->setTextCursor(cursor);
        break;
    case DB_KeypadFunction::KeyAcos :
        mEditor->insert( "acos()" );
        cursor.setPosition(cursor.position() - 1);
        mEditor->setTextCursor(cursor);
        break;
    case DB_KeypadFunction::KeyAtan :
        mEditor->insert( "atan()" );
        cursor.setPosition(cursor.position() - 1);
        mEditor->setTextCursor(cursor);
        break;
    case DB_KeypadFunction::KeyAsin :
        mEditor->insert( "asin()" );
        cursor.setPosition(cursor.position() - 1);
        mEditor->setTextCursor(cursor);
        break;

    case DB_KeypadFunction::KeyClear: clearEditor(); break;

    default: break;
  }

    QTimer::singleShot(0, mEditor, SLOT(setFocus()));
}

/**
 * Slot handling the numeric key pressed/clicked event
 */
void DB_CalculatorWidget::slotHandleKeypadNumericPress(DB_KeypadNumeric::Button pb) {
    switch (pb) {
        case DB_KeypadNumeric::Key0: mEditor->insert( "0" ); break;
        case DB_KeypadNumeric::Key1: mEditor->insert( "1" ); break;
        case DB_KeypadNumeric::Key2: mEditor->insert( "2" ); break;
        case DB_KeypadNumeric::Key3: mEditor->insert( "3" ); break;
        case DB_KeypadNumeric::Key4: mEditor->insert( "4" ); break;
        case DB_KeypadNumeric::Key5: mEditor->insert( "5" ); break;
        case DB_KeypadNumeric::Key6: mEditor->insert( "6" ); break;
        case DB_KeypadNumeric::Key7: mEditor->insert( "7" ); break;
        case DB_KeypadNumeric::Key8: mEditor->insert( "8" ); break;
        case DB_KeypadNumeric::Key9: mEditor->insert( "9" ); break;

        case DB_KeypadNumeric::KeyPlus  : mEditor->insert( "+" ); break;
        case DB_KeypadNumeric::KeyMinus : mEditor->insert( "-" ); break;
        case DB_KeypadNumeric::KeyTimes : mEditor->insert( "*" ); break;
        case DB_KeypadNumeric::KeyDivide: mEditor->insert( "/" ); break;

        case DB_KeypadNumeric::KeyRadixChar:
            mEditor->insert("." /*QString(d->settings->radixCharacter())*/ );
            break;

        default: break;
    }

    QTimer::singleShot(0, mEditor, SLOT(setFocus()));

    if (pb == DB_KeypadNumeric::KeyEquals) {
        mEditor->evaluate();
    }
}

// TODO: change to more generic function/class/utility and make general available
void DB_CalculatorWidget::slotHandleLeftParenPressed() {
    // Handles keyPressEvent in Editor class if key == Qt::Key_ParenLeft
    // and signal emitted for this widget to catch

    // cursor positions
    QTextCursor cursor = mEditor->textCursor();

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
    cursor.movePosition(QTextCursor::End, QTextCursor::KeepAnchor, 1);
    mEditor->setTextCursor(cursor);

    QString str = cursor.selectedText();
    int left = 0;
    int right = 0;

    for (int i = 0; i < str.size(); ++i) {
        if (str.at(i) == QChar('(')) {
            left += 1;
        } else if (str.at(i) == QChar(')')) {
            right += 1;
        }
    }

    if (cursor.hasSelection()) {
        // selection of text prevails above left/right mismatch
        cursor.setPosition(closeCharPos, QTextCursor::MoveAnchor);
        mEditor->setTextCursor(cursor);
        mEditor->insert(")");
        cursor.setPosition(openCharPos, QTextCursor::MoveAnchor);
        mEditor->setTextCursor(cursor);
        mEditor->insert("(");
    } else if (left < right) {
        // complete the left parenthese only
        cursor.setPosition(openCharPos, QTextCursor::MoveAnchor);
        mEditor->setTextCursor(cursor);
        mEditor->insert("(");
    } else {
        cursor.setPosition(openCharPos, QTextCursor::MoveAnchor);
        mEditor->insert("()");
        cursor.setPosition(cursor.position() - 1);
        mEditor->setTextCursor(cursor);
    }
}

/**
 * Clear editor
 */
void DB_CalculatorWidget::clearEditor() {
    mEditor->clear();
    QTimer::singleShot(0, mEditor, SLOT(setFocus()));
}

/**
 * Create signal/slot connections
 */
void DB_CalculatorWidget::createFixedConnections() {
    // ..

    connect(mEditor, SIGNAL(returnPressed()),
            this, SLOT(evaluateEditorExpression()));
//    connect(mEditor, SIGNAL(textChanged()),
//            this , SLOT(handleEditorTextChange()));

    // ..
}

/**
 * Evaluate expression in editor and show result
 */
void DB_CalculatorWidget::evaluateEditorExpression() {
    QString str = mEvaluator->autoFix(mEditor->text());
    if (str.isEmpty()) {
        return;
    }

    mEvaluator->setExpression(str);
    HNumber result = mEvaluator->evalUpdateAns();

    if (!mEvaluator->error().isEmpty()) {
//        showAutoCalcTip(mEvaluator->error());
    } else if (!result.isNan()) {
        mResultDisplay->append(str, result);
        const char format = result.format() != 0 ? result.format() : 'e';
        char* num = HMath::format(result, format, DECPRECISION);
        mEditor->appendHistory(str, num);
        free(num);
        mEditor->setAnsAvailable(true);

//        if (d->settings->variablesDockVisible) {
//            d->docks.variables->updateList();
//        }
//        if ( d->settings->historyDockVisible ) {
//            HistoryWidget * history = qobject_cast<HistoryWidget *>( d->docks.history->widget() );
//            history->append( str );
//        }

        mEditor->selectAll();
        mEditor->stopAutoCalc();
        mEditor->stopAutoComplete();
//        d->conditions.autoAns = true;
    }
}



