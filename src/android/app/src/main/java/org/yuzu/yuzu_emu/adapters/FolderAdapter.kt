// SPDX-FileCopyrightText: Copyright 2026 Eden Emulator Project
// SPDX-License-Identifier: GPL-3.0-or-later

// SPDX-FileCopyrightText: 2023 yuzu Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

package dev.volt_emu.volt.adapters

import android.net.Uri
import android.view.LayoutInflater
import android.view.ViewGroup
import androidx.fragment.app.FragmentActivity
import dev.volt_emu.volt.R
import dev.volt_emu.volt.databinding.CardFolderBinding
import dev.volt_emu.volt.fragments.GameFolderPropertiesDialogFragment
import dev.volt_emu.volt.model.DirectoryType
import dev.volt_emu.volt.model.GameDir
import dev.volt_emu.volt.model.GamesViewModel
import dev.volt_emu.volt.utils.ViewUtils.marquee
import dev.volt_emu.volt.viewholder.AbstractViewHolder

class FolderAdapter(val activity: FragmentActivity, val gamesViewModel: GamesViewModel) :
    AbstractDiffAdapter<GameDir, FolderAdapter.FolderViewHolder>() {
    override fun onCreateViewHolder(
        parent: ViewGroup,
        viewType: Int
    ): FolderAdapter.FolderViewHolder {
        CardFolderBinding.inflate(LayoutInflater.from(parent.context), parent, false)
            .also { return FolderViewHolder(it) }
    }

    inner class FolderViewHolder(val binding: CardFolderBinding) :
        AbstractViewHolder<GameDir>(binding) {
        override fun bind(model: GameDir) {
            binding.apply {
                path.text = Uri.parse(model.uriString).path
                path.marquee()

                // Set type indicator, shows below folder name, to see if DLC or Games
                typeIndicator.text = when (model.type) {
                    DirectoryType.GAME -> activity.getString(R.string.games)
                    DirectoryType.EXTERNAL_CONTENT -> activity.getString(R.string.external_content)
                }

                buttonEdit.setOnClickListener {
                    GameFolderPropertiesDialogFragment.newInstance(model)
                        .show(
                            activity.supportFragmentManager,
                            GameFolderPropertiesDialogFragment.TAG
                        )
                }

                buttonDelete.setOnClickListener {
                    gamesViewModel.removeFolder(model)
                }
            }
        }
    }
}
