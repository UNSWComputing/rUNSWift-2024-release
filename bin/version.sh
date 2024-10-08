#!/bin/bash
# This script returns the git summary of the code that is used as a version string
# We can later view this version information by running `runswift -v`
REALPATH=$(realpath "$0")
BIN_DIR=$(dirname "$REALPATH")
source "$BIN_DIR/source.sh"

cd "$RUNSWIFT_CHECKOUT_DIR"

cat << VERSION
Git branch: $(git rev-parse --abbrev-ref HEAD)
Git commit: $(git rev-parse HEAD)
Last built by ${USER-} on $(date)
VERSION
# I wanted to include this but i'm afraid the log will include funny characters that `genversion.sh` will need to escape
#Git log:    $(git log --pretty=oneline -1)
