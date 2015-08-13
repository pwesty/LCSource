#!/bin/bash

svn up && make cleanall && make kortest || exit 1;
./copyDevServer_10.1.90.30.sh

