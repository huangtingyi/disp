#!/bin/bash

curl ifconfig.me >$HOME/bin/ip.txt

diff $HOME/bin/ip.txt $HOME/bin/my-internet-ip.txt

if [ $? -eq 0 ]; then
	exit 0;
fi

cp $HOME/bin/ip.txt $HOME/bin/my-internet-ip.txt

$HOME/bin/putip.exp "$HOME"


