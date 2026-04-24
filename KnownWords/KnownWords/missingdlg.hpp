#ifndef MISSINGDLG_HPP
#define MISSINGDLG_HPP

#include "lineeditreadonly.hpp"

#include <QDialog>
#include <QString>
#include <QString>
#include <string>

class QPushButton;
class PlainTextEdit;
class QCheckBox;

class MissingDlg : public QDialog
{
    using baseClass = QDialog;
public:
    MissingDlg(QWidget* parent, const QString& text, const std::string& lemmaBasePath);
    ~MissingDlg(){}

private slots:
    void onTextChanged();
    void onButtonClick();
    void onStateChanged(int state);

private:
    bool isValidText(const QString& text);
    void addToLemmaBase(const QString& text);

private:
    const std::string& m_lemmaBasePath;
    const QString&     m_originText;
    PlainTextEdit*     m_textEdit;
    QPushButton*       m_button;
    QCheckBox*         m_checkbox;
};

#endif // MISSINGDLG_HPP
