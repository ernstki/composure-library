#!/bin/bash
xml2delim ()
{
    author 'Kevin Ernst'
    about 'use XmlStarlet to output specific fields from XML input as tab-delimited'
    param '-s|--separator - output field delimiter (default: tab)'
    example 'xml2delim //item title dc:creator pubDate comments'
    example 'xml2delim //cl "'$pagetitle'" @title  # quote any literal strings'
    example 'xml2delim //c --sort-on @pages --sort-numeric --sort-desc . @pages'
    group 'xml'

    local ME='xml2delim'
    local DEFAULT_SORT_ORDER='D'  # descending order
    local DEFAULT_SORT_TYPE='N'   # numeric (as opposed to text)
    local DEFAULT_SEP=$'\t'       # tab-delimited by default
    local outputs=()              # could be XPaths or quoted strings
    local xpath sortkey sortorder sorttype concatexpr sep

    while (( $# )); do
        case "$1" in
            /*)
                xpath=$1
                ;;
            -n|--num*)
                sorttype='N'
                ;;
            -t|--text*)
                sorttype='T'
                ;;
            -a|--asc*)
                sortorder='A'
                ;;
            -d|--desc*)
                sortorder='D'
                ;;
            -k|--key*)
                if [[ $1 =~ --sort-key=(.*) ]]; then
                    sortkey=${BASH_REMATCH[1]}
                else
                    shift; sortkey=$1
                fi
                ;;
            -s|--sep*)
                sep=$1
                ;;
            *)
                outputs+=("$1")
                ;;
        esac
        shift
    done

    if [[ ( -n $sortorder || -n $sorttype ) && -z $sortkey ]]; then
        echo >&2
        echo "ERROR: sort order/type options also require a sort key (-k)" >&2
        echo "       Try '$ME --help'." >&2
        echo >&2
        return 1
    fi

    [[ -z $sep ]]       && sep=$DEFAULT_SEP
    [[ -z $sorttype ]]  && sorttype=$DEFAULT_SORT_TYPE
    [[ -z $sortorder ]] && sortorder=$DEFAULT_SORT_ORDER

    concatexpr='concat('

    # $sep-delimit all the XPath expressions we want concat'd in the output
    for output in "${outputs[@]:0:${#outputs[@]}-1}"; do
        concatexpr+="$output, '$sep', "
    done

    # tack on the last item and finish off the concat()
    concatexpr="$concatexpr${outputs[@]: -1})"

    xmlstarlet \
        sel -t \
            -m "$xpath" \
            ${sortkey:+-s $sortorder:$sorttype:- "$sortkey"} \
            -v "$concatexpr" \
            -n
} # xml2delim
