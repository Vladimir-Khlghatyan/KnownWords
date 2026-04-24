#include "missingdlg.hpp"

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTextEdit>
#include <QCheckBox>
#include <QLabel>
#include <QStyle>
#include <QStringList>
#include <QRegularExpression>
#include <fstream>

static void updateWidget(QWidget* w)
{
    w->style()->unpolish(w);
    w->style()->polish(w);
    w->update();
}

MissingDlg::MissingDlg(QWidget* parent, const QString& text, const std::string& lemmaBasePath)
    : baseClass(parent)
    , m_originText(text)
    , m_lemmaBasePath(lemmaBasePath)
{
    setWindowIcon(QIcon(":/icons/missing.png"));
    setWindowTitle("Missing Lemmas");
    setMinimumSize(450,400);

    m_textEdit = new PlainTextEdit();
    m_textEdit->setProperty("isValidText", true);
    m_textEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_textEdit->setText(m_originText);
    m_textEdit->setReadOnly(true);
    connect(m_textEdit, &PlainTextEdit::textChanged, this, &MissingDlg::onTextChanged);

    m_button = new QPushButton("Clear");
    m_button->setCursor(Qt::PointingHandCursor);
    m_button->setToolTip("Clear and close");
    connect(m_button, &QPushButton::clicked, this, &MissingDlg::onButtonClick);

    m_checkbox = new QCheckBox("ADD mode");
    m_checkbox->setProperty("isActiveText", false);
    connect(m_checkbox, &QCheckBox::stateChanged, this, &MissingDlg::onStateChanged);

    QHBoxLayout* hLayout = new QHBoxLayout();
    hLayout->addWidget(m_checkbox);
    hLayout->addStretch();
    hLayout->addWidget(m_button);

    QVBoxLayout* vLayout = new QVBoxLayout();
    vLayout->addWidget(m_textEdit);
    vLayout->addLayout(hLayout);

    setLayout(vLayout);
}

void MissingDlg::onTextChanged()
{
    m_textEdit->setProperty("isValidText", true);
    updateWidget(m_textEdit);
}

void MissingDlg::onButtonClick()
{
    if (!m_checkbox->isChecked()) {
        accept();
        return;
    }

    if (!isValidText(m_textEdit->toPlainText()))
    {
        m_textEdit->setProperty("isValidText", false);
        updateWidget(m_textEdit);
    }
    else
    {
        addToLemmaBase(m_textEdit->toPlainText());
        m_textEdit->setText("");
    }
}

void MissingDlg::onStateChanged(int state)
{
    m_textEdit->setProperty("isValidText", true);
    updateWidget(m_textEdit);

    if (state == Qt::Checked)
    {
        setWindowTitle("Add New Lemmas to the Database");
        m_textEdit->setText("");
        m_textEdit->setPlaceholderText("Enter text...");
        m_textEdit->setReadOnly(false);

        m_checkbox->setProperty("isActiveText", true);
        updateWidget(m_checkbox);

        m_button->setText("Add");
    }
    else
    {
        setWindowTitle("Missing Lemmas");
        m_textEdit->setText(m_originText);
        m_textEdit->setPlaceholderText("");
        m_textEdit->setReadOnly(true);

        m_checkbox->setProperty("isActiveText", false);
        updateWidget(m_checkbox);

        m_button->setText("Clear");
    }
}

bool MissingDlg::isValidText(const QString& text)
{
    static const QRegularExpression spaceRegex("\\s+"); // one or more whitespaces
    static const QRegularExpression wordRegex("^[a-z]+(?:[-'][a-z]+)*$"); //lowercase letters, '-' or '\''

    const QStringList lines = text.split('\n');
    bool hasValidLine{};

    for (const QString& rawLine : lines)
    {
        const QString line = rawLine.trimmed();

        if (line.isEmpty()) { continue; }
        hasValidLine = true;

        const QStringList words =
            line.split(spaceRegex, Qt::SkipEmptyParts);

        if (words.size() != 2) { return false; }

        for (const QString& word : words) {
            if (!wordRegex.match(word).hasMatch()) { return false; }
        }
    }

    return hasValidLine;
}

void MissingDlg::addToLemmaBase(const QString& text)
{
    if (text.isEmpty()) {
        return;
    }

    std::fstream file(m_lemmaBasePath, std::ios::in | std::ios::out | std::ios::app | std::ios::binary);

    if (!file.is_open()) {
        return; // "Failed to open the file."
    }

    file.seekg(-1, std::ios::end);
    char lastChar{};
    file.get(lastChar);

    if (lastChar != '\n')
    {
        file.clear(); // reset EOF/fail flags
        file.seekp(0, std::ios::end);
        file.put('\n');
    }

    const QStringList lines = text.split('\n');
    for (const auto& line : lines) {
        file << line.trimmed().toStdString() << '\n';
    }
}
