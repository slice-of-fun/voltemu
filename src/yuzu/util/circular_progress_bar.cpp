// SPDX-FileCopyrightText: Copyright 2026 Eden Emulator Project
// SPDX-License-Identifier: GPL-3.0-or-later

#include "util/circular_progress_bar.h"

#include <QPaintEvent>
#include <QPainter>

CircularProgressBar::CircularProgressBar(QWidget* parent) : QWidget(parent)
{
    setMinimumSize(300, 300);
    color = QColor("#0ab9e6"); // Default fallback
}

CircularProgressBar::~CircularProgressBar() = default;

void CircularProgressBar::SetValue(int val)
{
    if (value != val) {
        value = val;
        update();
    }
}

void CircularProgressBar::SetMaximum(int max)
{
    if (maximum != max) {
        maximum = max;
        update();
    }
}

void CircularProgressBar::SetColor(const QColor& c)
{
    if (color != c) {
        color = c;
        update();
    }
}

void CircularProgressBar::SetStageText(const QString& text)
{
    if (stage_text != text) {
        stage_text = text;
        update();
    }
}

void CircularProgressBar::SetEstimateText(const QString& text)
{
    if (estimate_text != text) {
        estimate_text = text;
        update();
    }
}

void CircularProgressBar::paintEvent(QPaintEvent* event)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    int margin = 20;
    int size = qMin(width(), height()) - margin * 2;
    QRect rect((width() - size) / 2, (height() - size) / 2, size, size);

    // Draw background circle
    QPen bg_pen(QColor(40, 40, 40), 12);
    p.setPen(bg_pen);
    p.drawArc(rect, 0, 360 * 16);

    // Draw progress arc
    if (maximum > 0 && value > 0) {
        QPen prog_pen(color, 12);
        prog_pen.setCapStyle(Qt::RoundCap);
        p.setPen(prog_pen);

        int span_angle = static_cast<int>((static_cast<double>(value) / maximum) * 360.0 * 16.0);
        // Qt draws counter-clockwise for positive angles, we want clockwise so use negative angle
        p.drawArc(rect, 90 * 16, -span_angle);
    }

    // Draw text in the center
    p.setPen(Qt::white);

    QFont stage_font(QStringLiteral("Arial"), 16, QFont::Bold);
    p.setFont(stage_font);

    QRect text_rect = rect;
    // Shift slightly up for the main text
    text_rect.translate(0, -15);
    p.drawText(text_rect, Qt::AlignCenter, stage_text);

    if (!estimate_text.isEmpty()) {
        QFont estimate_font(QStringLiteral("Arial"), 12, QFont::Normal);
        p.setFont(estimate_font);

        QRect est_rect = rect;
        // Shift slightly down for the estimate text
        est_rect.translate(0, 15);

        p.setPen(QColor(180, 180, 180)); // Slightly dimmed text for estimate
        p.drawText(est_rect, Qt::AlignCenter, estimate_text);
    }
}
