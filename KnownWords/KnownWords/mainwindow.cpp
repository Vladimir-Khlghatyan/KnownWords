#include "mainwindow.hpp"

#include "sourcedlg.hpp"
#include "lineeditreadonly.hpp"
#include "texttospeech.hpp"

#include <QCoreApplication>
#include <QDir>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QTimer>

#include <fstream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_knownWordCnt(0)
    , m_currWordCnt(0)
    , m_currKnownCnt(0)
{
    setWindowTitle("Known Words");
    setWindowIcon(QIcon(":/icons/logo.png"));
    setStyleSheet(MAIN_WINDOW_STYLE);
    setFixedSize(QSize(400, 300));

    // setup Main Window layout
    QWidget* centralWidget = new QWidget();
    setCentralWidget(centralWidget);
    QGridLayout* layout = new QGridLayout(centralWidget);
    layout->setContentsMargins(8,8,8,8);
    layout->setSpacing(4);

    parseKnownWords();

    m_infoLayout = new QVBoxLayout();
    m_totalKnownMsg = addMessage("Total Known Words:");
    m_sourceMsg = addMessage("Current Source:");
    m_sourceMsg->setToolTip("known / total");
    m_percentMsg = addMessage("Known Percentage:");
    updateMessage();

    m_lineEdit = new LineEditReadOnly();
    m_lineEdit->setToolTip("Double click to edit.");
    m_lineEdit->setText("hello");

    m_sourceBtn = getButtonWithIcon("source", "Add new source");
    connect(m_sourceBtn, &QPushButton::clicked, this, &MainWindow::onSourceBtn);

    m_soundBtn = getButtonWithIcon("soundPlay", "Listen");
//    QSizePolicy sp = m_soundBtn->sizePolicy();
//    sp.setRetainSizeWhenHidden(true);
//    m_soundBtn->setSizePolicy(sp);
#ifdef _PLAY_SOUND_

    connect(m_soundBtn, &QPushButton::clicked, this, &MainWindow::onSoundBtn);
    m_textToSpeech = new TextToSpeech(this);
    connect(m_textToSpeech, &TextToSpeech::errorOccurred, this, &MainWindow::onErrorMsg);

#else
    ui->soundPlayBtn->hide();
#endif

    m_skipBtn = new QPushButton("Skip");
    m_skipBtn->setCursor(Qt::PointingHandCursor);
    connect(m_skipBtn, &QPushButton::clicked, this, &MainWindow::onSkipBtn);

    m_laterBtn = new QPushButton("Later");
    m_laterBtn->setCursor(Qt::PointingHandCursor);
    connect(m_laterBtn, &QPushButton::clicked, this, &MainWindow::onLaterBtn);

    m_deleteBtn = new QPushButton("Delete");
    m_deleteBtn->setCursor(Qt::PointingHandCursor);
    connect(m_deleteBtn, &QPushButton::clicked, this, &MainWindow::onDeleteBtn);

    m_knownBtn = new QPushButton("Known");
    m_knownBtn->setCursor(Qt::PointingHandCursor);
    connect(m_knownBtn, &QPushButton::clicked, this, &MainWindow::onKnownBtn);

    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLayout->addStretch();
    btnLayout->addWidget(m_skipBtn, Qt::AlignTop);
    btnLayout->addWidget(m_laterBtn, Qt::AlignTop);
    btnLayout->addWidget(m_deleteBtn, Qt::AlignTop);
    btnLayout->addWidget(m_knownBtn, Qt::AlignTop);
    btnLayout->addStretch();

    m_translateMsg = new QLabel();
    m_translateMsg->setProperty("translate", true);
    m_translateMsg->setAlignment(Qt::AlignCenter);
    m_translateMsg->setText("բարև");
    m_translateMsg->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    m_errorMsg = new QLabel();
    m_errorMsg->setProperty("error", true);

    layout->addLayout(m_infoLayout,   0, 0, 2, 1, Qt::AlignLeft | Qt::AlignTop);
    layout->addWidget(m_lineEdit,     2, 0, 1, 2, Qt::AlignVCenter);
    layout->addLayout(btnLayout,      3, 0, 1, 2);
    layout->addWidget(m_sourceBtn,    0, 1, 1, 1, Qt::AlignRight);
    layout->addWidget(m_soundBtn,     1, 1, 1, 1, Qt::AlignRight);
    layout->addWidget(m_errorMsg,     5, 0, 1, 1, Qt::AlignLeft);
    layout->addWidget(m_translateMsg, 4, 0, 1, 2);
}

