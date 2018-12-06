dropshadow ()
{
    author 'Kevin Ernst'
    about 'Adds a light border and a drop shadow to $1'
    param '$1 - the file to add a drop shadow to'
    example 'dropshadow image.jpg'
    group 'imageprocessing'

    local BORDER='-border 13'

    test -n "$1" || { reference dropshadow; return 1; }

    test -f "$1" -a -r "$1" || {
        echo "OH NOES! Input file '$1' is nonexistent/unreadable." >&2
        return 1
    }

    convert "$1" -bordercolor white $BORDER \
        \( +clone -background black -shadow 80x3+2+2 \) +swap \
        -background white -layers merge +repage \
        "${1%.*}+shadow.${1##*.}"
}
# vim: ft=sh
