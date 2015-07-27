#include <QtPlugin>
#include <QPainter>
#include <QSize>

#include "RTextEdit.h"
#include "RDebug.h"

RTextEdit::RTextEdit(QWidget* parent) :
    QTextEdit(parent) {
}

QSize RTextEdit::sizeHint() const {
    return QSize(256, 38);
}

QSize RTextEdit::minimumSizeHint() const {
    return QSize(256, 38);
}
