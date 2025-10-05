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

    m_dirPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    m_fileName = "words.txt";

    m_pathLabel = new QLabel(m_dirPath);
    m_pathLabel->setProperty("infoKeyLabel", true);

    m_lineEdit = new QLineEdit();
    m_lineEdit->setProperty("isFileName", true);
    m_lineEdit->setTextMargins(8, 0, 8, 0);
    m_lineEdit->setPlaceholderText("Enter file name...");
    m_lineEdit->setText(m_fileName);
    m_lineEdit->setFocus();
    m_lineEdit->setSelection(0, m_fileName.size() - 4);
    connect(m_lineEdit, &QLineEdit::textChanged, this, &ExportDlg::onTextChanged);

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
    connect(m_cancelBtn, &QPushButton::clicked, this, &ExportDlg::onCancel);

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
    vLayout->addWidget(m_pathLabel);
    vLayout->addLayout(hLayout0);
    vLayout->addLayout(hLayout1);

    setLayout(vLayout);
}

void ExportDlg::onBrowse()
{
    const QString path = getTxtFileOrDirectory(); // update this line
    if (!path.isEmpty())
    {
        m_dirPath = std::move(path);
        m_pathLabel->setText(m_dirPath);
    }
}

void ExportDlg::onCancel()
{
    reject();
}

void ExportDlg::onExport()
{
    accept();
}

void ExportDlg::onTextChanged(const QString& text)
{
    m_fileName = text;
}

QString ExportDlg::getTxtFileOrDirectory()
{
    QFileDialog dialog(this, tr("Select Directory"));
    dialog.setFileMode(QFileDialog::Directory);
    dialog.setOption(QFileDialog::ShowDirsOnly, true);
    dialog.setDirectory(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));

    if (dialog.exec() == QDialog::Accepted)
    {
        const QStringList selected = dialog.selectedFiles();
        if (!selected.isEmpty()) {
            return selected.first();
        }
    }

    return QString();
}

QString ExportDlg::getFilePath()
{
    if (!m_fileName.endsWith(".txt")) {
        m_fileName += ".txt";
    }
    return m_dirPath + "/" + m_fileName;
}
