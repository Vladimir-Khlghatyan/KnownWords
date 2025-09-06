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
    , m_isCaseInsensitive(true)
{
    setWindowIcon(QIcon(":/icons/source.png"));
    setWindowTitle("Add source");
    setMinimumSize(400,100);

    m_textEdit = new PlainTextEdit();
    m_textEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_textEdit->setPlaceholderText("Add text here...");

    m_caseBtn = new QPushButton();
    m_caseBtn->setIcon(QIcon(":/icons/toggle_off.png"));
    m_caseBtn->setProperty("toggleButton", true);
    m_caseBtn->setIconSize(QSize(30, 30));
    m_caseBtn->setCursor(Qt::PointingHandCursor);
    connect(m_caseBtn, &QPushButton::clicked, this, &SourceDlg::onCase);

    m_caseText = new QLabel("case insensitive");

    m_cancelBtn = new QPushButton("Cancel");
    m_cancelBtn->setCursor(Qt::PointingHandCursor);
    connect(m_cancelBtn, &QPushButton::clicked, this, &SourceDlg::onCencel);

    m_okBtn = new QPushButton("Ok");
    m_okBtn->setCursor(Qt::PointingHandCursor);
    connect(m_okBtn, &QPushButton::clicked, this, &SourceDlg::onOk);

    QHBoxLayout* hLayout = new QHBoxLayout();
    hLayout->addWidget(m_caseBtn);
    hLayout->addWidget(m_caseText);
    hLayout->addStretch();
    hLayout->addWidget(m_cancelBtn);
    hLayout->addWidget(m_okBtn);

    QVBoxLayout* vLayout = new QVBoxLayout();
    vLayout->addWidget(m_textEdit);
    vLayout->addLayout(hLayout);

    setLayout(vLayout);
}

void SourceDlg::onCase()
{
    m_isCaseInsensitive = !m_isCaseInsensitive;
    if (m_isCaseInsensitive)
    {
        m_caseBtn->setIcon(QIcon(":/icons/toggle_off.png"));
        m_caseText->setText("case insensitive");
    }
    else
    {
        m_caseBtn->setIcon(QIcon(":/icons/toggle_on.png"));
        m_caseText->setText("case sensitive");
    }
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

    for (QString& part : parts)
    {
        part.replace(alphaRegex, ""); // remove non non-letter characters from the end ($ = end of string)
        if (m_isCaseInsensitive) {
            part = part.toLower();
        }
        m_wordSet.insert(part.toStdString());
    }

    accept();
}
