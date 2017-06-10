find_non_ascii ()
{
    author 'Kevin Ernst'
    about ''
    param ''
    example ''
    group ''
    grep --color=tty --color='auto' -P -n "[\x80-\xFF]" README.md
}
