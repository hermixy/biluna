#ifndef RCOLOREDITORFACTORY_H_
#define RCOLOREDITORFACTORY_H_

#include "RAbstractEditorFactory.h"
#include "RColorPropertyManager.h"
#include "RColorCombo.h"

class RColorEditorFactory: public RAbstractEditorFactory<RColorPropertyManager> {

Q_OBJECT

public:
	RColorEditorFactory(QObject *parent = 0);
	~RColorEditorFactory();

signals:
	void editorCreated();
	void editorDestroyed();

public slots:
	void slotSetValue(const RColor& color);
	void slotEditorDestroyed(QObject*);
	void slotDialogRunning(bool isRunning);

protected:
	virtual QWidget* createEditor(RColorPropertyManager *manager,
			QtProperty *property, QWidget *parent);

private:
	RColorCombo* editor;
};

#endif
