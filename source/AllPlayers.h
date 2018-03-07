#pragma once

#include "Common.h"
#include "Player.h"
#include <memory>

// search-based players
#include "Player_AlphaBeta.h"
#include "Player_AdaptiveBeamAlphaBeta.h"
#include "Player_PortfolioGreedySearch.h"
#include "Player_PortfolioGreedySearchPlus.h"
#include "Player_PortfolioGreedySearchDavePlus.h"
#include "Player_PortfolioGreedySearchDavePlusShort.h"
#include "Player_PortfolioGreedySearchPlusShort.h"
#include "Player_StratifiedPolicySearch.h"
#include "Player_IRStratifiedPolicySearch.h"
#include "Player_ImprovedPortfolioGreedySearch.h"
#include "Player_PortfolioOnlineEvolution.h"
#include "Player_AdaptableStratifiedPolicySearch.h"
#include "Player_AdaptableStratifiedPolicySearchLimit.h"
#include "Player_UCT.h"

// script-based players
#include "Player_AttackClosest.h"
#include "Player_AttackDPS.h"
#include "Player_AttackWeakest.h"
#include "Player_Kiter.h"
#include "Player_KiterDPS.h"
#include "Player_NOKDPS.h"
#include "Player_Kiter_NOKDPS.h"
#include "Player_Cluster.h"
#include "Player_Random.h"
#include "GAB.h"
#include "SAB.h"
#include "SABSim.h"
#include "Player_PGSAA.h"
#include "AbstractScript.h"
#include "ABPGSSimetrico.h"
#include "Player_MoveForward.h"
#include "Player_MoveBackward.h"

namespace SparCraft
{
    
typedef std::shared_ptr<SparCraft::Player> PlayerPtr;

namespace AllPlayers
{
    Player * getPlayer(const IDType & playerID, const IDType & type);
    PlayerPtr getPlayerPtr(const IDType & playerID, const IDType & type);
}
}
