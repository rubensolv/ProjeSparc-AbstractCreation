#!/bin/bash

cd /home/es91661/dados/ABPGSLevi

#directory in which the experiment files will be saved
OUTPUT_BASE=experiments/

#PLAYERS=("PortfolioOnlineEvolution 40 NOKDPS 1 0" "ABPOELimit")
PLAYERS_0=("PortfolioGreedySearch 40 NOKDPS 1 0")
PLAYERS_1=("GenerationClass 1" "GenerationClass 2" "GenerationClass 3" "GenerationClass 4" "GenerationClass 5" "GenerationClass 6" "GenerationClass 7" "GenerationClass 8" "GenerationClass 9" "GenerationClass 10")
BASE_COMBAT="State SeparatedState 200 128 128 400 360 840 360"
COMBAT_UNITS=("Protoss_Zealot 4" "Protoss_Zealot 8" "Protoss_Zealot 12" "Protoss_Zealot 16" "Protoss_Zealot 32" "Protoss_Zealot 50" "Protoss_Dragoon 4" "Protoss_Dragoon 8" "Protoss_Dragoon 12" "Protoss_Dragoon 16" "Protoss_Dragoon 32" "Protoss_Dragoon 50" "Terran_Marine 4" "Terran_Marine 8" "Terran_Marine 12" "Terran_Marine 16" "Terran_Marine 32" "Terran_Marine 50" "Zerg_Zergling 4" "Zerg_Zergling 8" "Zerg_Zergling 12" "Zerg_Zergling 16" "Zerg_Zergling 32" "Zerg_Zergling 50" "Protoss_Dragoon 4 Protoss_Zealot 4" "Protoss_Dragoon 8 Protoss_Zealot 8" "Protoss_Dragoon 12 Protoss_Zealot 12" "Protoss_Dragoon 16 Protoss_Zealot 16" "Protoss_Dragoon 25 Protoss_Zealot 25" "Protoss_Dragoon 4 Terran_Marine 4" "Protoss_Dragoon 8 Terran_Marine 8" "Protoss_Dragoon 12 Terran_Marine 12" "Protoss_Dragoon 16 Terran_Marine 16" "Protoss_Dragoon 25 Terran_Marine 25" "Zerg_Zergling 4 Terran_Marine 4" "Zerg_Zergling 8 Terran_Marine 8" "Zerg_Zergling 12 Terran_Marine 12" "Zerg_Zergling 16 Terran_Marine 16" "Zerg_Zergling 25 Terran_Marine 25" "Zerg_Zergling 4 Protoss_Zealot 4" "Zerg_Zergling 8 Protoss_Zealot 8" "Zerg_Zergling 12 Protoss_Zealot 12" "Zerg_Zergling 16 Protoss_Zealot 16" "Zerg_Zergling 25 Protoss_Zealot 25")


NUMBER_PLAYERS_0=${#PLAYERS_0[@]}
NUMBER_PLAYERS_1=${#PLAYERS_1[@]}

for (( i=0; i<${NUMBER_PLAYERS_0}; i++ ));
do
	for (( j=$i; j<${NUMBER_PLAYERS_1}; j++ ));
	do
		for COMBAT in "${COMBAT_UNITS[@]}" 
		do
			#######file where the details of the experiment are stored#######
			COMBAT_NO_SPACE="$(echo -e "${COMBAT}" | tr -d '[:space:]')"
			PLAYER_0_NO_SPACE="$(echo -e "${PLAYERS_0[$i]}" | tr -d '[:space:]')"
			PLAYER_1_NO_SPACE="$(echo -e "${PLAYERS_1[$j]}" | tr -d '[:space:]')"
			
			OUTPUT=${OUTPUT_BASE}${PLAYER_0_NO_SPACE}"_versus_"${PLAYER_1_NO_SPACE}"_"${COMBAT_NO_SPACE}".txt"						
			
			#######creating the file with experiment details#######
			echo "Player 0 "${PLAYERS_0[$i]} >> ${OUTPUT}
			echo "Player 1 "${PLAYERS_1[$j]} >> ${OUTPUT}
			
			echo ${BASE_COMBAT}" "${COMBAT} >> ${OUTPUT}
			
			SUMMARY_OUTPUT=${OUTPUT_BASE}"results/"${PLAYER_0_NO_SPACE}"_versus_"${PLAYER_1_NO_SPACE}"_"${COMBAT_NO_SPACE}
			echo "ResultsFile "${SUMMARY_OUTPUT}" true" >> ${OUTPUT}
			
			#######sending process to cluster's queue#######
			(qsub -l nodes=1:STANDARD:ppn=1,mem=1gb -v FILENAME=$OUTPUT FullEvalSparcraft.sh) &

			q=$(qselect -u es91661 | wc -l)
            while [ "$q" -gt "199" ]
            do
                echo "Sleeping for 10 minutes before trying to submit more jobs..."
                echo "$q"
                sleep 1000
                q=$(qselect -u es91661 | wc -l)
            done
		done
	done
done


