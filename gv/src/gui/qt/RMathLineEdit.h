#ifndef RMATHLINEEDIT_H_
#define RMATHLINEEDIT_H_

#include <QMetaType>
#include <QLineEdit>
#include <QtDesigner/QDesignerExportWidget>

/**
 * \scriptable
 */
class /*QDESIGNER_WIDGET_EXPORT*/ RMathLineEdit: public QLineEdit {

Q_OBJECT
Q_PROPERTY(bool angle READ isAngle WRITE setAngle)
//Q_PROPERTY(double defaultValue READ getDefaultValue WRITE setDefaultValue);

public:
	RMathLineEdit(QWidget* parent);

    bool isAngle() {
       return angle;
    }
    void setAngle(bool on) {
       angle = on;
    }
    double getValue();
    QString getError();

    void setToolTip(const QString& toolTip);

    /*
    double getDefaultValue() {
        return defaultValue;
    }
    void setDefaultValue(double v) {
        defaultValue = v;
    }
    */
protected:
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void keyReleaseEvent(QKeyEvent* event);

public slots:
    void slotTextChanged(const QString& text);

signals:
    void valueChanged(double value, const QString& error);

private:
    bool angle;
    double value;
    QString error;
    QString originalToolTip;
    //double defaultValue;
};

Q_DECLARE_METATYPE(RMathLineEdit*)

#endif
