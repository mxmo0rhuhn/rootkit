#!/bin/bash - 

success() {
  if [ -z $1 ] ; then
    echo "no result" 
  else 
    echo "-->" $1
  fi
} 

case "$1" in
  remove)
    sudo rmmod sys_call_hijack.ko
    dmesg | grep hijack
    ;;
  compile)
    make
    sudo insmod sys_call_hijack.ko
    dmesg | grep hijack
    ;;
  call)
    dmesg | grep hijack
    cd
    touch .evil
    echo "file created"
    ls -al .evil
    bash -c "exec -a .evil sleep 600" &
    echo "process created"
    echo "ps -aux | grep .evil"
    ps -aux | grep .evil
    ;;
  fs)
    dmesg | grep hijack
    echo "grep hijack /proc/modules"
    success $(grep hijack /proc/modules)
    echo "ls /sys/module | grep hijack"
    success $(ls /sys/module | grep hijack)
    echo "modinfo sys_call_hijack"
    modinfo sys_call_hijack
#    echo "sudo modprobe -c | grep hijack"
#    success $(sudo modprobe -c | grep hijack)
    echo "grep hijack /proc/kallsyms "
    success $(grep hijack /proc/kallsyms )
    ;;
  *)
    echo "compile remove fs call"
    ;;
esac

