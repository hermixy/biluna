#include <QLabel>

#include "RColorEditorFactory.h"
#include "RDebug.h"
#include "RColorCombo.h"
#include "RTreePropertyBrowser.h"

RColorEditorFactory::RColorEditorFactory(QObject *parent) :
	RAbstractEditorFactory<RColorPropertyManager> (parent), editor(NULL) {

}

RColorEditorFactory::~RColorEditorFactory() {
	if (editor != NULL) {
		delete editor;
	}
}

QWidget* RColorEditorFactory::createEditor(RColorPropertyManager *manager,
		QtProperty *property, QWidget *parent) {
	editor = new RColorCombo(parent);
	editor->setColor(manager->value(property));
	if (!manager->value(property).isValid()) {
		editor->insertItem(0, RPropertyEditor::mixed, RColor());
		editor->setCurrentIndex(editor->findText(RPropertyEditor::mixed));
	}
	connect(editor, SIGNAL(valueChanged(const RColor&)), this,
			SLOT(slotSetValue(const RColor&)));
	connect(editor, SIGNAL(destroyed(QObject*)), this,
			SLOT(slotEditorDestroyed(QObject*)));
	connect(editor, SIGNAL(dialogRunning(bool)), this,
			SLOT(slotDialogRunning(bool)));
	this->property = property;
	return editor;
}

void RColorEditorFactory::slotSetValue(const RColor& color) {
	if (property == NULL) {
		return;
	}
	RColorPropertyManager* manager = propertyManager(property);
	if (manager == NULL) {
		return;
	}
	manager->setValue(property, color);
}

void RColorEditorFactory::slotDialogRunning(bool isRunning) {
	if (isRunning) {
		emit editorCreated();
	} else {
		emit editorDestroyed();
	}
}

void RColorEditorFactory::slotEditorDestroyed(QObject* /*obj*/) {
	editor = NULL;
	property = NULL;
}
