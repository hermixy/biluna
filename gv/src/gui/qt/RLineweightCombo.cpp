#include <QSize>

#include "RDebug.h"
#include "RLineweightCombo.h"

RLineweightCombo::RLineweightCombo(QWidget *parent) :
	QComboBox(parent), onlyFixed(false) {

	setIconSize(QSize(32, 16));
	init();
    connect(this, SIGNAL(currentIndexChanged(int)),
        this, SLOT(lineweightChanged(int)));
}

void RLineweightCombo::lineweightChanged(int index) {
	if (itemData(index).value<RLineweight::Lineweight> ()
			== RLineweight::WeightInvalid) {
		return;
	}
	currentLineweight = itemData(index).value<RLineweight::Lineweight> ();
    emit valueChanged(currentLineweight);
}

void RLineweightCombo::init() {
	clear();
	setMaxVisibleItems(12);
	QVariant v;
	QListIterator<QPair<QString, RLineweight::Lineweight> > it(
			RLineweight::getList(onlyFixed));

	while (it.hasNext()) {
		QPair<QString, RLineweight::Lineweight> p = it.next();
		v.setValue<RLineweight::Lineweight> (p.second);
		addItem(RLineweight::getIcon(p.second), RLineweight::tr(
				(const char*) p.first.toUtf8()), v);
	}

	if (!onlyFixed) {
		setLineweight(RLineweight::WeightByLayer);
	} else {
		setLineweight(RLineweight::Weight025);
	}
}

bool RLineweightCombo::getOnlyFixed() {
	return onlyFixed;
}

void RLineweightCombo::setOnlyFixed(bool onlyFixed) {
	this->onlyFixed = onlyFixed;
	init();
}

RLineweight::Lineweight RLineweightCombo::getLineweight() {
	return itemData(currentIndex()).value<RLineweight::Lineweight> ();
}

void RLineweightCombo::setLineweight(RLineweight::Lineweight lw) {
	for (int i = 0; i < count(); ++i) {
        if (itemData(i).isValid() &&
          itemData(i).value<RLineweight::Lineweight> () == lw) {
			setCurrentIndex(i);
			return;
		}
	}
}
