#include <QtPlugin>
#include <QPainter>
#include <QSize>

#include "RWebView.h"
#include "RDebug.h"

RWebView::RWebView(QWidget* parent) :
	QWebView(parent) {
}

QSize RWebView::sizeHint() const {
	return QSize(50, 50);
}

QSize RWebView::minimumSizeHint() const {
	return QSize(20, 20);
}
