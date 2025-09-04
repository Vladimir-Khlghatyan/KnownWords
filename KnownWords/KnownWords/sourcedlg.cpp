#include "sourcedlg.hpp"

#include <fstream>
#include <vector>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QFileDialog>
#include <QTextEdit>
#include <QDebug>
#include <QRegularExpression>

SourceDlg::SourceDlg(QWidget* parent)
    : baseClass(parent)
{
    m_textEdit = new QTextEdit();
    m_textEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_textEdit->setPlaceholderText("Add text here...");

    setWindowIcon(QIcon(":/icons/source.png"));
    setWindowTitle("Add source");
    setMinimumSize(400,100);


    m_cancelBtn = new QPushButton("Cancel");
    m_cancelBtn->setCursor(Qt::PointingHandCursor);
    m_cancelBtn->setProperty("dialog", true);
    connect(m_cancelBtn, &QPushButton::clicked, this, &SourceDlg::onCencel);

    m_okBtn = new QPushButton("Ok");
    m_okBtn->setCursor(Qt::PointingHandCursor);
    m_okBtn->setProperty("dialog", true);
    connect(m_okBtn, &QPushButton::clicked, this, &SourceDlg::onOk);

    QHBoxLayout* hLayout = new QHBoxLayout();
    hLayout->addStretch();
    hLayout->addWidget(m_cancelBtn);
    hLayout->addWidget(m_okBtn);

    QVBoxLayout* vLayout = new QVBoxLayout();
    vLayout->addWidget(m_textEdit);
    vLayout->addLayout(hLayout);

    setLayout(vLayout);
}

void SourceDlg::onCencel()
{
    reject();
}

void SourceDlg::onOk()
{
    const QString text = m_textEdit->toPlainText();
    if (text.isEmpty()) {
        return;
    }

    static const QRegularExpression wsRegex("\\s+"), alphaRegex("[^A-Za-z]+$");
    QStringList parts = text.split(wsRegex, Qt::SkipEmptyParts);

    for (QString &part : parts)
    {
        part.replace(alphaRegex, ""); // remove non non-letter characters from the end ($ = end of string)
        m_wordSet.insert(part.toStdString());
    }

    accept();
}
