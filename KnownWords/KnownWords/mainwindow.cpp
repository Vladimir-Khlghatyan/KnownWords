#include "mainwindow.hpp"

#include "define.hpp"
#include "sourcedlg.hpp"

#include <QCoreApplication>
#include <QDir>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QPushButton>
#include <QLabel>

#include <fstream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_infoLayout(nullptr)
    , m_sourceBtn(nullptr)
{
    setWindowTitle("Known Words");
    setWindowIcon(QIcon(":/icons/logo.png"));
    setStyleSheet(MAIN_WINDOW_STYLE);
    setFixedSize(QSize(600, 350));

    // setup Main Window layout
    QWidget* centralWidget = new QWidget();
    setCentralWidget(centralWidget);
    QGridLayout* layout = new QGridLayout(centralWidget);
    layout->setContentsMargins(8,8,8,8);
    layout->setSpacing(4);

    parseKnownWords();

    m_infoLayout = new QVBoxLayout();
    m_totalKnownMsg = addMessage("Total Known Words:");
    m_totalKnownMsg->setText(QString::number(m_knownWordSet.size()));

    m_sourceMsg = addMessage("Current Source:");
    m_sourceMsg->setText("0 / 0");
    m_sourceMsg->setToolTip("known / total");

    m_percentMsg = addMessage("Known Percentage:");
    m_percentMsg->setText("0.0%");

    m_sourceBtn = getButtonWithIcon("source", "Add new source");
    connect(m_sourceBtn, &QPushButton::clicked, this, &MainWindow::OnSourceBtn);

    layout->addLayout(m_infoLayout, 0, 0, Qt::AlignLeft | Qt::AlignTop);
    layout->addWidget(m_sourceBtn, 1, 0, 1, 1, Qt::AlignLeft);

}

MainWindow::~MainWindow()
{
}

QPushButton* MainWindow::getButtonWithIcon(const QString& iconName, const QString& tooltip,
                                     bool isEnabled, const QSize& buttonSize, const QSize& iconSize)
{
    QPushButton* btn = new QPushButton(/*this*/);
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

void MainWindow::OnSourceBtn()
{
    SourceDlg dlg(this);
    if (dlg.exec() == QDialog::Accepted)
    {
        const auto& words = dlg.getWords();
        m_currWordSet.clear();

        int known{};
        for (const auto& word : words)
        {
            if (m_knownWordSet.count(word) > 0) {
                ++known;
            } else {
                m_currWordSet.insert(word);
            }
        }

        updateMessage(known, words.size());

    }
}

void MainWindow::updateMessage(int known, int total)
{
    const QString sourceMsg = QString::number(known) + " / " + QString::number(total);
    const QString percentMsg = QString::number(static_cast<double>(known) / total * 100, 'f', 1) + "%";

    m_sourceMsg->setText(sourceMsg);
    m_percentMsg->setText(percentMsg);
}
