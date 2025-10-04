#include "mainwindow.hpp"

#include "sourcedlg.hpp"
#include "lineeditreadonly.hpp"
#include "texttospeech.hpp"
#include "armeniantranslator.hpp"

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
#include <random>
#include <algorithm>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_infoLayout(new QVBoxLayout())
    , m_knownWordCnt(0)
    , m_laterWordCnt(0)
    , m_currWordCnt(0)
    , m_currKnownCnt(0)
    , m_currIndex(-1)

{
    setWindowTitle("Known Words");
    setWindowIcon(QIcon(":/icons/logo.png"));
    setStyleSheet(MAIN_WINDOW_STYLE);
    setFixedSize(QSize(400, 300));

    m_knownFilePath = getExecutableGrandparentDirPath() + "/WordSource/KnownWords.txt";
    m_laterFilePath = getExecutableGrandparentDirPath() + "/WordSource/ForLater.txt";

    m_knownWordSet = parseWordSource(m_knownFilePath);
    m_laterWordSet = parseWordSource(m_laterFilePath);

    m_totalKnownMsg = addMessage("Total Known Words:").first;
    m_sourceMsgBtn = addMessage("Current Source:", true).second;
    m_percentMsg = addMessage("Known Percentage:").first;
    m_forLaterBtn = addMessage("For Later:", true).second;

    m_sourceMsgBtn->setToolTip("Known / Total");
    m_percentMsg->setToolTip("Current known / Total known * 100%");
    m_forLaterBtn->setToolTip(QString("Words to learn later.%1%2")
                                .arg("\n - Click to sync with knows words.",
                                     "\n - Double click to sync and load words to learn."));
    connect(m_forLaterBtn, &MyButton::singleClicked, this, &MainWindow::onLaterBtnSingleClick);
    connect(m_forLaterBtn, &MyButton::doubleClicked, this, &MainWindow::onLaterBtnDoubleClick);

    updateMessage();

    m_lineEdit = new LineEditReadOnly();
    m_lineEdit->setToolTip("Double click to edit.");
    connect(m_lineEdit, &QLineEdit::editingFinished, this, &MainWindow::onEditingFinished);

    m_translatedText = new QLabel();
    m_translatedText->setProperty("translate", true);
    m_translatedText->setFixedSize(4 * 80 + 3 * 4, 36);
    m_translatedText->setAlignment(Qt::AlignHCenter);

    showRandomWord();

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

    m_translateBtn = getButtonWithIcon("translate", "Translate");
    connect(m_translateBtn, &QPushButton::clicked, this, &MainWindow::onTranslateBtn);
    m_translator = new ArmenianTranslator(this);
    connect(m_translator, &ArmenianTranslator::errorOccurred, this, &MainWindow::onErrorMsg);
    connect(m_translator, &ArmenianTranslator::translationReady, this, &MainWindow::onTranslationReady);

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

    m_errorMsg = new QLabel();
    m_errorMsg->setProperty("error", true);

    // setup Main Window layout
    QWidget* centralWidget = new QWidget();
    setCentralWidget(centralWidget);
    QGridLayout* layout = new QGridLayout(centralWidget);
    layout->setContentsMargins(8,8,8,8);
    layout->setSpacing(4);

    QWidget* spacing = new QWidget();
    spacing->setFixedHeight(8);

    layout->addLayout(m_infoLayout,     0, 0, 3, 1, Qt::AlignLeft | Qt::AlignTop);
    layout->addWidget(m_lineEdit,       3, 0, 1, 2, Qt::AlignVCenter);
    layout->addWidget(m_sourceBtn,      0, 1, 1, 1, Qt::AlignRight);
    layout->addWidget(m_soundBtn,       1, 1, 1, 1, Qt::AlignRight);
    layout->addWidget(m_translateBtn,   2, 1, 1, 1, Qt::AlignRight);
    layout->addLayout(btnLayout,        4, 0, 1, 2);
    layout->addWidget(spacing,          5, 0, 1, 2);
    layout->addWidget(m_translatedText, 6, 0, 1, 2, Qt::AlignHCenter);
    layout->addWidget(m_errorMsg,       7, 0, 1, 1, Qt::AlignLeft);
}

