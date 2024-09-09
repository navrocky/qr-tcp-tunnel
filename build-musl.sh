#!/bin/bash

IMAGE_NAME=qrtt-cli-build

docker build --progress plain --tag ${IMAGE_NAME} -f Dockerfile.musl .

id=$(docker create ${IMAGE_NAME})
mkdir -p dist
docker cp $id:/build/cli/qrtt-cli ./dist/
docker rm -v $id
