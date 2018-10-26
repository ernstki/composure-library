divide ()
{
    author 'Kevin Ernst'
    about 'Divides the input file into n parts'
    param '-n|--n-parts|--parts N - divide $1 into this many parts'
    param '$1 - input file to divide into N parts (default 50)'
    example 'divide -n 100 input.txt'
    group 'util'

    local parts=50 inputfile inputlines basename ext

    # make things easier below by preventing slashes in 'inputfile'
    if [[ $inputfile =~ / ]]; then
        echo "OH NOES! Sorry, no slashes allowed in 'inputfile' parameter." >&2
        return 1
    fi

    while (( $# )); do
        case $1 in
            -n*|--n-parts*|--parts*)
                if [[ $1 =~ ^-(n|-n-parts=|-parts=).+ ]]; then
                    parts=${1##-n}
                    parts=${1##--n-parts=}
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
                filename=$1

                if [[ ! -f $filename ]]; then
                    echo "OH NOES! File '$filename' does not exist." >&2
                    return 1
                fi
                ;;
        esac
    done

    # count lines in input file
    inputlines=$(awk '{count++} END{print count}' "$inputfile")

    # divide the filename into basename and extension parts
    basename=${inputfile%%.*}
    ext=${inputfile#*.}

    # same as 'split -a3 -dl ...'
    echo split --suffix-length=3 --numeric-suffixes \
        --lines=$(( inputlines / parts )) "$inputfile" "$basename"

    # add the extension back on to the parts
    for part in $basename[0-9][0-9][0-9]; do
        echo mv "$part" "${part}.$ext"
    done
}
