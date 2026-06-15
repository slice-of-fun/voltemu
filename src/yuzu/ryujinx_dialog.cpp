// SPDX-FileCopyrightText: Copyright 2026 Eden Emulator Project
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ryujinx_dialog.h"

#include <filesystem>

#include "qt_common/abstract/frontend.h"
#include "qt_common/util/fs.h"
#include "ui_ryujinx_dialog.h"

RyujinxDialog::RyujinxDialog(std::filesystem::path volt_path, std::filesystem::path ryu_path,
                             QWidget* parent)
    : QDialog(parent), ui(new Ui::RyujinxDialog), m_volt(volt_path.make_preferred()),
      m_ryu(ryu_path.make_preferred())
{
    ui->setupUi(this);

    connect(ui->volt, &QPushButton::clicked, this, &RyujinxDialog::fromVolt);
    connect(ui->ryujinx, &QPushButton::clicked, this, &RyujinxDialog::fromRyujinx);
    connect(ui->cancel, &QPushButton::clicked, this, &RyujinxDialog::reject);
}

RyujinxDialog::~RyujinxDialog()
{
    delete ui;
}

void RyujinxDialog::fromVolt()
{
    accept();

    // Workaround: Ryujinx deletes and re-creates its directory structure???
    // So we just copy Volt's data to Ryujinx and then link the other way
    namespace fs = std::filesystem;
    try {
        fs::remove_all(m_ryu);
        fs::create_directories(m_ryu);
        fs::copy(m_volt, m_ryu, fs::copy_options::recursive);
    } catch (std::exception& e) {
        QtCommon::Frontend::Critical(
            tr("Failed to link save data"),
            tr("OS returned error: %1").arg(QString::fromStdString(e.what())));
    }

    // ?ploo
    QtCommon::FS::LinkRyujinx(m_ryu, m_volt);
}

void RyujinxDialog::fromRyujinx()
{
    accept();
    QtCommon::FS::LinkRyujinx(m_ryu, m_volt);
}
