#include <QtPlugin>
#include <QColor>
#include <QPalette>
#include <QKeyEvent>

#include "RMath.h"
#include "RMathLineEdit.h"

RMathLineEdit::RMathLineEdit(QWidget* parent) :
    QLineEdit(parent), angle(false), value(0.0) {

    connect(this, SIGNAL(textChanged(QString)),
       this, SLOT(slotTextChanged(QString)));

    slotTextChanged(text());
}

void RMathLineEdit::slotTextChanged(const QString& text) {
    if (originalToolTip.isNull()) {
        originalToolTip = toolTip();
    }

    value = RMath::eval(text);

    QPalette p = palette();
    if (RMath::hasError()) {
        error = RMath::getError();
        //res = defaultValue;
        p.setColor(QPalette::Text, QColor(Qt::red));
        setToolTip(error);
    }
    else {
        error = "";
        p.setColor(QPalette::Text, QColor(Qt::black));
        setToolTip(QString("%1").arg(value, 0, 'f', 6));
    }

    setPalette(p);

    if (!std::isnan(value) && isAngle()) {
        value = RMath::deg2rad(value);
    }

    emit valueChanged(value, error);
}

double RMathLineEdit::getValue() {
    return value;
}

QString RMathLineEdit::getError() {
    return error;
}

void RMathLineEdit::setToolTip(const QString& toolTip) {
    QLineEdit::setToolTip(
        QString(
            "%1%2"
            "<span style=\""
                "color: %3; "
                "font-size: small; "
                "font-family: monospace"
            "\">%4</span>"
        )
        .arg(originalToolTip)
        .arg(originalToolTip.isEmpty() ? "" : "<br>")
        .arg(error.isEmpty() ? "black" : "red")
        .arg(toolTip)
    );
}

void RMathLineEdit::keyPressEvent (QKeyEvent* event) {
    if (event->key()==Qt::Key_Escape) {
        clearFocus();
        event->accept();
    }
    else {
        QLineEdit::keyPressEvent(event);
    }
}

void RMathLineEdit::keyReleaseEvent (QKeyEvent* event) {
    if (event->key()==Qt::Key_Escape) {
        event->accept();
    }
    else {
        QLineEdit::keyReleaseEvent(event);
    }
}