MainWindow::~MainWindow()
{
}

QPushButton* MainWindow::getButtonWithIcon(const QString& iconName, const QString& tooltip,
                                     bool isEnabled, const QSize& buttonSize, const QSize& iconSize)
{
    QPushButton* btn = new QPushButton(/*this*/);
    btn->setProperty("iconButton", true);
    btn->setIcon(QIcon(":/icons/" + iconName + ".png"));
    btn->setFixedSize(buttonSize);
    btn->setIconSize(iconSize);
    btn->setCursor(Qt::PointingHandCursor);
    btn->setToolTip(tooltip);
    btn->setEnabled(isEnabled);
    return btn;
}

QLabel* MainWindow::addMessage(const char* keyMessage)
{
    QLabel* keyLabel = new QLabel(keyMessage);
    QLabel* valLabel = new QLabel();

    keyLabel->setProperty("infoKey", true);
    valLabel->setProperty("infoVal", true);

    QHBoxLayout* hLayout = new QHBoxLayout();
    hLayout->setContentsMargins(0,0,0,0);
    hLayout->setSpacing(8);
    hLayout->addWidget(keyLabel);
    hLayout->addWidget(valLabel);

    if (m_infoLayout) {
        m_infoLayout->addLayout(hLayout);
    }
    return valLabel;
}

std::string MainWindow::getExecutableGrandparentDirPath()
{
    const QString executableDirPath = QCoreApplication::applicationDirPath();
    QDir parentDir(executableDirPath);
    parentDir.cdUp();
    const QString grandparentDirPath = parentDir.absolutePath();
    return grandparentDirPath.toStdString();
}

void MainWindow::parseKnownWords()
{
    std::ifstream sourceFile(getExecutableGrandparentDirPath() + "/WordSource/KnownWords.txt");

    if (!sourceFile.is_open())
    {
        // "Failed to open the file."
        return;
    }

    std::string lineStr;
    while (std::getline(sourceFile, lineStr))
    {
        if (lineStr.empty()) {
            continue;
        }
        m_knownWordSet.insert(lineStr);
    }
}

void MainWindow::onSourceBtn()
{
    SourceDlg dlg(this);
    if (dlg.exec() == QDialog::Accepted)
    {
        const auto& words = dlg.getWords();
        m_currWordSet.clear();

        m_currWordCnt = words.size();
        m_currKnownCnt = 0;

        for (const auto& word : words)
        {
            if (m_knownWordSet.count(word) > 0) {
                ++m_currKnownCnt;
            } else {
                m_currWordSet.insert(word);
            }
        }
        updateMessage(m_currKnownCnt, m_currWordCnt);
    }
}

void MainWindow::onSoundBtn()
{
    m_errorMsg->setText("");
    m_textToSpeech->fetchAudio(m_lineEdit->text());
}

void MainWindow::onSkipBtn()
{

}
void MainWindow::onLaterBtn()
{

}
void MainWindow::onDeleteBtn()
{

}
void MainWindow::onKnownBtn()
{

}

void MainWindow::onErrorMsg(const QString& msg)
{
    m_errorMsg->setText(msg);
    QTimer::singleShot(3000, this, [this]() {
        m_errorMsg->setText("");
    });
}

void MainWindow::updateMessage(int known, int total)
{
    if (m_knownWordCnt != m_knownWordSet.size())
    {
        m_knownWordCnt = m_knownWordSet.size();
        const QString totalKnownMsg = QString::number(m_knownWordCnt);
        m_totalKnownMsg->setText(totalKnownMsg);
    }

    const QString sourceMsg = QString::number(known) + " / " + QString::number(total);
    m_sourceMsg->setText(sourceMsg);

    if (total == 0) {
        m_percentMsg->setText("0.0%");
    } else {
        const QString percentMsg = QString::number(static_cast<double>(known) / total * 100, 'f', 1) + "%";
        m_percentMsg->setText(percentMsg);
    }
}
