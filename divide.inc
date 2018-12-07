divide ()
{
    author 'Kevin Ernst'
    about 'Divides the input file into n parts'
    param '{-n|-p|--parts} N - divide $1 into N parts'
    param '$1 - input file to divide into N parts (default 50)'
    example 'divide -n 100 input.txt'
    group 'util'

    # set -x
    local DEFAULT_PARTS=50
    local parts inputfile inputlines basename ext linesperfile

    if [[ $# -lt 1 || $1 =~ ^--?h ]]; then
        reference divide
        return 1
    fi

    # make things easier below by preventing slashes in 'inputfile'
    if [[ $inputfile =~ / ]]; then
        echo "OH NOES! Sorry, no slashes allowed in 'inputfile' parameter." >&2
        return 1
    fi

    while (( $# )); do
        case $1 in
            -n*|-p*|--parts*)
                if [[ $1 =~ ^-(n|p|-parts=).+ ]]; then
                    parts=${1##-n}
                    parts=${1##-p}
                    parts=${1##--parts=}
                    shift
                    continue
                fi

                # otherwise, the option and its argument are separate
                shift

                if [[ -z $1 || -f $1 ]]; then
                    echo "OH NOES! Missing argument to 'parts' (-n) option." >&2
                    return 1
                fi

                parts=$1
                ;;

            *)
                # allow the number before or after the filename, too; if you
                # also use '-n', the last one wins

                # check to see if $1's an integer
                # source: http://stackoverflow.com/a/806923
                if [[ $1 =~ ^[0-9]+ ]]; then
                    parts=$1
                    shift
                    continue
                fi

                # otherwise, assume it's a filename (but check)
                inputfile=$1

                if [[ ! -f $inputfile ]]; then
                    echo "OH NOES! File '$inputfile' does not exist." >&2
                    return 1
                fi
                ;;
        esac

        shift  # otherwise, you'd loop infinitely. true story, bro.
    done

    # count lines in input file
    inputlines=$(cat "$inputfile" | wc -l)

    # this works, but is like 4× slower
    #inputlines=$(awk '{count++} END{print count}' "$inputfile")

    # divide the filename into basename and extension parts, but only if
    # the file seems to have an extension
    basename=${inputfile%%.*}
    [[ $inputfile =~ \..* ]] && ext=${inputfile#*.}
    # note that if the file ends with a period, that gets stripped

    # set default 'parts' if it wasn't specified on the command line
    if [[ -z $parts ]]; then
        echo "INFO: using default 'parts' (-n) of 50." >&2
        parts=$DEFAULT_PARTS
    fi

    linesperfile=$(( inputlines / parts ))

    # if there aren't actually enough lines to split...
    if (( linesperfile == 0 )); then
        echo "OH NOES! File isn't big enough to split into $parts parts." >&2
        return 1
    fi

    # same as 'split -a3 -dl ...'
    split --suffix-length=3 --numeric-suffixes \
        --lines=$linesperfile "$inputfile" "$basename"

    # add the extension back on to the parts (if there was one)
    if [[ -n $ext ]]; then 
        for part in $basename[0-9][0-9][0-9]; do
            mv "$part" "${part}$ext"
        done
    fi

    # set +x
}

# vim: ft=sh
