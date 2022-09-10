#!/bin/bash

href=https://twitter.com/SenWhitehouse/status/909818723028127749
hrid=909818723028127749

function test_created {
    echo
    echo '# test_created'

    if twurl user get "https://api.twitter.com/2/tweets/${hrid}?tweet.fields=created_at" print
    then
        echo '#'
        return 0
    else
        echo ng
        return 1
    fi
}

function test_embeded {
    echo
    echo '# test_embeded'

    if reference=$(urlencode "${href}") &&
       twurl user get "https://publish.twitter.com/oembed?url=${reference}&maxwidth=500&maxheight=500&omit_script=1" print
    then
        echo '#'
        return 0
    else
        echo ng
        return 1
    fi
}

function test_together {
    echo
    echo '# test_together'

    if reference=$(urlencode "${href}") &&
       twurl user get "https://api.twitter.com/2/tweets/${hrid}?tweet.fields=created_at" user get "https://publish.twitter.com/oembed?url=${reference}&maxwidth=500&maxheight=500&omit_script=1" print
    then
        echo '#'
        return 0
    else
        echo ng
        return 1
    fi
}

test_created


test_embeded

test_together
