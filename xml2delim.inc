#!/bin/bash
xml2delim ()
{
    author 'Kevin Ernst'
    about 'use XmlStarlet to output specific fields from XML input as tab-delimited'
    param 'XPATH                 - XPath selector for node of interest'
    param "-h|--help             - you're looking at it ;-)"
    param '-k|--key=XPATH        - sort key (XPath expression); can omit the "="'
    param "-n|--num[*]           - sort numerically; accepts '--num', '--number', etc."
    param '-t|--text             - sort on the key as text (dictionary order) (default)'
    param '-a|--asc[ending]      - sort in ascending order (default)'
    param '-d|--desc[ending]     - sort in descending order'
    param '-s|--sep[arator]=SEP  - output field delimiter; can omit "=" (default: tab)'
    param 'FIELD [FIELD...]      - literal string or XPath expression(s) to output'
    example 'xml2delim //item title dc:creator pubDate comments'
    example 'xml2delim //cl "'$pagetitle'" @title  # quote any literal strings'
    example 'xml2delim //c --sort-on @pages --sort-numeric --sort-desc . @pages'
    group 'xml'

    local ME='xml2delim'
    local DEFAULT_SORT_ORDER='A'  # ascending order
    local DEFAULT_SORT_TYPE='T'   # sort lexicographically (dictionary order)
    local DEFAULT_SEP=$'\t'       # tab-delimited by default
    local outputs=()              # could be XPaths or quoted strings
    local xpath sortkey sortorder sorttype concatexpr sep

    (( ${TRACE:-} )) && set -x; trap "set +x" RETURN

    if (( $# == 0 )); then
        reference "$ME"
        return
    fi

    while (( $# )); do
        case "$1" in
            -h|-\?|--h*)
                reference "$ME"
                return
                ;;
            /*)
                xpath=$1
                ;;
            -k|--key*)
                if [[ $1 =~ --?k(ey.*=)?(.+) ]]; then
                    sortkey=${BASH_REMATCH[2]}
                else
                    shift; sortkey=${1:?"missing sort key for '-k' / '--key' option"}
                fi
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
            -s*|-F*|--sep*|--delim)
                # also secretly accept '-F' like Awk ;-)
                if [[ $1 =~ --(sep.*|delim.*)=(.+) ]]; then
                    sep=${BASH_REMATCH[2]}
                elif [[ $1 =~ -(s|F)(.+) ]]; then
                    sep=${BASH_REMATCH[2]}
                else
                    shift; sep=$1
                    # catch at least one possible problem here
                    if [[ $sep == -* ]]; then
                        echo >&2
                        echo "ERROR: the '-s' / '--sep' option requires an"\
                             "argument." >&2
                        echo >&2
                        return 1
                    fi
                fi
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

    # this way it won't bomb if there's only one output field
    concatexpr='concat("", '

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
