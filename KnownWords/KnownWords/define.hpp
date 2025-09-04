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
QLabel[infoKey="true"]
{
    font-style: italic;
}
QLabel[infoVal="true"]
{
    font-weight: bold;
}
QLabel[dialog="true"]
{
    font-size:   14px;
    font-weight: normal;
}
QPushButton
{
    background-color: #52606E;
    color:            white;
    font-size:        20px;
    font-weight:      bold;
    border-radius:    10px;
}
QPushButton:hover
{
    background-color: #708294;
    color:            white;
    font-size:        20px;
    font-weight:      bold;
}
QPushButton:disabled
{
    color: gray;
}
QToolTip
{
    border:     1px solid #9B9C9C;
    background: #2e2f30;
    color:      #9B9C9C;
    font-size:  14px;
}
QPushButton[dialog="true"]
{
    background-color: #52606E;
    color:            white;
    font-size:        16px;
    font-weight:      normal;
    border-radius:    10px;
    min-width:        80px;
    max-width:        80px;
    min-height:       30px;
    max-height:       30px;
}
QPushButton:hover[dialog="true"]
{
    background-color: #708294;
    color:            white;
    font-size:        16px;
    font-weight:      normal;
}
QTextEdit
{
    border:     1px solid #9B9C9C;
    background: #2e2f30;
    color:      white;
}
)");

// https://fonts.google.com/icons

#endif // DEFINE_HPP
