#!/bin/bash

parent_path=$( cd "$(dirname "${BASH_SOURCE}")" ; pwd -P )
cd "$parent_path"

git remote update > /dev/null 2>&1
LOCAL=$(git rev-parse @)
REMOTE=$(git rev-parse @{u})
BASE=$(git merge-base @ @{u})

if [ $LOCAL = $REMOTE ]; then
    echo "up-to-date"
elif [ $LOCAL = $BASE ]; then
    echo "req pull"
elif [ $REMOTE = $BASE ]; then
    echo "req push"
else
    echo "diverged"
fi
