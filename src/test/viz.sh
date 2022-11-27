#!/usr/bin/env bash

bin='../../build/test'
output_dir='./output'

if [[ ! -d $output_dir ]]; then
    mkdir $output_dir
fi

$bin/tests | dot -Tsvg > $output_dir/graph.svg
