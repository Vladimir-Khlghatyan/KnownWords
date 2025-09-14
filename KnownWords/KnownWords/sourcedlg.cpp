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
    , m_laterSkipMode(false)
{
    setWindowIcon(QIcon(":/icons/source.png"));
    setWindowTitle("Add source");
    setMinimumSize(450,100);

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
    m_caseText->setProperty("infoKey", true);

    m_laterBtn = new QPushButton();
    m_laterBtn->setIcon(QIcon(":/icons/toggle_off.png"));
    m_laterBtn->setProperty("toggleButton", true);
    m_laterBtn->setIconSize(QSize(30, 30));
    m_laterBtn->setCursor(Qt::PointingHandCursor);
    connect(m_laterBtn, &QPushButton::clicked, this, &SourceDlg::onLater);

    m_laterText = new QLabel("take words from Later set");
    m_laterText->setProperty("infoKey", true);

    QGridLayout* gLayout = new QGridLayout();
    gLayout->addWidget(m_caseBtn,   0, 0, 1, 1);
    gLayout->addWidget(m_caseText,  0, 1, 1, 1);
    gLayout->addWidget(m_laterBtn,  1, 0, 1, 1);
    gLayout->addWidget(m_laterText, 1, 1, 1, 1);

    m_cancelBtn = new QPushButton("Cancel");
    m_cancelBtn->setCursor(Qt::PointingHandCursor);
    connect(m_cancelBtn, &QPushButton::clicked, this, &SourceDlg::onCencel);

    m_okBtn = new QPushButton("Ok");
    m_okBtn->setCursor(Qt::PointingHandCursor);
    connect(m_okBtn, &QPushButton::clicked, this, &SourceDlg::onOk);

    QHBoxLayout* hLayout = new QHBoxLayout();
    /*hLayout->addWidget(m_caseBtn);
    hLayout->addWidget(m_caseText)*/;
    hLayout->addLayout(gLayout);
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

void SourceDlg::onLater()
{
    m_laterSkipMode = !m_laterSkipMode;
    if (m_laterSkipMode)
    {
        m_laterBtn->setIcon(QIcon(":/icons/toggle_on.png"));
        m_laterText->setText("skip words from Later set");
    }
    else
    {
        m_laterBtn->setIcon(QIcon(":/icons/toggle_off.png"));
        m_laterText->setText("take words from Later set");
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

bool SourceDlg::isLaterSkipMode() const
{
    const QString text = m_laterText->text();
    return text[0] == QChar('s');
}
