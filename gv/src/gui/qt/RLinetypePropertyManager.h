#ifndef RLINETYPEPROPERTYMANAGER_H_
#define RLINETYPEPROPERTYMANAGER_H_

//#include <QtAbstractPropertyManager>
//#include <QtProperty>
#include "qtpropertybrowser.h"
#include <QString>
#include <QIcon>

#include "RLinetype.h"

class RLinetypePropertyManager: public QtAbstractPropertyManager {

Q_OBJECT

public:
	RLinetypePropertyManager(QObject* parent = NULL);
	virtual RLinetype value(const QtProperty * property) const;

signals:
	void valueChanged(QtProperty* property, const RLinetype linetype);

public slots:
	void setValue(QtProperty * property, const RLinetype& linetype);
	void setMixedValue(QtProperty * property);

protected:
	virtual void initializeProperty(QtProperty * property);
	virtual void uninitializeProperty(QtProperty * property);
	virtual QIcon valueIcon(const QtProperty * property) const;
	virtual QString valueText(const QtProperty * property) const;

private:
	RLinetype linetype;
};

#endif
