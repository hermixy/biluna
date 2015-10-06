/*****************************************************************
 * $Id: db_calculatorwidget.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Nov 24, 2012 1:08:25 PM - Rutger Botermans
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_CALCULATORWIDGET_H
#define DB_CALCULATORWIDGET_H

#include "db_dialogfactory.h"
#include "rb_widget.h"

#include "db_keypadfunction.h"
#include "db_keypadnumeric.h"

class Constants;
class Editor;
class Evaluator;
class Functions;
class ResultDisplay;


/**
 * Calculator widget
 */
class DB_EXPORT DB_CalculatorWidget : public RB_Widget {

    Q_OBJECT

public:
    explicit DB_CalculatorWidget(QWidget* parent = 0);
    ~DB_CalculatorWidget();

    virtual RB_String getId() const { return RB_String(); }
    virtual RB_String getName() const { return "DB Calculator"; }
    virtual int getWidgetType() const { return (int)DB_DialogFactory::WidgetCalculator; }
    virtual RB2::PerspectiveType getPerspectiveType() const { return RB2::PerspectiveDB; }

    virtual void init();
    virtual void initFocus();

    virtual QSize sizeHint() const {
        return QSize(188, 700);
    }


protected slots:
    void slotHandleKeypadFunctionPress(DB_KeypadFunction::Button pb);
    void slotHandleKeypadNumericPress(DB_KeypadNumeric::Button pb);

    void evaluateEditorExpression();
//    void handleEditorTextChange();
protected:
    void keyReleaseEvent(QKeyEvent* ke);

private:
    void setupUi();
    void clearEditor();
    void createFixedConnections();
    void addCloseParenthese();


    Editor* mEditor;
    ResultDisplay* mResultDisplay;

    Constants* mConstants;
    Evaluator* mEvaluator;
    Functions* mFunctions;
};

#endif // DB_CALCULATORWIDGET_H
