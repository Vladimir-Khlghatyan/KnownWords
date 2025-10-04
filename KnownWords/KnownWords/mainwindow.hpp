#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "define.hpp"
#include <QMainWindow>
#include <string>
#include <unordered_set>
#include <vector>

class QPushButton;
class QGridLayout;
class QVBoxLayout;
class QLabel;
class QLineEdit;
class LineEditReadOnly;
class TextToSpeech;
class ArmenianTranslator;
class MyButton;

class MainWindow : public QMainWindow
{
    Q_OBJECT

    using InfoPair = std::pair<QLabel*, MyButton*>;
    using WordSet = std::unordered_set<std::string>;
    using WordVec = std::vector<std::string>;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QPushButton* getButtonWithIcon( const QString& iconName,
                                    const QString& tooltip = "",
                                    bool isEnabled = true,
                                    const QSize& buttonSize = QSize(30, 30),
                                    const QSize& iconSize = QSize(24, 24));

    InfoPair addMessage(const char* keyMessage, bool isButton = false);
    std::string getExecutableGrandparentDirPath();
    WordSet parseWordSource(const std::string& inFile);
    void updateMessage(int known = 0, int total = 0);
    int  getRandomNumber(int lower, int upper);
    void showRandomWord();
    void showRandomWord(int index);
    void save(WordVec& src, const std::string& outFile);
    void sync();
    void loadWordsFromLaterSet();

private slots:
    void onSourceBtn();
    void onSoundBtn();
    void onTranslateBtn();
    void onSkipBtn();
    void onLaterBtn();
    void onDeleteBtn();
    void onKnownBtn();
    void onSourceMsgBtnSingleClick();
    void onLaterBtnSingleClick();
    void onLaterBtnDoubleClick();
    void onErrorMsg(const QString& msg);
    void onTranslationReady(const QString& translatedText);
    void onEditingFinished();

private:
    std::string m_knownFilePath;
    std::string m_laterFilePath;

    QPushButton* m_sourceBtn;
    QPushButton* m_soundBtn;
    QPushButton* m_translateBtn;
    QPushButton* m_skipBtn;
    QPushButton* m_laterBtn;
    QPushButton* m_deleteBtn;
    QPushButton* m_knownBtn;
    MyButton*    m_sourceMsgBtn;
    MyButton*    m_forLaterBtn;
    QVBoxLayout* m_infoLayout;
    QLabel*      m_totalKnownMsg;
    QLabel*      m_percentMsg;
    QLabel*      m_translatedText;
    QLabel*      m_errorMsg;

    LineEditReadOnly* m_lineEdit;

    WordSet m_knownWordSet;
    WordSet m_laterWordSet;
    WordVec m_currWordVec;
    WordVec m_newKnownWordVec;
    WordVec m_newLaterWordVec;

    int m_knownWordCnt;
    int m_laterWordCnt;
    int m_currWordCnt;
    int m_currKnownCnt;
    int m_currIndex;

#ifdef _PLAY_SOUND_
    TextToSpeech* m_textToSpeech;
#endif // _PLAY_SOUND_

    ArmenianTranslator* m_translator;
};
#endif // MAINWINDOW_HPP
