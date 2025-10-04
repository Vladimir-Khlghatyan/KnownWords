#ifndef DEFINE_HPP
#define DEFINE_HPP

#define _PLAY_SOUND_
//#undef _PLAY_SOUND_

#include <QString>

static const QString MAIN_WINDOW_STYLE(R"(
QMainWindow
{
    background: #272727;
}
QDialog
{
    background: #272727;
}
QLabel
{
    font-size:   16px;
    color:       white;
}
QLabel[infoKeyLabel="true"]
{
    font-style: italic;
    color:      #BFBFBF;
}
QLabel[infoValLabel="true"]
{
    font-weight: bold;
    color:       #BFBFBF;
}
QLabel[dialog="true"]
{
    font-size:   14px;
    font-weight: normal;
}
QLabel[error="true"]
{
    font-style: italic;
    color:      #DD7D7D;
}
QLabel[translate="true"]
{
    border:         1px solid #5E5E5E;
    border-radius:  5px;
    font-size:      20px;
    color:          #BFBFBF;
}
QPushButton
{
    background-color: #52606E;
    color:            white;
    font-size:        16px;
    border-radius:    10px;
    min-width:        80px;
    max-width:        80px;
    min-height:       30px;
    max-height:       30px;
}
QPushButton:hover
{
    background-color: #708294;
}
QPushButton[iconButton="true"]
{
    min-width:        30px;
    max-width:        30px;
}
QPushButton[toggleButton="true"]
{
    background-color: #272727;
    min-width:        30px;
    max-width:        30px;
    min-height:       20px;
    max-height:       20px;
}
QPushButton[infoValButton="true"]
{
    background-color: #272727;
    color:            #BFBFBF;
    font-weight:      bold;
}
QPushButton:hover[infoValButton="true"]
{
    text-decoration: underline;
}
QToolTip
{
    border:     1px solid #9B9C9C;
    background: #2e2f30;
    color:      #9B9C9C;
    font-size:  14px;
}
QTextEdit
{
    border:         1px solid #9B9C9C;
    border-radius:  5px;
    background:     #2e2f30;
    color:          white;
}
QLineEdit
{
    border:     none;
    background: #272727;
    color:      white;
    font-size:  32px;
}
)");

// https://fonts.google.com/icons

#endif // DEFINE_HPP
