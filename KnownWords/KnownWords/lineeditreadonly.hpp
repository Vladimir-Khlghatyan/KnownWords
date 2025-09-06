#ifndef LINEEDITREADONLY_HPP
#define LINEEDITREADONLY_HPP

#include <QLineEdit>
#include <QTextEdit>
#include <QMimeData>

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

////////////////////////////////////////////////////

class PlainTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    using QTextEdit::QTextEdit; // inherit constructors

protected:
    void insertFromMimeData(const QMimeData *source) override {
        if (source->hasText()) {
            insertPlainText(source->text()); // paste only plain text
        }
        // ignore everything else
        }
};

#endif // LINEEDITREADONLY_HPP
