#!/bin/bash
set -e

./protolua.sh upgrade
# ./protolua.sh test --wat $@
./protolua.sh build --fast --sim $@
