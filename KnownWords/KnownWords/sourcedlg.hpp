#ifndef SOURCEDLG_HPP
#define SOURCEDLG_HPP

#include "lineeditreadonly.hpp"

#include <QDialog>
#include <QString>

#include <unordered_set>
#include <unordered_map>
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
    const QString& getLemmaMsg() const { return m_lemmaMsg; }
    const QString& getMissingLemmas() const { return m_missingLemmas; }

private:
    std::string getExecutableGrandparentDirPath();
    void setupLemmas();
    void checkLemmas();

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
    QString        m_lemmaMsg;
    QString        m_missingLemmas;
    bool           m_isCaseInsensitive;
    bool           m_laterSkipMode;
    std::unordered_set<std::string> m_wordSet;
    std::unordered_map<std::string, std::string> m_lemmaMap;
};

#endif // SOURCEDLG_HPP
