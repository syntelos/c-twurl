#!/bin/bash

if twurl user get 'https://api.twitter.com/2/tweets/909818723028127749?tweet.fields=created_at'
then
  echo 
  exit 0
else
  echo ng
  exit 1
fi
