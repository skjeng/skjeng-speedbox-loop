#!/bin/bash
# Cron job runs this from time to time

echo "update.sh"

systemctl stop speedbox
echo "stopped speedbox"

parent_path=$( cd "$(dirname "${BASH_SOURCE}")" ; pwd -P )
cd "$parent_path"

if [ ! -d "skjeng-speedbox-loop" ]; then
  source lcd "Initial cloning" "Please wait"
  echo "Initial cloning of skjeng-speedbox-loop..."
  git clone git@github.com:skjeng/skjeng-speedbox-loop.git
fi

cd skjeng-speedbox-loop

LOCAL=$(git rev-parse @)
REMOTE=$(git rev-parse @{u})
BASE=$(git merge-base @ @{u})

if [ $LOCAL = $REMOTE ]; then
    echo "Up-to-date"
elif [ $LOCAL = $BASE ]; then
    echo "Need to pull"
elif [ $REMOTE = $BASE ]; then
    echo "Need to push"
else
    echo "Diverged"
fi

exit

cd ..
cd skjeng-speedbox-loop
echo "Pulling skjeng-speedbox-loop..."
git pull

cd "$parent_path"
./lcd "Done updating" "Wait..."

#systemctl start speedbox
echo "started speedbox"
