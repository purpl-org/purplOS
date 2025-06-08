#!/usr/bin/env bash

# Get the current date of the local system which we know is accurate.
# We are going to use this along with other information to indentify
# the person collecting data.
DATE=`date +%Y-%m-%d`

# Get the directory of this script
SCRIPT_PATH=$(dirname $([ -L $0 ] && echo "$(dirname $0)/$(readlink -n $0)" || echo $0))
SCRIPT_NAME=$(basename ${0})
GIT=`which git`
if [ -z $GIT ]
then
    echo git not found
    exit 1
fi
TOPLEVEL=`$GIT rev-parse --show-toplevel`

GIT_PROJ_ROOT=`git rev-parse --show-toplevel`
source ${GIT_PROJ_ROOT}/project/victor/scripts/victor_env.sh

source ${GIT_PROJ_ROOT}/project/victor/scripts/host_robot_ip_override.sh

robot_set_host

robot_sh rm -rf /data/data/com.anki.victor/cache/camera/images/*
robot_sh "printf ${DATE} > /data/data/com.anki.victor/persistent/devImageCapturePrefix.txt"
