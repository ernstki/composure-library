ltail ()
{
    author 'Kevin Ernst'
    about 'tail the last modified file, optionally matching a prefix'
    param '$1 - the filename prefix to match'
    example 'ltail rmd-shiny  # tails last log file starting with "rmd-shiny"'
    group 'util'

    local pat=
    [[ -n ${1-} ]] && pat="$1*"

    tail -f $(
        \ls -t $pat | \
            while read thing; do
                if [[ -f $thing ]]; then
                    echo "$thing"
                    break
                fi
            done
    )
}
