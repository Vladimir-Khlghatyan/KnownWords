#ifndef EXPORTDLG_HPP
#define EXPORTDLG_HPP

#include "qlabel.h"
#include <QDialog>

class QLineEdit;
class QLabel;
class QPushButton;

class ExportDlg : public QDialog
{
    using baseClass = QDialog;
public:
    ExportDlg(QWidget* parent);
    ~ExportDlg(){}

    QString getFilePath();

private slots:
    void onBrowse();
    void onCancel();
    void onExport();
    void onTextChanged(const QString&);

private:
    QString getTxtFileOrDirectory();

private:
    QLineEdit*   m_lineEdit;
    QLabel*      m_pathLabel;
    QPushButton* m_browseBtn;
    QPushButton* m_cancelBtn;
    QPushButton* m_exportBtn;
    QString      m_dirPath;
    QString      m_fileName;
};

#endif // EXPORTDLG_HPP
