#include <QColorDialog>
#include <QColor>
#include <QMapIterator>
#include <QCoreApplication>
#include <QHBoxLayout>
#include <QFontComboBox>
#include <QComboBox>
#include <QLabel>
#include <QDebug>

#include "RDebug.h"
#include "RFontChooserWidget.h"

// TODO non-TTF fonts fails to save / load
// TODO test case for saving / loading fonts with RSettings

RFontChooserWidget::RFontChooserWidget(QWidget *parent) :
	QWidget(parent), font(QFont()), cbSize(NULL), cbFont(NULL), lbSampleText(
			NULL) {

	QGridLayout* layout = new QGridLayout;
	layout->setMargin(0);

    lbLabel = new QLabel("");
    layout->addWidget(lbLabel, 0,0);

	cbFont = new QFontComboBox();
	cbFont->setObjectName("FontFamily");
	cbFont->setFontFilters(QFontComboBox::ScalableFonts);
//	cbFont->setFontFilters(QFontComboBox::ProportionalFonts);
//	cbFont->setFontFilters(QFontComboBox::NonScalableFonts);

	connect(cbFont, SIGNAL(currentFontChanged(const QFont&)), this,
			SLOT(fontChanged(const QFont &)));
	layout->addWidget(cbFont, 0, 1);

	cbSize = new QComboBox();
	cbSize->setObjectName("FontSize");
	connect(cbSize, SIGNAL(activated(int)), this,
			SLOT(sizeChanged(int)));
	layout->addWidget(cbSize, 0, 2);

	lbSampleText = new QLabel("The quick brown fox jumps over the lazy dog");
	lbSampleText->setMinimumWidth(0);
	layout->addWidget(lbSampleText, 1, 0, 1, 3);

	QLabel* lbSpace = new QLabel();
	lbSpace->setBaseSize(8, 8);
	layout->addWidget(lbSpace, 2, 0, 1, 3);

	setLayout(layout);
}

void RFontChooserWidget::updateSizeCombo() {
	if (cbSize == NULL) {
		return;
	}

	cbSize->clear();
	QFontDatabase fontDb;
	QListIterator<int> i(fontDb.pointSizes(font.family()));
	while (i.hasNext()) {
		int s = i.next();
		cbSize->addItem(QString("%1").arg(s), s);
	}
    int s = font.pointSize();
    if (cbSize->findData(s)==-1) {
        cbSize->addItem(QString("%1").arg(s), s);
    }
	cbSize->setCurrentIndex(cbSize->findData(s));
}

void RFontChooserWidget::fontChanged(const QFont& font) {
    QFont f = font;
    int currentIndex = cbSize->currentIndex();
    if (currentIndex!=-1) {
        f.setPointSize(cbSize->itemData(currentIndex).toInt());
    }
	setFont(f);
}

QFont RFontChooserWidget::getFont() const {
	return font;
}

void RFontChooserWidget::setFont(const QFont &font) {
	this->font = font;
	cbFont->setCurrentFont(font);
	updateSizeCombo();
	lbSampleText->setFont(font);
}

QString RFontChooserWidget::getLabel() const {
    return lbLabel->text();
}

void RFontChooserWidget::setLabel(const QString& l) {
    lbLabel->setText(l);
}

void RFontChooserWidget::sizeChanged(int index) {
	if (cbFont == NULL || cbSize == NULL || index < 0) {
		return;
	}
	if (!cbSize->itemData(index).isValid()) {
		RDebug::warning(
				QString("RFontChooserWidget::sizeChanged: invalid item data at index %1")
				.arg(index));
		return;
	}

	int size = cbSize->itemData(index).toInt();
	setSize(size);
}

int RFontChooserWidget::getSize() const {
	return font.pointSize();
}

void RFontChooserWidget::setSize(int size) {
	if (size <= 0) {
		RDebug::warning(QString(
				"RFontChooserWidget::setSize: size <= 0 (size = %1)").arg(size));
		return;
	}
	this->font.setPointSize(size);
	lbSampleText->setFont(font);
}

void RFontChooserWidget::resizeEvent(QResizeEvent* event) {
	lbSampleText->setMaximumWidth(width());
}
