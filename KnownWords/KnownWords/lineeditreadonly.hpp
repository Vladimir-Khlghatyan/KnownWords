#ifndef LINEEDITREADONLY_HPP
#define LINEEDITREADONLY_HPP

#include <QLineEdit>

class QWidget;
class QMouseEvent;
class QFocusEvent;
class QObject;
class QEvent;

class LineEditReadOnly : public QLineEdit
{
    using baseClass = QLineEdit;
    Q_OBJECT

public:
    LineEditReadOnly(QWidget* parent = nullptr);
    ~LineEditReadOnly() {}

protected:
    void mouseDoubleClickEvent(QMouseEvent* event) override;
    void focusOutEvent(QFocusEvent* event) override;
    bool eventFilter(QObject* obj, QEvent* event) override;

protected slots:
    void onEditingFinished();
};

#endif // LINEEDITREADONLY_HPP
