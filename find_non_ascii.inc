find_non_ascii ()
{
    cite url
    author 'Kevin Ernst'
    about 'find non-ASCII characters in a named file (or stdin)'
    url 'https://stackoverflow.com/a/9395552'
    param '$1 (optional)  filename to grep for non-ASCII characters'
    example 'find_non_ascii README.md'
    group 'text'

    if [ -f "$1" ]; then 
        grep --color=tty -P -n "[\x80-\xFF]" "$1"
    else
        grep --color=tty -P -n "[\x80-\xFF]"
    fi
}

# vim: ft=sh
