# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Overview

KnownWords is a Qt6/C++17 desktop app for vocabulary drilling. It shows one word at a time from a user-pasted text source, lets the user mark it Known/Later/Delete/Skip, and can optionally play an English pronunciation and show an English→Armenian gloss. It also has a lemmatization workflow to reduce inflected word forms to dictionary base forms using a local lemma database.

## Build & run

Dev workflow is CMake + Qt 6.10.1 (mingw_64 kit) + Ninja, normally driven from Qt Creator (see `KnownWords/.qtcreator/`). From the command line, from the `KnownWords/KnownWords` source directory:

```
cmake -S . -B ../../build -G Ninja -DCMAKE_PREFIX_PATH=C:/Qt/6.10.1/mingw_64
cmake --build ../../build
```

The resulting `KnownWords.exe` must be run from a `build` directory that is a **direct child of the repo root** (`c:\Users\M77235\Downloads\KnownWords\KnownWords`), because it locates its data files relative to its own path — see "Runtime data layout" below. The repo's own `build/` at the root is the expected location.

There is no automated test suite (no `add_test`/CTest targets are defined; `build/Testing/` is just an empty ctest artifact directory).

## Architecture

### Repo layout vs. runtime layout

The repo root (this directory) is **not** the CMake source directory — it's one level up from it:

- `KnownWords/` — the CMake project / C++ sources (`KnownWords/CMakeLists.txt`, `*.cpp`/`*.hpp`).
- `Settings/lemma_base.txt` — ~190k-line `word lemma` map (one pair per line), used to reduce inflected forms to base dictionary form.
- `WordSource/KnownWords.txt` — flat newline-delimited set of words already marked "known".
- `WordSource/ForLater.txt` — words deferred to learn later.
- `build/` — where `KnownWords.exe` is built and run from.

At startup, `getExecutableGrandparentDirPath()` (duplicated in `mainwindow.cpp` and `sourcedlg.cpp`) computes `applicationDirPath()` and calls `cdUp()` once, then reads/writes the three data files above relative to that grandparent directory. Any change to the build output location or install layout must preserve this one-level-up relationship.

### Data flow

1. On launch, `MainWindow` loads `KnownWords.txt` and `ForLater.txt` into `std::unordered_set<std::string>` (`m_knownWordSet`, `m_laterWordSet`).
2. "Add source" (`SourceDlg`) accepts pasted free text, tokenizes on whitespace, strips leading/trailing non-letter characters, optionally lowercases, then (`checkLemmas()`) looks each token up in `lemma_base.txt`. Tokens not found are collected into a pre-written LLM prompt (`m_missingLemmas`) that asks an external AI to lemmatize them — the app itself never calls an LLM, it just prepares text for the user to paste elsewhere.
3. Words already Known (or, in "skip" mode, already in the Later set) are filtered out; the remainder becomes the practice queue (`m_currWordVec`), from which the main window shows one random word at a time (`showRandomWord`).
4. Known / Later / Delete / Skip only mutate in-memory state during the session. Known/Later picks are buffered into `m_newKnownWordVec`/`m_newLaterWordVec` and only appended to disk by `save()`, which runs in `MainWindow::~MainWindow()` (app close) or during the Later-sync flow — data is not persisted per click.
5. "Missing lemmas" (`MissingDlg`) displays the generated prompt; pasting back `word lemma` pairs (validated against a strict regex) appends them to `Settings/lemma_base.txt` via `addToLemmaBase()`.
6. Later-sync (`MainWindow::sync()`) drops any Later word that has since become Known and rewrites `ForLater.txt`; single-clicking the "For Later" button just syncs, double-clicking also loads the remaining Later words as the new practice queue (`loadWordsFromLaterSet()`).
7. Export (`ExportDlg`, triggered from the source-count button) writes the current unlearned queue plus newly-deferred words to a user-chosen text file, 10 words per line.

### External services (public endpoints, no API keys)

- `TextToSpeech` (texttospeech.cpp) fetches an English pronunciation clip from `translate.google.com/translate_tts` and plays it via `QMediaPlayer`/`QAudioOutput`. Compiled in/out via the `_PLAY_SOUND_` macro in `define.hpp`.
- `ArmenianTranslator` (armeniantranslator.cpp) fetches an English→Armenian gloss from `api.mymemory.translated.net/get` (`langpair=en|hy`) and reads `responseData.translatedText` from the JSON response. (Previously used the unofficial `translate.googleapis.com/translate_a/single` `client=gtx` endpoint, which Google started rate-limiting/blocking with a 429 "automated queries" page.)

### UI building blocks (lineeditreadonly.hpp)

- `LineEditReadOnly` — a QLineEdit that's read-only until double-clicked; used for the current word.
- `PlainTextEdit` — a QTextEdit that strips formatting on paste (plain text only), used for the source/missing-lemma text areas.
- `MyButton` — a QPushButton that distinguishes single- vs. double-click (250ms timer, `singleClicked`/`doubleClicked` signals); used for the info-row buttons ("Current Source" / "For Later") which have distinct single- vs. double-click behavior.

Styling is centralized as one QSS string (`MAIN_WINDOW_STYLE` in `define.hpp`) applied to the QMainWindow; widgets opt into style variants via dynamic Qt properties (e.g. `infoKeyLabel`, `toggleButton`, `isValidText`, `error`) rather than per-widget stylesheets.

Rules:
1. Do not run any Git-related commands under any circumstances.
   - This includes: git status, git diff, git pull, git fetch, git checkout, git branch, git merge, git commit, git add, git reset, and any other Git operation.

   2. You may only read files inside the `KnownWords` folder.
   - Do not inspect or rely on files outside `KnownWords` unless explicitly required to answer the current question.
   - Prefer the smallest possible file set needed to solve the problem.

Behavior:
- Be cautious, minimal, and read-only.
- Prefer concise answers unless more detail is necessary.
- If information is missing, ask for a specific file or snippet rather than reading broadly.
- Clearly distinguish between what you know, what you infer, and what you need next.
