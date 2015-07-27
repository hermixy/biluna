#include "RListWidget.h"
#include "RDebug.h"

#include <QContextMenuEvent>

/**
 * Default Constructor.
 */
RListWidget::RListWidget(QWidget* parent) :
	QListWidget(parent) {
	itemPressed = NULL;
#ifdef Q_OS_MAC
    iconOffset = 7;
#else
    iconOffset = 0;
#endif
}

/**
 * Destructor
 */
RListWidget::~RListWidget() {
}

void RListWidget::contextMenuEvent(QContextMenuEvent* e) {
	RDebug::debug(QString("RListWidget::contextMenuEvent: "));
	if (e != NULL) {
		QListWidgetItem* item = itemAt(e->pos());
		if (item != NULL) {
			setCurrentItem(item);
			emit itemClicked(item);
		}
	}
	e->ignore();
}

void RListWidget::mousePressEvent(QMouseEvent* e) {
	if (e->x()-iconOffset < iconSize().width()) {
		itemPressed = itemAt(e->pos());
	} else {
		e->ignore();
		QListWidget::mousePressEvent(e);
	}
}

void RListWidget::mouseReleaseEvent(QMouseEvent* e) {
	if (e->x()-iconOffset < iconSize().width()) {
		QListWidgetItem* item = itemAt(e->pos());
		if (item != NULL && item == itemPressed) {
			emit iconClicked(e->x()-iconOffset, item);
		}
	} else {
		e->ignore();
		QListWidget::mouseReleaseEvent(e);
	}
}

void RListWidget::mouseMoveEvent(QMouseEvent* e) {
	if (e->x()-iconOffset < iconSize().width()) {
	} else {
		e->ignore();
		QListWidget::mouseMoveEvent(e);
	}
}
