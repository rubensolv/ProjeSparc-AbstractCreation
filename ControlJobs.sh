#!/bin/bash


cd /home/ps11489/dados/ABPGSLevi

(qsub -l nodes=1:STANDARD:ppn=1,mem=1gb qsub_Abstract_PortfolioOnline.sh) &
