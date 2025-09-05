#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "define.hpp"
#include <QMainWindow>
#include <string>
#include <unordered_set>

class QPushButton;
class QGridLayout;
class QVBoxLayout;
class QLabel;
class QLineEdit;
class LineEditReadOnly;
class TextToSpeech;

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
    void updateMessage(int known = 0, int total = 0);

private slots:
    void onSourceBtn();
    void onSoundBtn();
    void onSkipBtn();
    void onLaterBtn();
    void onDeleteBtn();
    void onKnownBtn();
    void onErrorMsg(const QString& msg);

private:
    QPushButton* m_sourceBtn;
    QPushButton* m_soundBtn;
    QPushButton* m_skipBtn;
    QPushButton* m_laterBtn;
    QPushButton* m_deleteBtn;
    QPushButton* m_knownBtn;
    QVBoxLayout* m_infoLayout;
    QLabel*      m_totalKnownMsg;
    QLabel*      m_sourceMsg;
    QLabel*      m_percentMsg;
    QLabel*      m_translateMsg;
    QLabel*      m_errorMsg;

    LineEditReadOnly*   m_lineEdit;

    std::unordered_set<std::string> m_knownWordSet;
    std::unordered_set<std::string> m_currWordSet;

    int m_knownWordCnt;
    int m_currWordCnt;
    int m_currKnownCnt;

#ifdef _PLAY_SOUND_
    TextToSpeech* m_textToSpeech;
#endif // _PLAY_SOUND_
};
#endif // MAINWINDOW_HPP
