#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <string>
#include <unordered_set>

class QPushButton;
class QGridLayout;
class QVBoxLayout;
class QLabel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QPushButton* getButtonWithIcon( const QString& iconName,
                                    const QString& tooltip = "",
                                    bool isEnabled = true,
                                    const QSize& buttonSize = QSize(30, 30),
                                    const QSize& iconSize = QSize(24, 24));

    QLabel* addMessage(const char* keyMessage);
    std::string getExecutableGrandparentDirPath();
    void parseKnownWords();
    void updateMessage(int known, int total);

private slots:
    void OnSourceBtn();

private:
    QVBoxLayout* m_infoLayout;
    QPushButton* m_sourceBtn;
    QLabel*      m_totalKnownMsg;
    QLabel*      m_sourceMsg;
    QLabel*      m_percentMsg;

    std::unordered_set<std::string> m_knownWordSet;
    std::unordered_set<std::string> m_currWordSet;
};
#endif // MAINWINDOW_HPP
