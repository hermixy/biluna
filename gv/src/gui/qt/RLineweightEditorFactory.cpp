#include <QLabel>

#include "RLineweightEditorFactory.h"
#include "RDebug.h"
#include "RLineweightCombo.h"
#include "RTreePropertyBrowser.h"

RLineweightEditorFactory::RLineweightEditorFactory(QObject *parent) :
	RAbstractEditorFactory<RLineweightPropertyManager> (parent) {

}

RLineweightEditorFactory::~RLineweightEditorFactory() {
}

QWidget* RLineweightEditorFactory::createEditor(RLineweightPropertyManager *manager,
		QtProperty *property, QWidget *parent) {
	RLineweightCombo* editor = new RLineweightCombo(parent);
	editor->setLineweight(manager->value(property));
	if (manager->value(property) == RLineweight::WeightInvalid) {
		editor->insertItem(0, RPropertyEditor::mixed, RLineweight::WeightInvalid);
		editor->setCurrentIndex(editor->findText(RPropertyEditor::mixed));
	}
	connect(editor, SIGNAL(valueChanged(RLineweight::Lineweight)), this,
			SLOT(slotSetValue(RLineweight::Lineweight)));
	this->property = property;
	return editor;
}

void RLineweightEditorFactory::slotSetValue(RLineweight::Lineweight lw) {
	if (property == NULL) {
		return;
	}
	RLineweightPropertyManager* manager = propertyManager(property);
	if (manager == NULL) {
		return;
	}
	manager->setValue(property, lw);
}
