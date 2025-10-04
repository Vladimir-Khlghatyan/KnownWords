#include "exportdlg.hpp"

#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QFileDialog>
#include <QStandardPaths>

ExportDlg::ExportDlg(QWidget* parent)
    : baseClass(parent)
{
    setWindowIcon(QIcon(":/icons/export.png"));
    setWindowTitle("Export current source");
    setMinimumWidth(450);
    setFixedHeight(120);

    const QString desktop = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    m_dirPath = new QLabel(desktop);
    m_dirPath->setProperty("infoKeyLabel", true);

    m_lineEdit = new QLineEdit();
    m_lineEdit->setProperty("isFileName", true);
    m_lineEdit->setTextMargins(8, 0, 8, 0);
    m_lineEdit->setPlaceholderText("Enter file name...");

    m_browseBtn = new QPushButton();
    m_browseBtn->setProperty("iconButton", true);
    m_browseBtn->setIcon(QIcon(":/icons/source.png"));
    m_browseBtn->setFixedSize(QSize(30, 30));
    m_browseBtn->setIconSize(QSize(24, 24));
    m_browseBtn->setCursor(Qt::PointingHandCursor);
    m_browseBtn->setToolTip("Browse folder to save.");
    connect(m_browseBtn, &QPushButton::clicked, this, &ExportDlg::onBrowse);

    m_cancelBtn = new QPushButton("Cancel");
    m_cancelBtn->setCursor(Qt::PointingHandCursor);
    connect(m_cancelBtn, &QPushButton::clicked, this, &ExportDlg::onCencel);

    m_exportBtn = new QPushButton("Export");
    m_exportBtn->setCursor(Qt::PointingHandCursor);
    connect(m_exportBtn, &QPushButton::clicked, this, &ExportDlg::onExport);

    QHBoxLayout* hLayout0 = new QHBoxLayout();
    hLayout0->addWidget(m_lineEdit);
    hLayout0->addWidget(m_browseBtn);

    QHBoxLayout* hLayout1 = new QHBoxLayout();
    hLayout1->addStretch();
    hLayout1->addWidget(m_cancelBtn);
    hLayout1->addWidget(m_exportBtn);

    QVBoxLayout* vLayout = new QVBoxLayout();
    vLayout->addWidget(m_dirPath);
    vLayout->addLayout(hLayout0);
    vLayout->addLayout(hLayout1);

    setLayout(vLayout);
}

void ExportDlg::onBrowse()
{
    const QString path = getTxtFileOrDirectory(); // update this line
}

void ExportDlg::onCencel()
{
    reject();
}

void ExportDlg::onExport()
{
    // add code here
}

QString ExportDlg::getTxtFileOrDirectory()
{
    QFileDialog dialog(this, QObject::tr("Select .txt File or Directory"));
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setNameFilter(QObject::tr("Text Files (*.txt)"));
    dialog.setDirectory(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));
    dialog.setOption(QFileDialog::ShowDirsOnly, false);

    if (dialog.exec() == QDialog::Accepted)
    {
        const QStringList files = dialog.selectedFiles();
        QString path = files.isEmpty() ? QString() : files.at(0);
        QFileInfo info(path);

        return (info.isFile() && info.suffix().toLower() == "txt") ?
                      info.absoluteFilePath() : info.absolutePath();
    }

    return QString();
}
