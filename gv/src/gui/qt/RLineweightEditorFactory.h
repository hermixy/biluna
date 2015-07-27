#ifndef RLINEWEIGHTEDITORFACTORY_H_
#define RLINEWEIGHTEDITORFACTORY_H_

#include "RAbstractEditorFactory.h"
#include "RLineweightPropertyManager.h"
#include "RLineweightCombo.h"

class RLineweightEditorFactory: public RAbstractEditorFactory<RLineweightPropertyManager> {

Q_OBJECT

public:
	RLineweightEditorFactory(QObject *parent = 0);
	~RLineweightEditorFactory();

public slots:
	void slotSetValue(RLineweight::Lineweight lw);

protected:
	virtual QWidget* createEditor(RLineweightPropertyManager *manager,
			QtProperty *property, QWidget *parent);

};

#endif
