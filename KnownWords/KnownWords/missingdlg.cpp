#include "missingdlg.hpp"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTextEdit>

MissingDlg::MissingDlg(QWidget* parent, const QString& text)
    : baseClass(parent)
{
    setWindowIcon(QIcon(":/icons/missing.png"));
    setWindowTitle("Missing lemmas");
    setMinimumSize(450,400);

    m_textEdit = new PlainTextEdit();
    m_textEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_textEdit->setText(text);
    m_textEdit->setReadOnly(true);

    m_clearAndCloseBtn = new QPushButton("Clear");
    m_clearAndCloseBtn->setCursor(Qt::PointingHandCursor);
    m_clearAndCloseBtn->setToolTip("Clear and close");
    connect(m_clearAndCloseBtn, &QPushButton::clicked, this, &MissingDlg::onClearAndClos);

    QHBoxLayout* hLayout = new QHBoxLayout();
    hLayout->addStretch();
    hLayout->addWidget(m_clearAndCloseBtn);

    QVBoxLayout* vLayout = new QVBoxLayout();
    vLayout->addWidget(m_textEdit);
    vLayout->addLayout(hLayout);

    setLayout(vLayout);
}

void MissingDlg::onClearAndClos()
{
    accept();
}
