#ifndef RABSTRACTEDITORFACTORY_H_
#define RABSTRACTEDITORFACTORY_H_

//#include <QtVariantEditorFactory>
//#include <QtProperty>
#include "qtpropertybrowser.h"

template<class TPM>
class RAbstractEditorFactory: public QtAbstractEditorFactory<TPM> {

public:
	RAbstractEditorFactory(QObject* parent = 0) :
		QtAbstractEditorFactory<TPM>(parent), property(NULL) {
	}

	~RAbstractEditorFactory() {
	}

protected:
	virtual void connectPropertyManager(TPM* /*manager*/) {
	}

	virtual void disconnectPropertyManager(TPM* /*manager*/) {
	}

protected:
	QtProperty* property;
};

#endif
