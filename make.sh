#!/bin/bash
# WONKAVATOR v0.9.1.1 MAKE.SH
# Usage: chmod +x make.sh && ./make.sh [clean|run|debug]
# If no arg: just builds

set -e

ACTION=${1:-all}

echo "================================="
echo "  WONKAVATOR v0.9.1.1 MAKE.SH"
echo "================================="

case $ACTION in
    clean)
        echo "[CMD] make clean"
        make clean
        ;;
    run)
        echo "[CMD] make run"
        make run
        ;;
    debug)
        echo "[CMD] make debug"
        make debug
        ;;
    all|build)
        echo "[CMD] make"
        make
        echo "[OK]  Build complete: ./wonkavator"
        ;;
    *)
        echo "Usage: ./make.sh [all|clean|run|debug]"
        exit 1
        ;;
esac

echo "================================="
