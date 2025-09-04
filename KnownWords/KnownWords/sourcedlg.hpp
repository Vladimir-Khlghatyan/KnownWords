#ifndef SOURCEDLG_HPP
#define SOURCEDLG_HPP

#include <QDialog>
#include <QString>

#include <unordered_set>
#include <string>

class QPushButton;
class QLabel;
class QTextEdit;

class SourceDlg : public QDialog
{
    using baseClass = QDialog;
public:
    SourceDlg(QWidget* parent);
    ~SourceDlg(){}

    const std::unordered_set<std::string>& getWords() const { return m_wordSet; }

private slots:
    void onCencel();
    void onOk();

private:
    QTextEdit*   m_textEdit;
    QPushButton* m_cancelBtn;
    QPushButton* m_okBtn;
    std::unordered_set<std::string> m_wordSet;
};

#endif // SOURCEDLG_HPP
