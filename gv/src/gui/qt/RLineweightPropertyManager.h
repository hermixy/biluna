#ifndef RLINEWEIGHTPROPERTYMANAGER_H_
#define RLINEWEIGHTPROPERTYMANAGER_H_

//#include <QtAbstractPropertyManager>
//#include <QtProperty>
#include "qtpropertybrowser.h"
#include <QString>
#include <QIcon>

#include "RLineweight.h"

class RLineweightPropertyManager: public QtAbstractPropertyManager {

Q_OBJECT

public:
	RLineweightPropertyManager(QObject* parent = NULL);
	virtual RLineweight::Lineweight value(const QtProperty * property) const;

signals:
	void valueChanged(QtProperty* property, RLineweight::Lineweight lw);

public slots:
	void setValue(QtProperty * property, RLineweight::Lineweight lw);
	void setMixedValue(QtProperty * property);

protected:
	virtual void initializeProperty(QtProperty * property);
	virtual void uninitializeProperty(QtProperty * property);
	virtual QIcon valueIcon(const QtProperty * property) const;
	virtual QString valueText(const QtProperty * property) const;

private:
	RLineweight::Lineweight lw;
};

#endif
