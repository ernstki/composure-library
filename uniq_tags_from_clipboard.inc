uniq_tags_from_clipboard () 
{ 
    author 'Kevin Ernst'
    about 'Takes a space-separated list of tags from the clipboard, returns unique ones, sorted'
    param ''
    example 'uniq_tags_from_clipboard'
    group 'tags'

    pbpaste | \
        python -c 'import sys; print(" ".join(sorted(set(sys.stdin.read().strip().split(" ")))))' | \
        pbcopy
}
