#ifndef MISSINGDLG_HPP
#define MISSINGDLG_HPP

#include "lineeditreadonly.hpp"

#include <QDialog>
#include <QString>

class QPushButton;
class PlainTextEdit;

class MissingDlg : public QDialog
{
    using baseClass = QDialog;
public:
    MissingDlg(QWidget* parent, const QString& text);
    ~MissingDlg(){}

private slots:
    void onClearAndClos();

private:
    PlainTextEdit* m_textEdit;
    QPushButton*   m_clearAndCloseBtn;
};

#endif // MISSINGDLG_HPP
