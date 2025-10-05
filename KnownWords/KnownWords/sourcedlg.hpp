#ifndef SOURCEDLG_HPP
#define SOURCEDLG_HPP

#include "lineeditreadonly.hpp"

#include <QDialog>
#include <QString>

#include <unordered_set>
#include <string>

class QPushButton;
class QLabel;
class PlainTextEdit;

class SourceDlg : public QDialog
{
    using baseClass = QDialog;
public:
    SourceDlg(QWidget* parent);
    ~SourceDlg(){}

    const std::unordered_set<std::string>& getWords() const { return m_wordSet; }
    bool isLaterSkipMode() const;

private slots:
    void onCase();
    void onLater();
    void onCancel();
    void onOk();

private:
    PlainTextEdit* m_textEdit;
    QLabel*        m_caseText;
    QLabel*        m_laterText;
    QPushButton*   m_caseBtn;
    QPushButton*   m_laterBtn;
    QPushButton*   m_cancelBtn;
    QPushButton*   m_okBtn;
    bool           m_isCaseInsensitive;
    bool           m_laterSkipMode;
    std::unordered_set<std::string> m_wordSet;
};

#endif // SOURCEDLG_HPP
