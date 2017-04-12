#pragma once

#include "Common.h"
#include "Player.h"
#include <memory>

// search-based players
#include "Player_AlphaBeta.h"
#include "Player_AdaptiveBeamAlphaBeta.h"
#include "Player_PortfolioGreedySearch.h"
#include "Player_PortfolioGreedySearchDeep.h"
#include "Player_PortfolioGreedySearchCache.h"
#include "Player_PortfolioOnlineEvolutionDeep.h"
#include "Player_PortfolioOnlineEvolutionCache.h"
#include "Player_StratifiedPolicySearch.h"
#include "Player_IRStratifiedPolicySearch.h"
#include "Player_ImprovedPortfolioGreedySearch.h"
#include "Player_PortfolioOnlineEvolution.h"
#include "Player_AdaptableStratifiedPolicySearch.h"
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
#include "GenerationClass.h"
#include "GAB.h"
#include "DovePlayerAlpha.h"
#include "DovePlayerBeta.h"
#include "ABPOEFixo.h"
#include "ABPOELimit.h"
#include "ABPGSSand.h"
#include "ABPGSPlus.h"
#include "ABPGSwithRepeatAction.h"
#include "ABPOELimitWithRepeatAction.h"
#include "GenerationClassWithClosest.h"
#include "GenerationClassForMetric.h"
#include "GenerationClassForMetricSample.h"
#include "GenerationClassForMetricClosest.h"
#include "GenerationClassForDeep.h"
#include "ABPOESimetricoDeep.h"
#include "ABPGSSimetricoDeep.h"
#include "ABPOELimitDeep.h"
#include "GenerationClassForMonit.h"
#include "Player_PGSAA.h"
#include "AbstractScript.h"
#include "ABPGSSimetrico.h"
#include "ABPOESimetrico.h"
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
