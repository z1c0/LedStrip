#!/bin/sh
# /etc/init.d/ledserver

### BEGIN INIT INFO
# Provides: ledserver
# Required-Start: $remote_fs $syslog
# Required-Stop: $remote_fs $syslog
# Default-Start: 2 3 4 5
# Default-Stop: 0 1 6
# Description: Led Server Web Site
# Short-Description: Led Server Web Site
### END INIT INFO

LOGGER="/usr/bin/logger -t ledserver"
export PATH=$PATH:/usr/local/bin/
export NODE_PATH=$NODE_PATH:/usr/local/lib/node_modules

myEcho() {
  echo "$1"
  $LOGGER "$1"
}

case "$1" in
  start)
    myEcho "Starting LedServer"
    /usr/local/bin/forever start -p /root/.forever /home/pi/LedStrip/ledcontrol/bin/www
    ;;
  stop)
    myEcho "Stopping LedServer"
    exec /usr/local/bin/forever stopall
    ;;
  *)
  
  myEcho "Usage: /etc/init.d/ledserver {start|stop}"
  exit 1
  ;;
esac
exit 0

