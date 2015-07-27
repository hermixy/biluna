#ifndef RLINEWEIGHTCOMBO_H_
#define RLINEWEIGHTCOMBO_H_

#include <QComboBox>

#include "RGlobal.h"
#include "RLineweight.h"

/**
 * \scriptable
 */
class RLineweightCombo: public QComboBox {

Q_OBJECT
Q_PROPERTY(bool onlyFixed READ getOnlyFixed WRITE setOnlyFixed);

public:
	RLineweightCombo(QWidget *parent = 0);

	void init();

	RLineweight::Lineweight getLineweight();
	void setLineweight(RLineweight::Lineweight lw);

	bool getOnlyFixed();
	void setOnlyFixed(bool onlyFixed);

signals:
    void valueChanged(RLineweight::Lineweight lw);

public slots:
	void lineweightChanged(int index);

private:
	RLineweight::Lineweight currentLineweight;
	bool onlyFixed;
};

Q_DECLARE_METATYPE(RLineweightCombo*)

#endif
