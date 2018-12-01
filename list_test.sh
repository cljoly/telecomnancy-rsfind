#!/usr/bin/env bash

jq -r '.commands | to_entries | .[] | "\(.key), rsfind: \(.value.a), find: \(.value.b)"' < test_data.json
