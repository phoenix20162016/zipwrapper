#!/bin/bash

file_list=$(ls source)
for item in ${file_list[@]}; do

  echo "XXXX ${item}"
  s_sha=$(sha256sum source/$item)
  d_sha=$(sha256sum dest/$item)
  if [[ "$s_sha" != "$d_sha" ]]; then
    echo "sha does not match, $item"
  fi
done
