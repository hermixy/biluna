#ifndef RLINETYPECOMBO_H_
#define RLINETYPECOMBO_H_

#include <QComboBox>
#include <QColor>

#include "RDocument.h"
#include "RLinetype.h"

/**
 * \scriptable
 */
class RLinetypeCombo: public QComboBox {

Q_OBJECT
Q_PROPERTY(bool onlyFixed READ getOnlyFixed WRITE setOnlyFixed);

public:
	RLinetypeCombo(QWidget *parent = 0);

	RLinetype getLinetype();
	void setLinetype(const RLinetype& linetype);
	void init();
	bool getOnlyFixed();
	void setOnlyFixed(bool onlyFixed);

signals:
	void valueChanged(const RLinetype& linetype);

public slots:
	void linetypeChanged(int index);

private:
	bool onlyFixed;
};

Q_DECLARE_METATYPE(RLinetypeCombo*)

#endif
