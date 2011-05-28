/*
 * Copyright (C) 2005-2011 MaNGOS <http://getmangos.com/>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "Common.h"
#include "Player.h"
#include "WorldSession.h"
#include "Chat.h"
#include "Language.h"
#include "Log.h"
#include "SpellAuras.h"
#include "World.h"

bool ChatHandler::HandleVipDebuffCommand(char* /*args*/)
{   
    Player *chr = m_session->GetPlayer();

    //Different Checks
    if( chr->isInCombat() || chr->IsTaxiFlying() || chr->InBattleGround() || chr->HasStealthAura() || chr->HasFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH) || chr->isDead())
    {
        SendSysMessage(VIP_CANT_DO);
        SetSentErrorMessage(true);
        return false;
    }
    
    m_session->GetPlayer()->RemoveAurasDueToSpell(SPELL_ID_PASSIVE_RESURRECTION_SICKNESS);
    m_session->GetPlayer()->RemoveAurasDueToSpell(26013);

    return true;
}

bool ChatHandler::HandleVipMapCommand(char* /*args*/)
{
    Player *chr = m_session->GetPlayer();

    //Different Checks
    if( chr->isInCombat() || chr->IsTaxiFlying() || chr->InBattleGround() || chr->HasStealthAura() || chr->HasFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH) || chr->isDead())
    {
        SendSysMessage(VIP_CANT_DO);
        SetSentErrorMessage(true);
        return false;
    }

    PSendSysMessage(LANG_YOU_SET_EXPLORE_ALL, GetNameLink(m_session->GetPlayer()).c_str());
    for (uint8 i=0; i<PLAYER_EXPLORED_ZONES_SIZE; ++i)
    {
        m_session->GetPlayer()->SetFlag(PLAYER_EXPLORED_ZONES_1+i,0xFFFFFFFF); 
    }

    return true;
}

bool ChatHandler::HandleVipBankCommand(char* /*args*/)
{
    Player *chr = m_session->GetPlayer();

    //Different Checks
    if( chr->isInCombat() || chr->IsTaxiFlying() || chr->InBattleGround() || chr->HasStealthAura() || chr->HasFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH) || chr->isDead())
    {
        SendSysMessage(VIP_CANT_DO);
        SetSentErrorMessage(true);
        return false;
    }

    m_session->SendShowBank(m_session->GetPlayer()->GetObjectGuid());

    return true;
}

bool ChatHandler::HandleVipRepairCommand(char* /*args*/)
{
    Player *chr = m_session->GetPlayer();

    //Different Checks
    if( chr->isInCombat() || chr->IsTaxiFlying() || chr->InBattleGround() || chr->HasStealthAura() || chr->HasFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH) || chr->isDead())
    {
        SendSysMessage(VIP_CANT_DO);
        SetSentErrorMessage(true);
        return false;
    }
    
    // Repair items
    m_session->GetPlayer()->DurabilityRepairAll(false, 0, false);

    PSendSysMessage(LANG_YOUR_ITEMS_REPAIRED, GetNameLink(m_session->GetPlayer()).c_str());
    return true;
}

bool ChatHandler::HandleVipAuctionCommand(char* /*args*/)
{
    Player *chr = m_session->GetPlayer();

    //Different Checks
    if( chr->isInCombat() || chr->IsTaxiFlying() || chr->InBattleGround() || chr->HasStealthAura() || chr->HasFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH) || chr->isDead())
    {
        SendSysMessage(VIP_CANT_DO);
        SetSentErrorMessage(true);
        return false;
    }

    //m_session->GetPlayer()->SetAuctionAccessMode(0);
    if (chr->GetTeam() == HORDE)
        m_session->GetPlayer()->SetAuctionAccessMode(m_session->GetPlayer()->GetTeam() != ALLIANCE ? -1 : 0);
    else
        m_session->GetPlayer()->SetAuctionAccessMode(m_session->GetPlayer()->GetTeam() != HORDE ? -1 : 0);
        
    m_session->SendAuctionHello(m_session->GetPlayer());

    return true;
}

bool ChatHandler::HandleVipResetTalentsCommand(char* /*args*/)
{
    Player *chr = m_session->GetPlayer();

    //Different Checks
    if( chr->isInCombat() || chr->IsTaxiFlying() || chr->InBattleGround() || chr->HasStealthAura() || chr->HasFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH) || chr->isDead())
    {
        SendSysMessage(VIP_CANT_DO);
        SetSentErrorMessage(true);
        return false;
    }

    // Reset Talents
    m_session->GetPlayer()->resetTalents(true);
    m_session->GetPlayer()->SendTalentsInfoData(false);

    PSendSysMessage(LANG_RESET_TALENTS_ONLINE, GetNameLink(m_session->GetPlayer()).c_str());
    return true;
}


bool ChatHandler::HandleVipWhispersCommand(char* args)
{
    if(!*args)
    {
        PSendSysMessage(LANG_COMMAND_WHISPERACCEPTING, m_session->GetPlayer()->isAcceptWhispers() ?  GetMangosString(LANG_ON) : GetMangosString(LANG_OFF));
        return true;
    }

    bool value;
    if (!ExtractOnOff(&args, value))
    {
        SendSysMessage(LANG_USE_BOL);
        SetSentErrorMessage(true);
        return false;
    }

    // whisper on
    if (value)
    {
        m_session->GetPlayer()->SetAcceptWhispers(true);
        SendSysMessage(LANG_COMMAND_WHISPERON);
    }
    // whisper off
    else
    {
        m_session->GetPlayer()->SetAcceptWhispers(false);
        SendSysMessage(LANG_COMMAND_WHISPEROFF);
    }

    return true;
}

bool ChatHandler::HandleVipTaxiCommand(char* /*args*/)
{
    Player *chr = m_session->GetPlayer();

    //Different Checks
    if( chr->isInCombat() || chr->IsTaxiFlying() || chr->InBattleGround() || chr->HasStealthAura() || chr->HasFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH) || chr->isDead())
    {
        SendSysMessage(VIP_CANT_DO_2);
        SetSentErrorMessage(true);
        return false;
    }
    
    chr->SetTaxiCheater(true);
    PSendSysMessage(LANG_YOU_GIVE_TAXIS, GetNameLink(chr).c_str());
    if (needReportToTarget(chr))
        ChatHandler(chr).PSendSysMessage(LANG_YOURS_TAXIS_ADDED, GetNameLink().c_str());
    return true;
}

bool ChatHandler::HandleVipHomeCommand(char* /*args*/)
{
    Player *chr = m_session->GetPlayer();

    //Different Checks
    if( chr->isInCombat() || chr->IsTaxiFlying() || chr->InBattleGround() || chr->HasStealthAura() || chr->HasFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH) || chr->isDead())
    {
        SendSysMessage(VIP_CANT_DO);
        SetSentErrorMessage(true);
        return false;
    }

    // remove CD and cast spell
    chr->RemoveSpellCooldown(8690,true);
    chr->CastSpell(chr,8690,false);

    return true;
}

bool ChatHandler::HandleVipCapitalCommand(char* /*args*/)
{
    Player *chr = m_session->GetPlayer();

    //Different Checks
    if( chr->isInCombat() || chr->IsTaxiFlying() || chr->InBattleGround() || chr->HasStealthAura() || chr->HasFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH) || chr->isDead())
    {
        SendSysMessage(VIP_CANT_DO);
        SetSentErrorMessage(true);
        return false;
    }

    if (chr->GetTeam() == HORDE)
        chr->CastSpell(chr,3567,true);
    else
        chr->CastSpell(chr,3561,true);

    return true;
}