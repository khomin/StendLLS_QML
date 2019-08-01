#include "clipboard.h"

Clipboard::Clipboard(QClipboard* c) : clipboard(c) {
    QObject::connect(c, &QClipboard::dataChanged, this, &Clipboard::textChanged);
}

void Clipboard::text(QString text) {
    clipboard->setText(text);
}
