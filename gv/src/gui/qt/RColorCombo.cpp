#include <QColorDialog>
#include <QColor>
#include <QMapIterator>
#include <QCoreApplication>

#include "RDebug.h"
#include "RColorCombo.h"
#include "RColor.h"

RColorCombo::RColorCombo(QWidget *parent) :
	QComboBox(parent), onlyFixed(false), blackWhite(true), showAlphaChannel(
			false) {
	setIconSize(QSize(32, 16));
	init();
	connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(colorChanged(int)));
}

void RColorCombo::init(){
	clear();
	setMaxVisibleItems(15);
	QVariant v;
	QListIterator<QPair<QString, RColor> > it(RColor::getList(onlyFixed,
			blackWhite));

	while (it.hasNext()) {
		QPair<QString, RColor> p = it.next();
		if (p.second.isValid()) {
			v.setValue<RColor> (p.second);
			addItem(RColor::getIcon(p.second), RColor::tr(
					(const char*) p.first.toUtf8()), v);
		} else {
			addItem(RColor::getIcon(p.second), RColor::tr(
					(const char*) p.first.toUtf8()));
		}
	}

	if (!onlyFixed) {
		setColor(RColor(RColor::ByLayer));
	} else if (blackWhite) {
		setColor(RColor(Qt::black, RColor::BlackWhite));
	} else {
		setColor(RColor(Qt::black));
	}
}

bool RColorCombo::getOnlyFixed() {
	return onlyFixed;
}

void RColorCombo::setOnlyFixed(bool onlyFixed) {
	this->onlyFixed = onlyFixed;
	init();
}

bool RColorCombo::isBlackWhite() {
	return blackWhite;
}

void RColorCombo::setBlackWhite(bool blackWhite) {
	this->blackWhite = blackWhite;
	init();
}

bool RColorCombo::getShowAlphaChannel() {
	return showAlphaChannel;
}

void RColorCombo::setShowAlphaChannel(bool showAlphaChannel) {
	this->showAlphaChannel = showAlphaChannel;
	init();
}

void RColorCombo::colorChanged(int index) {
	emit dialogRunning(true);
	if (!itemData(index).isValid()) {
		QColor clr;
		if (isVisible()) {
			clr = QColorDialog::getColor(currentColor, NULL, "",
					showAlphaChannel ? QColorDialog::ShowAlphaChannel
							: (QColorDialog::ColorDialogOption) 0);
		}
		if (clr.isValid()) {
			currentColor = clr;
		}
		setColor(currentColor);
	} else {
		if (!itemData(index).value<RColor> ().isValid()) {
			return;
		}
		currentColor = itemData(index).value<RColor> ();
	}
	emit valueChanged(currentColor);
	emit dialogRunning(false);
}

RColor RColorCombo::getColor() {
	return currentColor;
}

void RColorCombo::setColor(const RColor& col) {
	for (int i = 0; i < count(); ++i) {
		if (itemData(i).isValid()) {
			RColor c = itemData(i).value<RColor> ();
			if (c.equalsCorrected(col)) {
				currentColor = c;
				setCurrentIndex(i);
				return;
			}
		}
	}
	if (!col.isFixed() || !col.isValid()) {
		return;
	}
	currentColor = col;
	QVariant v;
	v.setValue<RColor> (currentColor);
	blockSignals(true);
	QString custom = tr("Custom");
	int i = findText(custom, Qt::MatchExactly);
	if (i != -1) {
		removeItem(i);
	}
	insertItem(count() - 1, RColor::getIcon(currentColor), custom, v);
	setCurrentIndex(count() - 2);
	blockSignals(false);
}
