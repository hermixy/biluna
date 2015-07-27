#include "RLinetypeEditorFactory.h"

#include "RLinetypeCombo.h"
#include "RPropertyEditor.h"

RLinetypeEditorFactory::RLinetypeEditorFactory(QObject *parent) :
	RAbstractEditorFactory<RLinetypePropertyManager> (parent) {
}

RLinetypeEditorFactory::~RLinetypeEditorFactory() {
}

QWidget* RLinetypeEditorFactory::createEditor(
		RLinetypePropertyManager *manager, QtProperty *property,
		QWidget *parent) {
	RLinetypeCombo* editor = new RLinetypeCombo(parent);
	editor->setLinetype(manager->value(property));
	if (!manager->value(property).isValid()) {
		QVariant v;
		v.setValue<RLinetype> (RLinetype());
		editor->insertItem(0, RPropertyEditor::mixed, v);
		editor->setCurrentIndex(editor->findText(RPropertyEditor::mixed));
	}
	connect(editor, SIGNAL(valueChanged(const RLinetype&)), this,
			SLOT(slotSetValue(const RLinetype&)));
	this->property = property;
	return editor;
}

void RLinetypeEditorFactory::slotSetValue(const RLinetype& linetype) {
	if (property == NULL) {
		return;
	}
	RLinetypePropertyManager* manager = propertyManager(property);
	if (manager == NULL) {
		return;
	}
	manager->setValue(property, linetype);
}

