#!/usr/bin/bash
# shellcheck disable=SC1117
drollup ()
{
    author 'Kevin Ernst'
    about 'sum up column COL over DEPTH levels of hierarchy'
    param '-d|--depth DEPTH - summarize this many levels of hierarchy (default: 5)'
    param '-c|--column COL  - add up this column (default: 2)'
    param '-h|--humanize    - print byte sizes in "human" figures (K, M, G, etc.)'
    param '-F|--separator   - field separator (default: tab)'
    example "find . -printf '%p\t%s\n' | drollup  # defaults to '-d5 -c2' "
    group 'sysadmin'

    local DEFAULT_DEPTH=5
    local DEFAULT_COLUMN=2
    local DEFAULT_SEP=$'\t'
    local SORT depth column human sep infile

    (( ${TRACE:-} )) && set -x
    #set -uo pipefail

    while (( $# )); do
        case "$1" in
            -\?|--he*)
                reference drollup
                return
                ;;
            -d*|--depth*)
                if [[ $1 =~ --?d(epth=)?([1-9][0-9]*) ]]; then
                    depth=${BASH_REMATCH[2]}
                else
                    shift; depth=$1
                fi
                ;;
            -F*|-s*|-t*|--sep*)
                # accept -{F|s|t}SEP, -{F|s|t} SEP, --sep[arator]SEP and
                # --sep[arator] SEP
                if [[ $1 =~ --?([Fst]|sep.*=)(.*) ]]; then
                    sep=${BASH_REMATCH[2]}
                else
                    shift; sep=$1
                fi
                ;;
            -c*|--col*)
                if [[ $1 =~ --?c(ol.*=)?([1-9][0-9]*) ]]; then
                    column=${BASH_REMATCH[2]}
                else
                    shift; depth=$1
                fi
                ;;
            -h|--human*)
                human=1
                ;;
            *)
                if [[ -r $1 ]]; then
                    infile=$1
                else
                    echo >&2
                    echo "ERROR: Bad argument. Try '--help'." >&2
                    echo >&2
                    return 1
                fi
                ;;
        esac
        shift
    done

    # if no input file but stdin is a terminal, error out
    if [[ -z $infile && -t 0 ]]; then
        echo >&2
        echo "ERROR: expected input file (or piped input). Try '--help'." >&2
        echo >&2
        return 1
    fi

    [[ -z $depth ]]  && depth=$DEFAULT_DEPTH
    [[ -z $column ]] && column=$DEFAULT_COLUMN
    [[ -z $sep ]]    && sep=$DEFAULT_SEP

    # change sort key based on '-h' / '--human-readable' option
    if (( human )); then
      SORT='sort -k2,2 -rh'
    else
      SORT='sort -k2,2 -rn'
    fi

    # give $infile to 'awk' as an argument if it's set; otherwise stdin
    awk -F "$sep" -v HUMAN="$human" -v DEPTH="$depth" -v COLUMN="$column" \
        -v TRACE="$TRACE" '
        # sum up value in COLUMN for DEPTH levels of hierarchy
        {
            split($1, a, "/")
            # Ex.: ./dir1/dir2/dir3 -> [".", "dir1", "dir2", "dir3"]
            #      /dir1/dir2/dir3  -> ["", "dir1", "dir2", "dir3"]
            if (length(a) < DEPTH + 1) {
                if (TRACE)
                    print "Skipping " $1 "; too shallow" > "/dev/stderr"
                next
            }
            hierkey = a[1]
            # append the rest until we have DEPTH levels, separated by slashes
            for (i = 2; i <= DEPTH + 1; i++)
                hierkey = hierkey "/" a[i]
            if (TRACE) print hierkey " += " $COLUMN > "/dev/stderr"
            usages[hierkey] += $COLUMN
        }

        END {
            for (key in usages)
                print key "\t" (HUMAN ? humanize(usages[key]) : usages[key])
        }

        # return "humanized" size in bytes
        function humanize(b) {
            # check to see if b is divisible by next higher prefix
            if      (b % 1024 == b)     return b
            else if (b % 1024**2 == b)  return sprintf("%0.1fK", b/1024)
            else if (b % 1024**3 == b)  return sprintf("%0.1fM", b/1024**2)
            else if (b % 1024**4 == b)  return sprintf("%0.1fG", b/1024**3)
            else                        return sprintf("%0.1fT", b/1024**4)
        }' \
        ${infile:+"$infile"} \
      | $SORT  
} # drollup
