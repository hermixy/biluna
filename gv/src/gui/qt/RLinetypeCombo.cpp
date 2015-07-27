#include <QSize>
#include <QSet>

#include "RDebug.h"
#include "RLinetypeCombo.h"

RLinetypeCombo::RLinetypeCombo(QWidget *parent) :
	QComboBox(parent), onlyFixed(false) {
	setIconSize(QSize(32, 16));

	init();

    connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(linetypeChanged(int)));
}

void RLinetypeCombo::linetypeChanged(int index) {
	if (!itemData(index).isValid()) {
		return;
	}
	if (!itemData(index).value<RLinetype> ().isValid()) {
		return;
	}
	emit valueChanged(itemData(index).value<RLinetype> ());
}

void RLinetypeCombo::init() {
	RDebug::debug(QString("RLinetypeCombo::init: "));
	clear();
	setMaxVisibleItems(12);
	QVariant v;
	QListIterator<QPair<QString, RLinetype> > it(RLinetype::getList(onlyFixed));

	while (it.hasNext()) {
		QPair<QString, RLinetype> p = it.next();
		if (p.second.isValid()) {
			v.setValue<RLinetype> (p.second);
			addItem(RLinetype::getIcon(p.second), RLinetype::tr(
					(const char*) p.first.toUtf8()), v);
		} else {
			addItem(RLinetype::getIcon(p.second), RLinetype::tr(
					(const char*) p.first.toUtf8()));
		}
	}

	if (!onlyFixed) {
        setLinetype(RLinetype(NULL, "BYLAYER"));
	} else {
        setLinetype(RLinetype(NULL, "CONTINUOUS"));
	}
}

RLinetype RLinetypeCombo::getLinetype() {
	return itemData(currentIndex()).value<RLinetype> ();
}

void RLinetypeCombo::setLinetype(const RLinetype& linetype) {
	for (int i = 0; i < count(); ++i) {
		if (itemData(i).isValid()) {
			RLinetype t = itemData(i).value<RLinetype> ();
			if (t == linetype) {
				setCurrentIndex(i);
				return;
			}
		}
	}
}

bool RLinetypeCombo::getOnlyFixed() {
	return onlyFixed;
}

void RLinetypeCombo::setOnlyFixed(bool onlyFixed) {
	this->onlyFixed = onlyFixed;
	init();
}
