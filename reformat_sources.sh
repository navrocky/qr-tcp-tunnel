#!/bin/sh

find . \( -iname *.h -o -iname *.cpp \) -a \
    -not -path "./.git/*" \
    -not -path "./lib/3rdparty/*" \
    -not -path "./debug/*" \
    -not -path "./release/*" \
    | xargs clang-format -i
