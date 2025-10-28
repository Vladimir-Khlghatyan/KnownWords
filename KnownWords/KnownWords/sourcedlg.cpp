#include "sourcedlg.hpp"

#include <fstream>
#include <vector>
#include <QCoreApplication>
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

    setupLemmas();

    m_textEdit = new PlainTextEdit();
    m_textEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_textEdit->setPlaceholderText("Enter text...");

    m_caseBtn = new QPushButton();
    m_caseBtn->setIcon(QIcon(":/icons/toggle_off.png"));
    m_caseBtn->setProperty("toggleButton", true);
    m_caseBtn->setIconSize(QSize(30, 30));
    m_caseBtn->setCursor(Qt::PointingHandCursor);
    connect(m_caseBtn, &QPushButton::clicked, this, &SourceDlg::onCase);

    m_caseText = new QLabel("case insensitive");
    m_caseText->setProperty("infoKeyLabel", true);

    m_laterBtn = new QPushButton();
    m_laterBtn->setIcon(QIcon(":/icons/toggle_off.png"));
    m_laterBtn->setProperty("toggleButton", true);
    m_laterBtn->setIconSize(QSize(30, 30));
    m_laterBtn->setCursor(Qt::PointingHandCursor);
    connect(m_laterBtn, &QPushButton::clicked, this, &SourceDlg::onLater);

    m_laterText = new QLabel("take words from Later set");
    m_laterText->setProperty("infoKeyLabel", true);

    QGridLayout* gLayout = new QGridLayout();
    gLayout->addWidget(m_caseBtn,   0, 0, 1, 1);
    gLayout->addWidget(m_caseText,  0, 1, 1, 1);
    gLayout->addWidget(m_laterBtn,  1, 0, 1, 1);
    gLayout->addWidget(m_laterText, 1, 1, 1, 1);

    m_cancelBtn = new QPushButton("Cancel");
    m_cancelBtn->setCursor(Qt::PointingHandCursor);
    connect(m_cancelBtn, &QPushButton::clicked, this, &SourceDlg::onCancel);

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


void SourceDlg::onCancel()
{
    reject();
}

void SourceDlg::onOk()
{
    const QString text = m_textEdit->toPlainText();
    if (text.isEmpty()) {
        return;
    }

    static const QRegularExpression whiteSpaceRegex("\\s+");
    static const QRegularExpression startRegex("^[^A-Za-z]+");
    static const QRegularExpression endRegex("[^A-Za-z]+$");
    QStringList parts = text.split(whiteSpaceRegex, Qt::SkipEmptyParts);

    for (QString& part : parts)
    {
        part.replace(startRegex, "");   // remove non-letter characters from the start
        part.replace(endRegex, "");     // remove non-letter characters from the end

        if (!part.isEmpty())
        {
            if (m_isCaseInsensitive) {
                part = part.toLower();
            }
            m_wordSet.insert(part.toStdString());
        }
    }

    checkLemmas();
    accept();
}

bool SourceDlg::isLaterSkipMode() const
{
    const QString text = m_laterText->text();
    return text[0] == QChar('s');
}

void SourceDlg::setupLemmas()
{
    const std::string path = getExecutableGrandparentDirPath() + "/Settings/lemma_base.txt";
    std::fstream file(path, std::ios::in | std::ios::binary);

    if (!file)
    {
        // "Failed to open the file."
        return;
    }

    std::string lineStr;
    while (std::getline(file, lineStr))
    {
        if (!lineStr.empty())
        {
            int i = lineStr.find_first_of(' ');
            const std::string key = lineStr.substr(0,i);
            const std::string value = lineStr.substr(i+1);
            m_lemmaMap[key] = value;
        }
    }
}

void SourceDlg::checkLemmas()
{
    if (m_wordSet.empty()) {
        return;
    }

    const std::string path = getExecutableGrandparentDirPath() + "/Settings/lemma_missing.txt";
    std::fstream missingLemmaFile(path, std::ios::out | std::ios::binary | std::ios::trunc);

    std::string promtText = "I have a list of English words. "
    "I want you to lemmatize them — each word should be reduced to its base dictionary form "
    "(e.g., went → go, children’s → child, wasn’t → be not).\n"
    "Handle contractions and possessives as follows:\n"
    "* can’t → cannot\n"
    "* woman’s → woman\n"
    "* Everything should be lowercase.\n"
    "Then generate a .txt file containing all word<space>lemma pairs, one per line.\n"
    "The separator must be a single space character. Skip non-English words.\n\n";
    missingLemmaFile << promtText;

    std::unordered_set<std::string> uniqueLemmaWordSet;

    int found{}, total = m_wordSet.size();
    for (const std::string& word : m_wordSet)
    {
        auto it = m_lemmaMap.find(word);
        if (it == m_lemmaMap.end()) {
            uniqueLemmaWordSet.insert(word);
            missingLemmaFile << word << '\n';
        } else {
            uniqueLemmaWordSet.insert(it->second);
            ++found;
        }
    }

    m_lemmaMsg = "Lemmatizer success ratio: " + QString::number(static_cast<double>(found) / total * 100, 'f', 1) + "%";
    m_wordSet = std::move(uniqueLemmaWordSet);

}

std::string SourceDlg::getExecutableGrandparentDirPath()
{
    const QString executableDirPath = QCoreApplication::applicationDirPath();
    QDir parentDir(executableDirPath);
    parentDir.cdUp();
    const QString grandparentDirPath = parentDir.absolutePath();
    return grandparentDirPath.toStdString();
}
