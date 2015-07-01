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
echo "Pulling skjeng-speedbox-loop..."

cd "$parent_path"
./lcd "Pulling" "update..."
cd skjeng-speedbox-loop
git pull

make
if [ ! -f "lcd" ]; then
	echo "LCD FAILED"
	exit
fi

cd "$parent_path"

systemctl daemon-reload
rm update.sh
rm lcd
systemctl start speedbox
echo "started speedbox"
echo "update.sh done"
