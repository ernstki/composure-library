#!/bin/bash
urlencode () 
{ 
    author 'Chris Down (https://gist.github.com/cdown/1163649)'
    about "percent-encodes string argument so it's a valid URL"
    param '$1 - the string to percent-encode'
    example "urlencode 'This is the subject argument to a mailto: link' "
    group 'web'
    old_lc_collate=$LC_COLLATE
    LC_COLLATE=C
    
    local length="${#1}"
    for (( i = 0; i < length; i++ )); do
        local c="${1:i:1}"
        case $c in
            [a-zA-Z0-9.~_-]) printf "$c" ;;
            *) printf '%%%02X' "'$c" ;;
        esac
    done
    
    LC_COLLATE=$old_lc_collate
}
