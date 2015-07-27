#ifndef RLINETYPEEDITORFACTORY_H_
#define RLINETYPEEDITORFACTORY_H_

#include "RAbstractEditorFactory.h"
#include "RLinetypePropertyManager.h"

class RLinetypeEditorFactory: public RAbstractEditorFactory<RLinetypePropertyManager> {

Q_OBJECT

public:
	RLinetypeEditorFactory(QObject *parent = 0);
	virtual ~RLinetypeEditorFactory();

public slots:
	void slotSetValue(const RLinetype& linetype);

protected:
	virtual QWidget* createEditor(RLinetypePropertyManager *manager,
			QtProperty *property, QWidget *parent);
};

#endif /* RLINETYPEEDITORFACTORY_H_ */
