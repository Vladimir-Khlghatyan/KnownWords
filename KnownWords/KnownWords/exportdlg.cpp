#include "exportdlg.hpp"

#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>

ExportDlg::ExportDlg(QWidget* parent)
    : baseClass(parent)
{
    setWindowIcon(QIcon(":/icons/source.png"));
    setWindowTitle("Export current source");
    setMinimumSize(450,100);

    m_dirPath = new QLabel("/path/to/directory");
    //    m_path->setProperty("infoKey", true);

    m_lineEdit = new QLineEdit();
    m_lineEdit->setPlaceholderText("Enter file name...");

    m_cancelBtn = new QPushButton("Cancel");
    m_cancelBtn->setCursor(Qt::PointingHandCursor);
    connect(m_cancelBtn, &QPushButton::clicked, this, &ExportDlg::onCencel);

    m_saveBtn = new QPushButton("Save");
    m_saveBtn->setCursor(Qt::PointingHandCursor);
    connect(m_saveBtn, &QPushButton::clicked, this, &ExportDlg::onSave);

    QHBoxLayout* hLayout = new QHBoxLayout();
    hLayout->addStretch();
    hLayout->addWidget(m_cancelBtn);
    hLayout->addWidget(m_saveBtn);

    QVBoxLayout* vLayout = new QVBoxLayout();
    vLayout->addWidget(m_dirPath);
    vLayout->addWidget(m_lineEdit);
    vLayout->addLayout(hLayout);

    setLayout(vLayout);
}

void ExportDlg::onCencel()
{
    reject();
}

void ExportDlg::onSave()
{

}
