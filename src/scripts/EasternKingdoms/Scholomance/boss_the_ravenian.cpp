/*
 * This file is part of the OregonCore Project. See AUTHORS file for Copyright information
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/* ScriptData
SDName: Boss_the_ravenian
SD%Complete: 100
SDComment:
SDCategory: Scholomance
EndScriptData */

#include "ScriptPCH.h"
#include "scholomance.h"

#define SPELL_TRAMPLE           15550
#define SPELL_CLEAVE            20691
#define SPELL_SUNDERINCLEAVE    25174
#define SPELL_KNOCKAWAY         10101

struct boss_theravenianAI : public ScriptedAI
{
    boss_theravenianAI(Creature *c) : ScriptedAI(c) {}

    uint32 Trample_Timer;
    uint32 Cleave_Timer;
    uint32 SunderingCleave_Timer;
    uint32 KnockAway_Timer;
    bool HasYelled;

    void Reset()
    {
        Trample_Timer = 24000;
        Cleave_Timer = 15000;
        SunderingCleave_Timer = 40000;
        KnockAway_Timer = 32000;
        HasYelled = false;
    }

    void JustDied(Unit * /*killer*/)
    {
        ScriptedInstance *pInstance = me->GetInstanceData();
        if (pInstance)
        {
            pInstance->SetData(DATA_THERAVENIAN_DEATH, 0);

            if (pInstance->GetData(TYPE_GANDLING) == IN_PROGRESS)
                me->SummonCreature(1853, 180.73f, -9.43856f, 75.507f, 1.61399f, TEMPSUMMON_DEAD_DESPAWN, 0);
        }
    }

    void EnterCombat(Unit * /*who*/)
    {
    }

    void UpdateAI(const uint32 diff)
    {
        if (!UpdateVictim())
            return;

        //Trample_Timer
        if (Trample_Timer <= diff)
        {
            DoCastVictim( SPELL_TRAMPLE);
            Trample_Timer = 10000;
        } else Trample_Timer -= diff;

        //Cleave_Timer
        if (Cleave_Timer <= diff)
        {
            DoCastVictim( SPELL_CLEAVE);
            Cleave_Timer = 7000;
        } else Cleave_Timer -= diff;

        //SunderingCleave_Timer
        if (SunderingCleave_Timer <= diff)
        {
            DoCastVictim( SPELL_SUNDERINCLEAVE);
            SunderingCleave_Timer = 20000;
        } else SunderingCleave_Timer -= diff;

        //KnockAway_Timer
        if (KnockAway_Timer <= diff)
        {
            DoCastVictim( SPELL_KNOCKAWAY);
            KnockAway_Timer = 12000;
        } else KnockAway_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_theravenian(Creature* pCreature)
{
    return new boss_theravenianAI (pCreature);
}

void AddSC_boss_theravenian()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_the_ravenian";
    newscript->GetAI = &GetAI_boss_theravenian;
    newscript->RegisterSelf();
}

