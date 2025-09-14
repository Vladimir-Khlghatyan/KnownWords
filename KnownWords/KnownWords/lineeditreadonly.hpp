#ifndef LINEEDITREADONLY_HPP
#define LINEEDITREADONLY_HPP

#include <QLineEdit>
#include <QTextEdit>
#include <QMimeData>
#include <QPushButton>
#include <QMouseEvent>
#include <QTimer>

class QWidget;
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

////////////////////////////////////////////////////

class MyButton : public QPushButton
{
    Q_OBJECT

    public:
        using QPushButton::QPushButton; // inherit constructors

    signals:
        void singleClicked();
        void doubleClicked();

    protected:
        void mousePressEvent(QMouseEvent* event) override
        {
            if (event->button() == Qt::LeftButton)
            {
                if (!m_clickTimer)
                {
                    m_clickTimer = new QTimer(this);
                    m_clickTimer->setSingleShot(true);
                    connect(m_clickTimer, &QTimer::timeout, this, [this]() {
                        emit singleClicked();
                    });
                }

                if (m_clickTimer->isActive())
                {
                    // second click arrived before timeout → double-click
                    m_clickTimer->stop();
                    emit doubleClicked();
                } else {
                    // first click → start timer
                    m_clickTimer->start(250);
                }
            }

            QPushButton::mousePressEvent(event);
        }

    private:
        QTimer* m_clickTimer = nullptr;
};

#endif // LINEEDITREADONLY_HPP
