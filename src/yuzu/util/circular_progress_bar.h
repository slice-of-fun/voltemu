// SPDX-FileCopyrightText: Copyright 2026 Eden Emulator Project
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QColor>
#include <QString>
#include <QWidget>

class CircularProgressBar : public QWidget {
    Q_OBJECT
public:
    explicit CircularProgressBar(QWidget* parent = nullptr);
    ~CircularProgressBar() override;

    void SetValue(int val);
    void SetMaximum(int max);
    void SetColor(const QColor& c);
    void SetStageText(const QString& text);
    void SetEstimateText(const QString& text);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    int value = 0;
    int maximum = 100;
    QColor color;
    QString stage_text;
    QString estimate_text;
};
