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

private slots:
    void onBrowse();
    void onCencel();
    void onExport();

private:
    QString getTxtFileOrDirectory();

private:
    QLineEdit*   m_lineEdit;
    QLabel*      m_dirPath;
    QPushButton* m_browseBtn;
    QPushButton* m_cancelBtn;
    QPushButton* m_exportBtn;
};

#endif // EXPORTDLG_HPP