MainWindow::~MainWindow()
{
    save(m_newKnownWordVec, m_knownFilePath);
    save(m_newLaterWordVec, m_laterFilePath);
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

MainWindow::InfoPair MainWindow::addMessage(const char* keyMessage, bool isButton)
{
    QHBoxLayout* hLayout = new QHBoxLayout();
    hLayout->setContentsMargins(0,0,0,0);
    hLayout->setSpacing(8);

    QLabel* keyLabel = new QLabel(keyMessage);
    keyLabel->setProperty("infoKeyLabel", true);
    hLayout->addWidget(keyLabel);

    QLabel* valLabel{};
    MyButton* valBtn{};

    if (isButton)
    {
        valBtn = new MyButton();
        valBtn->setProperty("infoValButton", true);
        valBtn->setCursor(Qt::PointingHandCursor);
        hLayout->addWidget(valBtn, Qt::AlignLeft);
        hLayout->addStretch();
    }
    else
    {
        valLabel = new QLabel();
        valLabel->setProperty("infoValLabel", true);
        hLayout->addWidget(valLabel, Qt::AlignLeft);
    }

    m_infoLayout->addLayout(hLayout);
    return {valLabel, valBtn};
}

std::string MainWindow::getExecutableGrandparentDirPath()
{
    const QString executableDirPath = QCoreApplication::applicationDirPath();
    QDir parentDir(executableDirPath);
    parentDir.cdUp();
    const QString grandparentDirPath = parentDir.absolutePath();
    return grandparentDirPath.toStdString();
}

MainWindow::WordSet MainWindow::parseWordSource(const std::string& inFile)
{
    std::ifstream file(inFile);

    if (!file)
    {
        // "Failed to open the file."
        return {};
    }

    MainWindow::WordSet ws;
    std::string lineStr;
    while (std::getline(file, lineStr)) {
        if (!lineStr.empty()) {
            ws.insert(std::move(lineStr));
        }
    }

    return ws;
}

void MainWindow::onSourceBtn()
{
    SourceDlg dlg(this);
    if (dlg.exec() == QDialog::Accepted)
    {
        const auto& words = dlg.getWords();
        const bool isSkipMode = dlg.isLaterSkipMode();
        int skipCnt{};
        m_currWordVec.clear();

        if (isSkipMode)
        {
            for (const auto& word : words)
            {
                if (m_laterWordSet.count(word) > 0) {
                    ++skipCnt;
                } else if (m_knownWordSet.count(word) == 0) {
                    m_currWordVec.push_back(word);
                }
            }
        }
        else
        {
            for (const auto& word : words) {
                if (m_knownWordSet.count(word) == 0) {
                    m_currWordVec.push_back(word);
                }
            }
        }

        m_currWordCnt = words.size();
        m_currKnownCnt = words.size() - m_currWordVec.size() - skipCnt;

        updateMessage(m_currKnownCnt, m_currWordCnt);
        showRandomWord();
    }
}

void MainWindow::onSoundBtn()
{
    m_errorMsg->setText("");
    m_textToSpeech->fetchAudio(m_lineEdit->text());
}

void MainWindow::onTranslateBtn()
{
    m_errorMsg->setText("");
    m_translator->translateToArmenian(m_lineEdit->text());
}

void MainWindow::onSkipBtn()
{
    if (m_currIndex ==-1 || m_currWordVec.size() < 2) {
        onErrorMsg("No word to skip.");
        return;
    }

    const int prevIndex = m_currIndex;
    m_currIndex = getRandomNumber(0, m_currWordVec.size() - 1);
    if (m_currIndex == prevIndex) {
        m_currIndex += (prevIndex == 0) ? 1 : -1;
    }

    showRandomWord(m_currIndex);
}
void MainWindow::onLaterBtn()
{
    if (m_currIndex == -1) {
        onErrorMsg("No word to laern later.");
        return;
    }

    const auto success = m_laterWordSet.insert(m_currWordVec[m_currIndex]);
    if (success.second == true) {
        m_newLaterWordVec.push_back(m_currWordVec[m_currIndex]);
    }

    onDeleteBtn();
}
void MainWindow::onDeleteBtn()
{
    if (m_currIndex ==-1 || m_currWordVec.empty()) {
        onErrorMsg("No word to delete.");
        return;
    }

    std::swap(m_currWordVec[m_currIndex], m_currWordVec.back());
    m_currWordVec.pop_back();
    updateMessage(m_currKnownCnt, --m_currWordCnt);
    showRandomWord();
}
void MainWindow::onKnownBtn()
{
    if (m_currIndex == -1) {
        onErrorMsg("No word to add in the known words' set.");
        return;
    }

    const auto success = m_knownWordSet.insert(m_currWordVec[m_currIndex]);
    if (success.second == true)
    {
        m_newKnownWordVec.push_back(m_currWordVec[m_currIndex]);

        std::swap(m_currWordVec[m_currIndex], m_currWordVec.back());
        m_currWordVec.pop_back();
        updateMessage(++m_currKnownCnt, m_currWordCnt);
        showRandomWord();
    }
}

void MainWindow::onLaterBtnSingleClick()
{
    sync();
    updateMessage(m_currKnownCnt, m_currWordCnt);
}

void MainWindow::onLaterBtnDoubleClick()
{
    sync();
    save(m_newKnownWordVec, m_knownFilePath);
    loadWordsFromLaterSet();
    updateMessage(m_currKnownCnt, m_currWordCnt);
    showRandomWord();
}

void MainWindow::onErrorMsg(const QString& msg)
{
    m_errorMsg->setText(msg);
    QTimer::singleShot(3000, this, [this]() {
        m_errorMsg->setText("");
    });
}

void MainWindow::onTranslationReady(const QString& translatedText)
{
    m_translatedText->setText(translatedText);
}

void MainWindow::onEditingFinished()
{
    if (m_currIndex == -1) {
        return;
    }

    m_currWordVec[m_currIndex] = m_lineEdit->text().toStdString();
    m_translatedText->setText("");
}

void MainWindow::updateMessage(int known, int total)
{
    if (m_knownWordCnt != m_knownWordSet.size())
    {
        m_knownWordCnt = m_knownWordSet.size();
        m_totalKnownMsg->setText(QString::number(m_knownWordCnt));
    }

    const QString sourceMsg = QString::number(known) + " / " + QString::number(total);
    m_sourceMsgBtn->setButtonText(sourceMsg);

    if (total == 0) {
        m_percentMsg->setText("0.0%");
    } else {
        const QString percentMsg = QString::number(static_cast<double>(known) / total * 100, 'f', 1) + "%";
        m_percentMsg->setText(percentMsg);
    }

    if (m_laterWordCnt != m_laterWordSet.size() || m_laterWordCnt == 0)
    {
        m_laterWordCnt = m_laterWordSet.size();
        m_forLaterBtn->setButtonText(QString::number(m_laterWordCnt));
    }
}

int MainWindow::getRandomNumber(int lower, int upper)
{
    std::random_device rd;
    std::mt19937 eng(rd());

    std::uniform_int_distribution<> distribution(lower, upper);
    const int randomNumber = distribution(eng);

    return randomNumber;
}

void MainWindow::showRandomWord()
{
    if (m_currWordVec.empty()) {
        m_currIndex = -1;
        m_lineEdit->setText("...");
    } else {
        m_currIndex = getRandomNumber(0, m_currWordVec.size() - 1);
        m_lineEdit->setText(m_currWordVec[m_currIndex].c_str());
    }
    m_translatedText->setText("");
}

void MainWindow::showRandomWord(int index)
{
    m_lineEdit->setText(m_currWordVec[index].c_str());
    m_translatedText->setText("");
}

void MainWindow::save(WordVec& src, const std::string& outFile)
{
    std::fstream file(outFile, std::ios::in | std::ios::out | std::ios::app | std::ios::binary);

    if (!file.is_open())
    {
        // "Failed to open the file."
        return;
    }

    file.seekg(-1, std::ios::end);
    char lastChar;
    file.get(lastChar);

    if (lastChar != '\n')
    {
        file.clear(); // reset EOF/fail flags
        file.seekp(0, std::ios::end);
        file.put('\n');
    }

    for (const auto& word : src) {
        file << word << '\n';
    }

    src.clear();
}

void MainWindow::sync()
{
    const size_t oldSize = m_laterWordSet.size();
    for (auto it = m_laterWordSet.begin(); it != m_laterWordSet.end(); ) {
        it = (m_knownWordSet.count(*it) > 0) ? m_laterWordSet.erase(it) : std::next(it);
    }

    if (oldSize != m_laterWordSet.size())
    {
        m_newLaterWordVec.clear();
        std::ofstream file(m_laterFilePath, std::ios::trunc | std::ios::binary);

        for (const std::string& word : m_laterWordSet) {
            file << word << '\n';
        }
    }
}

void MainWindow::loadWordsFromLaterSet()
{
    m_currWordVec = WordVec(m_laterWordSet.begin(), m_laterWordSet.end());
    m_currWordCnt = m_currWordVec.size();
    m_currKnownCnt = 0;
}
