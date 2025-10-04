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
    void onCencel();
    void onSave();

private:
    QLineEdit*   m_lineEdit;
    QLabel*      m_dirPath;
    QPushButton* m_cancelBtn;
    QPushButton* m_saveBtn;
};

#endif // EXPORTDLG_HPP
