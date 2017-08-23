unmarkdown ()
{
    author 'Kevin Ernst'
    about 'Re-render Markdown as plain text with Pandoc'
    param '$1 - the markdown file (default: README.md)'
    example 'unmarkdown'
    example 'unmarkdown LESEMICH.md'
    group 'markdown'

    if [ $# -eq 0 -a -f README.md ]; then
        pandoc -f markdown -t plain README.md
    elif [ -f "$1" ]; then
        pandoc -f markdown -t plain "$1"
    else
        reference unmarkdown
    fi
}

# vim: ft=sh
