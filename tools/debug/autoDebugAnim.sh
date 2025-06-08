#!/usr/bin/env bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

while true; do
    lldb $DIR/../../simulator/controllers/webotsCtrlAnim/webotsCtrlAnim -s $DIR/autoDebugAnim.lldb
    # sleep for a couple seconds so the user has time to hit ctrl+c
    echo "restarting lldb in 2 seconds, hit ctrl+c now to abort"
    sleep 2
done
