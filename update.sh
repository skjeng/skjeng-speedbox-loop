#!/bin/bash
# Cron job runs this from time to time

echo "update.sh"

systemctl stop speedbox
echo "stopped speedbox"

parent_path=$( cd "$(dirname "${BASH_SOURCE}")" ; pwd -P )
cd "$parent_path"

if [ ! -d "skjeng-speedbox-loop" ]; then
  cd "$parent_path"
  ./lcd "Initial" "clone..."
  cd skjeng-speedbox-loop
  echo "Initial cloning of skjeng-speedbox-loop..."
  git clone git@github.com:skjeng/skjeng-speedbox-loop.git
fi

cd skjeng-speedbox-loop
git remote update

LOCAL=$(git rev-parse @)
REMOTE=$(git rev-parse @{u})
BASE=$(git merge-base @ @{u})

if [ $LOCAL = $REMOTE ]; then
    echo "Up-to-date"

    cd "$parent_path"
    ./lcd "Software" "OK"
    cd skjeng-speedbox-loop

elif [ $LOCAL = $BASE ]; then
    echo "Need to pull"
    echo "Pulling skjeng-speedbox-loop..."
    
    cd "$parent_path"
    ./lcd "Pulling" "update..."
    cd skjeng-speedbox-loop

    git pull
elif [ $REMOTE = $BASE ]; then
    echo "Need to push, should never happen!"
else
    echo "Diverged"
fi

cd "$parent_path"

#systemctl start speedbox
echo "started speedbox"
