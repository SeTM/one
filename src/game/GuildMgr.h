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

#ifndef _GUILDMGR_H
#define _GUILDMGR_H

#include "Common.h"
#include "Policies/Singleton.h"

class Guild;
class ObjectGuid;

struct GuildHousePosition
{
    float  target_X;
    float  target_Y;
    float  target_Z;
    uint32 target_mapId;
};

class GuildMgr
{
        typedef UNORDERED_MAP<uint32, Guild*> GuildMap;
        typedef UNORDERED_MAP<uint32, GuildHousePosition> GuildHousePositionMap;

        GuildMap m_GuildMap;
        GuildHousePositionMap mGuildHousePositions;
    public:
        GuildMgr();
        ~GuildMgr();

        void AddGuild(Guild* guild);
        void RemoveGuild(uint32 guildId);

        Guild* GetGuildById(uint32 guildId) const;
        Guild* GetGuildByName(std::string const& name) const;
        Guild* GetGuildByLeader(ObjectGuid const& guid) const;
        std::string GetGuildNameById(uint32 guildId) const;

        void LoadGuilds();
        void LoadGuildHousePositions();

        GuildHousePosition const* GetGuildHouseCoordById(uint32 guildId) const
        {
            GuildHousePositionMap::const_iterator itr = mGuildHousePositions.find(guildId);
            if (itr != mGuildHousePositions.end())
                return &itr->second;

            return NULL;
        }
        bool AddGuildHouse(uint32 guildId, GuildHousePosition ghp);
        bool DelGuildHouse(uint32 guildId);
};

#define sGuildMgr MaNGOS::Singleton<GuildMgr>::Instance()

MANGOS_DLL_SPEC GuildHousePosition const* GetGuildHouseCoorditates(uint32 guildId);

#endif // _GUILDMGR_H
