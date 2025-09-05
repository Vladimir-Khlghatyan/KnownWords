#include "lineeditreadonly.hpp"

#include <QApplication>
#include <QMouseEvent>
#include <QFocusEvent>

LineEditReadOnly::LineEditReadOnly(QWidget* parent)
    : baseClass(parent)
{
//    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setAlignment(Qt::AlignCenter);
    setFixedHeight(60);
    setReadOnly(true);
    setFocusPolicy(Qt::StrongFocus);

    qApp->installEventFilter(this);

    connect(this, &QLineEdit::editingFinished, this, &LineEditReadOnly::onEditingFinished);
}

void LineEditReadOnly::mouseDoubleClickEvent(QMouseEvent* event)
{
    setReadOnly(false);
    baseClass::mouseDoubleClickEvent(event);
    deselect();
}

void LineEditReadOnly::onEditingFinished()
{
    setReadOnly(true);
}

void LineEditReadOnly::focusOutEvent(QFocusEvent* event)
{
    baseClass::focusOutEvent(event);
}

bool LineEditReadOnly::eventFilter(QObject* obj, QEvent* event)
{
    if (event->type() == QEvent::MouseButtonPress && this->hasFocus())
    {
        if (QWidget* w = qobject_cast<QWidget*>(obj))
        {
            if (w != this && !this->isAncestorOf(w))
            {
                this->clearFocus();
                this->setReadOnly(true);
            }
        }
    }
    return QObject::eventFilter(obj, event);
}
