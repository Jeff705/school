#!/bin/bash

LOGFILE=./schedule.log
rm $LOGFILE
touch $LOGFILE


swipl < sometests.input >> $LOGFILE
cat sometests.pro >> $LOGFILE


