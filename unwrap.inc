unwrap ()
{
    author 'Kevin Ernst'
    about 'Unwraps text on stdin and squeezes multiple spaces.'
    about 'Good for reformatting Git log messages into GitHub PRs.'
    param '(none; operates on stdin)'
    example 'pbpaste | unwrap | pbcopy'
    group 'textutil'

    # source: https://stackoverflow.com/a/1252191
    pbpaste | sed ':a;N;$!ba;s/\n/ /g;s/  */ /g' | pbcopy
}
