#!/bin/bash

function test_created {
    if twurl user get 'https://api.twitter.com/2/tweets/909818723028127749?expansions=author_id&tweet.fields=created_at&user.fields=username' print
    then
        echo 
        return 0
    else
        echo ng
        return 1
    fi
}

function test_embeded {
    
    if reference=$(urlencode "https://twitter.com/US_CYBERCOM/status/1425129511520190470") &&
       twurl user get "https://publish.twitter.com/oembed?url=${reference}&maxwidth=500&maxheight=500&omit_script=1" print
    then
        echo 
        return 0
    else
        echo ng
        return 1
    fi
}

function test_together {
    if reference=$(urlencode "https://twitter.com/US_CYBERCOM/status/1425129511520190470") &&
       twurl  user get 'https://api.twitter.com/2/tweets/909818723028127749?expansions=author_id&tweet.fields=created_at&user.fields=username' print user get "https://publish.twitter.com/oembed?url=${reference}&maxwidth=500&maxheight=500&omit_script=1" print
    then
        echo 
        return 0
    else
        echo ng
        return 1
    fi
}

if test_together
then
    exit 0
else
    exit 1
fi
