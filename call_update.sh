#!/bin/bash

# This copies and runs the update procedure

echo "update.sh started"

parent_path=$( cd "$(dirname "${BASH_SOURCE}")" ; pwd -P )
cd "$parent_path"

\cp update.sh ~/update.sh
\cp lcd ~/lcd

source ~/update.sh
echo "update.sh completed"
