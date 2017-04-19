#!/bin/bash

EMAIL=root@server
SENDER="Door"

time=`date "+%H:%M"`
logger GPIO - DEBUG - door opened at $time
echo Door opened at $time | mail -s "Door opened $time" -a "From: $SENDER <$EMAIL>" $EMAIL

