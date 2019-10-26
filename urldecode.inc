#!/bin/bash
urldecode () 
{ 
    author 'Chris Down (https://gist.github.com/cdown/1163649)'
    about "decodes a percent-encoded string, e.g., a URL query string"
    param '$1 - the string to decode'
    example "urldecode 'some%20percent-encoded%20string' "
    group 'web'
    group ''
    local url_encoded="${1//+/ }"
    printf '%b' "${url_encoded//%/\\x}"
}
